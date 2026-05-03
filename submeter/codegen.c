#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

typedef struct MethodInfo MethodInfo;
typedef struct LocalVar LocalVar;
typedef struct StringConst StringConst;
typedef struct ExprValue ExprValue;

typedef struct {
    MethodInfo *methods;
    MethodInfo *current_method;
    LocalVar *locals;
    StringConst *strings;
    int temp_counter;
    int label_counter;
    int terminated;
} CodegenCtx;

struct MethodInfo {
    char *name;
    char *ret_type;
    char **param_types;
    char **param_names;
    int param_count;
    char *signature;
    char *llvm_name;
    int is_entry_main;
    Node *method_decl;
    MethodInfo *next;
};

struct LocalVar {
    char *name;
    char *type;
    char ptr[64];
    LocalVar *next;
};

struct StringConst {
    char *token;
    char *name;
    char *encoded;
    int length;
    StringConst *next;
};

struct ExprValue {
    char *type;
    char place[128];
};

static char *xstrdup(const char *s) {
    char *d;
    if (!s) return NULL;
    d = (char *)malloc(strlen(s) + 1);
    if (!d) return NULL;
    strcpy(d, s);
    return d;
}

static const char *node_type_to_juc_type(const Node *n) {
    if (!n || !n->type) return "undef";
    if (strcmp(n->type, "Int") == 0) return "int";
    if (strcmp(n->type, "Double") == 0) return "double";
    if (strcmp(n->type, "Bool") == 0) return "boolean";
    if (strcmp(n->type, "Void") == 0) return "void";
    if (strcmp(n->type, "StringArray") == 0) return "String[]";
    return "undef";
}

static const char *juc_to_llvm_type(const char *t) {
    if (!t) return "i32";
    if (strcmp(t, "int") == 0) return "i32";
    if (strcmp(t, "double") == 0) return "double";
    if (strcmp(t, "boolean") == 0) return "i1";
    if (strcmp(t, "void") == 0) return "void";
    if (strcmp(t, "String[]") == 0) return "i8**";
    return "i32";
}

static void sanitize_identifier(const char *in, char *out, size_t out_sz) {
    size_t i, j;
    if (!in || out_sz == 0) return;
    j = 0;
    for (i = 0; in[i] != '\0' && j + 1 < out_sz; i++) {
        unsigned char c = (unsigned char)in[i];
        if (isalnum(c) || c == '_') {
            out[j++] = (char)c;
        } else {
            if (j + 3 < out_sz) {
                out[j++] = '_';
                out[j++] = 'x';
                out[j++] = "0123456789ABCDEF"[(c >> 4) & 0xF];
                out[j++] = "0123456789ABCDEF"[c & 0xF];
            }
        }
    }
    out[j] = '\0';
}

static const char *type_code(const char *t) {
    if (strcmp(t, "int") == 0) return "i";
    if (strcmp(t, "double") == 0) return "d";
    if (strcmp(t, "boolean") == 0) return "b";
    if (strcmp(t, "String[]") == 0) return "s";
    if (strcmp(t, "void") == 0) return "v";
    return "u";
}

static char *build_signature(char **param_types, int param_count) {
    size_t len = 3;
    int i;
    char *s;

    for (i = 0; i < param_count; i++) {
        len += strlen(param_types[i]);
        if (i + 1 < param_count) len += 1;
    }

    s = (char *)malloc(len);
    if (!s) return NULL;
    s[0] = '\0';
    strcat(s, "(");
    for (i = 0; i < param_count; i++) {
        strcat(s, param_types[i]);
        if (i + 1 < param_count) strcat(s, ",");
    }
    strcat(s, ")");
    return s;
}

static char *build_method_llvm_name(const char *name, char **param_types, int param_count, int is_entry_main, const char *ret_type) {
    char base[256];
    char buf[512];
    int i;

    if (is_entry_main && strcmp(ret_type, "void") == 0 && param_count == 1 && strcmp(param_types[0], "String[]") == 0 && strcmp(name, "main") == 0) {
        return xstrdup("main");
    }

    sanitize_identifier(name, base, sizeof(base));
    snprintf(buf, sizeof(buf), "%s__", base);

    if (param_count == 0) {
        strcat(buf, "v");
    } else {
        for (i = 0; i < param_count; i++) {
            strcat(buf, type_code(param_types[i]));
            if (i + 1 < param_count) strcat(buf, "_");
        }
    }

    return xstrdup(buf);
}

static MethodInfo *find_method(CodegenCtx *ctx, const char *name, const char *signature) {
    MethodInfo *m = ctx->methods;
    while (m) {
        if (strcmp(m->name, name) == 0) {
            if (signature && strcmp(m->signature, signature) == 0) return m;
        }
        m = m->next;
    }
    return NULL;
}

static LocalVar *find_local(CodegenCtx *ctx, const char *name) {
    LocalVar *v = ctx->locals;
    while (v) {
        if (strcmp(v->name, name) == 0) return v;
        v = v->next;
    }
    return NULL;
}

static int new_temp(CodegenCtx *ctx) {
    return ++ctx->temp_counter;
}

static int new_label(CodegenCtx *ctx) {
    return ++ctx->label_counter;
}

static void strip_underscores(const char *in, char *out, size_t out_sz) {
    size_t i, j = 0;
    for (i = 0; in && in[i] != '\0' && j + 1 < out_sz; i++) {
        if (in[i] != '_') out[j++] = in[i];
    }
    out[j] = '\0';
}

static StringConst *intern_string(CodegenCtx *ctx, const char *token) {
    StringConst *it;
    StringConst *sc;
    char name[64];
    char *enc;
    size_t alloc;
    size_t j;
    size_t i;
    int bytes = 0;

    for (it = ctx->strings; it; it = it->next) {
        if (strcmp(it->token, token) == 0) return it;
    }

    alloc = strlen(token) * 4 + 8;
    enc = (char *)malloc(alloc);
    if (!enc) return NULL;

    j = 0;
    for (i = 1; token[i] != '\0' && token[i + 1] != '\0'; i++) {
        unsigned char c = (unsigned char)token[i];
        if (c == '\\' && token[i + 1] != '\0' && token[i + 1] != '"') {
            i++;
            switch (token[i]) {
                case 'n': c = 0x0A; break;
                case 'r': c = 0x0D; break;
                case 't': c = 0x09; break;
                case 'f': c = 0x0C; break;
                case '\\': c = 0x5C; break;
                default: c = (unsigned char)token[i]; break;
            }
        } else if (c == '\\' && token[i + 1] == '"') {
            i++;
            c = 0x22;
        }

        if (j + 4 >= alloc) break;
        enc[j++] = '\\';
        enc[j++] = "0123456789ABCDEF"[(c >> 4) & 0xF];
        enc[j++] = "0123456789ABCDEF"[c & 0xF];
        bytes++;
    }
    enc[j] = '\0';

    sc = (StringConst *)malloc(sizeof(StringConst));
    if (!sc) {
        free(enc);
        return NULL;
    }

    snprintf(name, sizeof(name), ".str.%d", new_label(ctx));
    sc->token = xstrdup(token);
    sc->name = xstrdup(name);
    sc->encoded = enc;
    sc->length = bytes + 1;
    sc->next = ctx->strings;
    ctx->strings = sc;

    return sc;
}

static void collect_methods(Node *root, CodegenCtx *ctx) {
    Node *class_id;
    Node *member;
    MethodInfo *tail = NULL;

    if (!root || strcmp(root->type, "Program") != 0) return;

    class_id = root->child;
    member = class_id ? class_id->sibling : NULL;

    while (member) {
        if (strcmp(member->type, "MethodDecl") == 0) {
            Node *header = member->child;
            Node *ret_node = header ? header->child : NULL;
            Node *id_node = ret_node ? ret_node->sibling : NULL;
            Node *params_node = id_node ? id_node->sibling : NULL;
            int count = 0;
            Node *pd = params_node ? params_node->child : NULL;
            MethodInfo *m;
            int i;

            while (pd) {
                count++;
                pd = pd->sibling;
            }

            m = (MethodInfo *)calloc(1, sizeof(MethodInfo));
            if (!m) return;

            m->name = xstrdup(id_node ? id_node->value : "");
            m->ret_type = xstrdup(node_type_to_juc_type(ret_node));
            m->param_count = count;
            m->param_types = (char **)malloc(sizeof(char *) * (count > 0 ? count : 1));
            m->param_names = (char **)malloc(sizeof(char *) * (count > 0 ? count : 1));
            m->method_decl = member;

            pd = params_node ? params_node->child : NULL;
            i = 0;
            while (pd) {
                Node *t = pd->child;
                Node *id = t ? t->sibling : NULL;
                m->param_types[i] = xstrdup(node_type_to_juc_type(t));
                m->param_names[i] = xstrdup(id ? id->value : "");
                i++;
                pd = pd->sibling;
            }

            m->signature = build_signature(m->param_types, m->param_count);
            m->is_entry_main = (strcmp(m->name, "main") == 0 && strcmp(m->ret_type, "void") == 0 && m->param_count == 1 && strcmp(m->param_types[0], "String[]") == 0);
            m->llvm_name = build_method_llvm_name(m->name, m->param_types, m->param_count, m->is_entry_main, m->ret_type);

            if (!ctx->methods) ctx->methods = m;
            else tail->next = m;
            tail = m;
        }
        member = member->sibling;
    }
}

static void emit_string_constants(CodegenCtx *ctx) {
    StringConst *s = ctx->strings;
    while (s) {
        printf("@%s = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n", s->name, s->length, s->encoded);
        s = s->next;
    }
}

static int stmt_always_returns(Node *stmt) {
    Node *cur;
    if (!stmt || !stmt->type) return 0;

    if (strcmp(stmt->type, "Return") == 0) return 1;

    if (strcmp(stmt->type, "Block") == 0) {
        cur = stmt->child;
        while (cur) {
            if (stmt_always_returns(cur)) return 1;
            cur = cur->sibling;
        }
        return 0;
    }

    if (strcmp(stmt->type, "If") == 0) {
        Node *cond = stmt->child;
        Node *then_s = cond ? cond->sibling : NULL;
        Node *else_s = then_s ? then_s->sibling : NULL;
        return stmt_always_returns(then_s) && stmt_always_returns(else_s);
    }

    return 0;
}

static ExprValue cast_value(CodegenCtx *ctx, ExprValue v, const char *to_type) {
    ExprValue out = v;
    if (strcmp(v.type, to_type) == 0) return v;
    if (strcmp(v.type, "int") == 0 && strcmp(to_type, "double") == 0) {
        int t = new_temp(ctx);
        printf("  %%t%d = sitofp i32 %s to double\n", t, v.place);
        out.type = "double";
        snprintf(out.place, sizeof(out.place), "%%t%d", t);
    }
    return out;
}

static ExprValue gen_expr(CodegenCtx *ctx, Node *expr);
static int stmt_always_returns(Node *stmt);

static ExprValue load_identifier(CodegenCtx *ctx, const char *name) {
    ExprValue v;
    LocalVar *lv = find_local(ctx, name);
    int t;

    v.type = "int";
    strcpy(v.place, "0");

    if (lv) {
        t = new_temp(ctx);
        printf("  %%t%d = load %s, %s* %s\n", t, juc_to_llvm_type(lv->type), juc_to_llvm_type(lv->type), lv->ptr);
        v.type = lv->type;
        snprintf(v.place, sizeof(v.place), "%%t%d", t);
        return v;
    }

    t = new_temp(ctx);
    printf("  %%t%d = load i32, i32* @g_%s\n", t, name);
    v.type = "int";
    snprintf(v.place, sizeof(v.place), "%%t%d", t);
    return v;
}

static ExprValue gen_call(CodegenCtx *ctx, Node *call) {
    ExprValue out;
    Node *callee = call ? call->child : NULL;
    Node *arg = callee ? callee->sibling : NULL;
    ExprValue args[128];
    int argc = 0;
    MethodInfo *m = NULL;
    int i;

    out.type = call && call->sem_type ? call->sem_type : "undef";
    strcpy(out.place, "0");

    while (arg && argc < 128) {
        args[argc++] = gen_expr(ctx, arg);
        arg = arg->sibling;
    }

    if (callee && callee->value) {
        m = find_method(ctx, callee->value, callee->sem_type);
        if (!m) {
            MethodInfo *it = ctx->methods;
            while (it) {
                if (strcmp(it->name, callee->value) == 0 && it->param_count == argc) {
                    m = it;
                    break;
                }
                it = it->next;
            }
        }
    }

    if (!m) {
        out.type = "int";
        strcpy(out.place, "0");
        return out;
    }

    for (i = 0; i < argc && i < m->param_count; i++) {
        args[i] = cast_value(ctx, args[i], m->param_types[i]);
    }

    if (strcmp(m->ret_type, "void") == 0) {
        printf("  call void @%s(", m->llvm_name);
    } else {
        int t = new_temp(ctx);
        printf("  %%t%d = call %s @%s(", t, juc_to_llvm_type(m->ret_type), m->llvm_name);
        snprintf(out.place, sizeof(out.place), "%%t%d", t);
        out.type = m->ret_type;
    }

    for (i = 0; i < argc; i++) {
        if (i > 0) printf(", ");
        printf("%s %s", juc_to_llvm_type(args[i].type), args[i].place);
    }
    printf(")\n");

    if (strcmp(m->ret_type, "void") == 0) {
        out.type = "void";
        strcpy(out.place, "0");
    }

    return out;
}

static ExprValue gen_expr(CodegenCtx *ctx, Node *expr) {
    ExprValue v;
    char numbuf[256];

    v.type = "int";
    strcpy(v.place, "0");

    if (!expr || !expr->type) return v;

    if (strcmp(expr->type, "Natural") == 0) {
        strip_underscores(expr->value, numbuf, sizeof(numbuf));
        v.type = "int";
        snprintf(v.place, sizeof(v.place), "%s", numbuf);
        return v;
    }

    if (strcmp(expr->type, "Decimal") == 0) {
        strip_underscores(expr->value, numbuf, sizeof(numbuf));
        v.type = "double";
        snprintf(v.place, sizeof(v.place), "%s", numbuf);
        return v;
    }

    if (strcmp(expr->type, "BoolLit") == 0) {
        v.type = "boolean";
        snprintf(v.place, sizeof(v.place), "%s", strcmp(expr->value, "true") == 0 ? "1" : "0");
        return v;
    }

    if (strcmp(expr->type, "Identifier") == 0) {
        return load_identifier(ctx, expr->value);
    }

    if (strcmp(expr->type, "Call") == 0) {
        return gen_call(ctx, expr);
    }

    if (strcmp(expr->type, "ParseArgs") == 0) {
        Node *id = expr->child;
        Node *idx = id ? id->sibling : NULL;
        ExprValue base = load_identifier(ctx, id ? id->value : "");
        ExprValue i = gen_expr(ctx, idx);
        int t1 = new_temp(ctx);
        int t2 = new_temp(ctx);
        int t3 = new_temp(ctx);

        if (ctx->current_method && ctx->current_method->is_entry_main) {
            int tadj = new_temp(ctx);
            printf("  %%t%d = add i32 %s, 1\n", tadj, i.place);
            snprintf(i.place, sizeof(i.place), "%%t%d", tadj);
        }

        printf("  %%t%d = getelementptr inbounds i8*, i8** %s, i32 %s\n", t1, base.place, i.place);
        printf("  %%t%d = load i8*, i8** %%t%d\n", t2, t1);
        printf("  %%t%d = call i32 @atoi(i8* %%t%d)\n", t3, t2);

        v.type = "int";
        snprintf(v.place, sizeof(v.place), "%%t%d", t3);
        return v;
    }

    if (strcmp(expr->type, "Length") == 0) {
        if (ctx->current_method && ctx->current_method->is_entry_main) {
            int t = new_temp(ctx);
            printf("  %%t%d = sub i32 %%argc, 1\n", t);
            v.type = "int";
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }
        v.type = "int";
        strcpy(v.place, "0");
        return v;
    }

    if (strcmp(expr->type, "Assign") == 0) {
        Node *lhs = expr->child;
        Node *rhs = lhs ? lhs->sibling : NULL;
        LocalVar *lv = find_local(ctx, lhs ? lhs->value : "");
        ExprValue rv = gen_expr(ctx, rhs);

        if (lv) {
            rv = cast_value(ctx, rv, lv->type);
            printf("  store %s %s, %s* %s\n", juc_to_llvm_type(lv->type), rv.place, juc_to_llvm_type(lv->type), lv->ptr);
            v.type = lv->type;
            snprintf(v.place, sizeof(v.place), "%s", rv.place);
            return v;
        }

        printf("  store i32 %s, i32* @g_%s\n", rv.place, lhs ? lhs->value : "");
        v.type = "int";
        snprintf(v.place, sizeof(v.place), "%s", rv.place);
        return v;
    }

    if (strcmp(expr->type, "Plus") == 0 || strcmp(expr->type, "Minus") == 0 || strcmp(expr->type, "Not") == 0) {
        ExprValue a = gen_expr(ctx, expr->child);
        int t = new_temp(ctx);

        if (strcmp(expr->type, "Plus") == 0) return a;
        if (strcmp(expr->type, "Minus") == 0) {
            if (strcmp(a.type, "double") == 0) {
                printf("  %%t%d = fsub double 0.0, %s\n", t, a.place);
            } else {
                printf("  %%t%d = sub i32 0, %s\n", t, a.place);
            }
            v.type = a.type;
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }

        printf("  %%t%d = xor i1 %s, true\n", t, a.place);
        v.type = "boolean";
        snprintf(v.place, sizeof(v.place), "%%t%d", t);
        return v;
    }

    {
        Node *l = expr->child;
        Node *r = l ? l->sibling : NULL;
        ExprValue a = gen_expr(ctx, l);
        ExprValue b = gen_expr(ctx, r);
        int t = new_temp(ctx);

        if (strcmp(expr->type, "Add") == 0 || strcmp(expr->type, "Sub") == 0 || strcmp(expr->type, "Mul") == 0 || strcmp(expr->type, "Div") == 0 || strcmp(expr->type, "Mod") == 0) {
            const char *op_i = "add";
            const char *op_d = "fadd";
            if (strcmp(expr->type, "Sub") == 0) { op_i = "sub"; op_d = "fsub"; }
            if (strcmp(expr->type, "Mul") == 0) { op_i = "mul"; op_d = "fmul"; }
            if (strcmp(expr->type, "Div") == 0) { op_i = "sdiv"; op_d = "fdiv"; }
            if (strcmp(expr->type, "Mod") == 0) { op_i = "srem"; op_d = "frem"; }

            if (strcmp(a.type, "double") == 0 || strcmp(b.type, "double") == 0) {
                a = cast_value(ctx, a, "double");
                b = cast_value(ctx, b, "double");
                printf("  %%t%d = %s double %s, %s\n", t, op_d, a.place, b.place);
                v.type = "double";
            } else {
                printf("  %%t%d = %s i32 %s, %s\n", t, op_i, a.place, b.place);
                v.type = "int";
            }
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }

        if (strcmp(expr->type, "And") == 0 || strcmp(expr->type, "Or") == 0 || strcmp(expr->type, "Xor") == 0) {
            const char *op = strcmp(expr->type, "And") == 0 ? "and" : (strcmp(expr->type, "Or") == 0 ? "or" : "xor");
            const char *ty = strcmp(a.type, "boolean") == 0 ? "i1" : "i32";
            printf("  %%t%d = %s %s %s, %s\n", t, op, ty, a.place, b.place);
            v.type = a.type;
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }

        if (strcmp(expr->type, "Lshift") == 0 || strcmp(expr->type, "Rshift") == 0) {
            const char *op = strcmp(expr->type, "Lshift") == 0 ? "shl" : "ashr";
            printf("  %%t%d = %s i32 %s, %s\n", t, op, a.place, b.place);
            v.type = "int";
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }

        if (strcmp(expr->type, "Eq") == 0 || strcmp(expr->type, "Ne") == 0 || strcmp(expr->type, "Lt") == 0 || strcmp(expr->type, "Le") == 0 || strcmp(expr->type, "Gt") == 0 || strcmp(expr->type, "Ge") == 0) {
            const char *iop = "eq";
            const char *dop = "oeq";
            if (strcmp(expr->type, "Ne") == 0) { iop = "ne"; dop = "one"; }
            if (strcmp(expr->type, "Lt") == 0) { iop = "slt"; dop = "olt"; }
            if (strcmp(expr->type, "Le") == 0) { iop = "sle"; dop = "ole"; }
            if (strcmp(expr->type, "Gt") == 0) { iop = "sgt"; dop = "ogt"; }
            if (strcmp(expr->type, "Ge") == 0) { iop = "sge"; dop = "oge"; }

            if (strcmp(a.type, "double") == 0 || strcmp(b.type, "double") == 0) {
                a = cast_value(ctx, a, "double");
                b = cast_value(ctx, b, "double");
                printf("  %%t%d = fcmp %s double %s, %s\n", t, dop, a.place, b.place);
            } else if (strcmp(a.type, "boolean") == 0 && strcmp(b.type, "boolean") == 0) {
                printf("  %%t%d = icmp %s i1 %s, %s\n", t, iop, a.place, b.place);
            } else {
                printf("  %%t%d = icmp %s i32 %s, %s\n", t, iop, a.place, b.place);
            }

            v.type = "boolean";
            snprintf(v.place, sizeof(v.place), "%%t%d", t);
            return v;
        }
    }

    return v;
}

static void gen_statement(CodegenCtx *ctx, Node *stmt, const char *ret_type);

static void gen_print(CodegenCtx *ctx, Node *arg) {
    if (!arg) return;

    if (strcmp(arg->type, "StrLit") == 0) {
        StringConst *sc = intern_string(ctx, arg->value);
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* getelementptr inbounds ([%d x i8], [%d x i8]* @%s, i32 0, i32 0))\n", sc->length, sc->length, sc->name);
        return;
    }

    ExprValue v = gen_expr(ctx, arg);
    if (strcmp(v.type, "int") == 0) {
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %s)\n", v.place);
    } else if (strcmp(v.type, "double") == 0) {
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmt_double, i32 0, i32 0), double %s)\n", v.place);
    } else if (strcmp(v.type, "boolean") == 0) {
        int t = new_temp(ctx);
        printf("  %%t%d = select i1 %s, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str_true, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str_false, i32 0, i32 0)\n", t, v.place);
        printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %%t%d)\n", t);
    }
}

static void gen_statement_list(CodegenCtx *ctx, Node *stmt, const char *ret_type) {
    Node *cur = stmt;
    while (cur) {
        gen_statement(ctx, cur, ret_type);
        cur = cur->sibling;
    }
}

static void gen_statement(CodegenCtx *ctx, Node *stmt, const char *ret_type) {
    if (ctx->terminated) return;
    if (!stmt || !stmt->type) return;

    if (strcmp(stmt->type, "Block") == 0) {
        gen_statement_list(ctx, stmt->child, ret_type);
        return;
    }

    if (strcmp(stmt->type, "If") == 0) {
        Node *cond = stmt->child;
        Node *then_s = cond ? cond->sibling : NULL;
        Node *else_s = then_s ? then_s->sibling : NULL;
        ExprValue c = gen_expr(ctx, cond);
        int l_then = new_label(ctx);
        int l_else = new_label(ctx);
        int l_end = new_label(ctx);
        int then_terminated;
        int else_terminated;
        int then_syntactic_return = stmt_always_returns(then_s);
        int else_syntactic_return = stmt_always_returns(else_s);

        printf("  br i1 %s, label %%L%d, label %%L%d\n", c.place, l_then, l_else);
        printf("L%d:\n", l_then);
        ctx->terminated = 0;
        gen_statement(ctx, then_s, ret_type);
        then_terminated = ctx->terminated;
        if (!then_terminated && !then_syntactic_return) {
            printf("  br label %%L%d\n", l_end);
        }

        printf("L%d:\n", l_else);
        ctx->terminated = 0;
        gen_statement(ctx, else_s, ret_type);
        else_terminated = ctx->terminated;
        if (!else_terminated && !else_syntactic_return) {
            printf("  br label %%L%d\n", l_end);
        }

        if ((then_terminated || then_syntactic_return) && (else_terminated || else_syntactic_return)) {
            ctx->terminated = 1;
        } else {
            ctx->terminated = 0;
            printf("L%d:\n", l_end);
        }
        return;
    }

    if (strcmp(stmt->type, "While") == 0) {
        Node *cond = stmt->child;
        Node *body = cond ? cond->sibling : NULL;
        int l_cond = new_label(ctx);
        int l_body = new_label(ctx);
        int l_end = new_label(ctx);
        int body_terminated;
        printf("  br label %%L%d\n", l_cond);
        printf("L%d:\n", l_cond);
        {
            ExprValue c = gen_expr(ctx, cond);
            printf("  br i1 %s, label %%L%d, label %%L%d\n", c.place, l_body, l_end);
        }
        printf("L%d:\n", l_body);
        ctx->terminated = 0;
        gen_statement(ctx, body, ret_type);
        body_terminated = ctx->terminated;
        if (!body_terminated) {
            printf("  br label %%L%d\n", l_cond);
        }
        printf("L%d:\n", l_end);
        ctx->terminated = 0;
        return;
    }

    if (strcmp(stmt->type, "Return") == 0) {
        Node *expr = stmt->child;
        if (strcmp(ret_type, "void") == 0) {
            if (ctx->current_method && ctx->current_method->is_entry_main) {
                printf("  ret i32 0\n");
            } else {
                printf("  ret void\n");
            }
        } else {
            ExprValue v = gen_expr(ctx, expr);
            v = cast_value(ctx, v, ret_type);
            printf("  ret %s %s\n", juc_to_llvm_type(ret_type), v.place);
        }
        ctx->terminated = 1;
        return;
    }

    if (strcmp(stmt->type, "Print") == 0) {
        gen_print(ctx, stmt->child);
        return;
    }

    if (strcmp(stmt->type, "Assign") == 0 || strcmp(stmt->type, "Call") == 0 || strcmp(stmt->type, "ParseArgs") == 0) {
        (void)gen_expr(ctx, stmt);
        return;
    }
}

static void collect_locals_from_method_body(CodegenCtx *ctx, Node *body) {
    Node *item = body ? body->child : NULL;
    while (item) {
        if (strcmp(item->type, "VarDecl") == 0) {
            Node *t = item->child;
            Node *id = t ? t->sibling : NULL;
            LocalVar *lv = (LocalVar *)calloc(1, sizeof(LocalVar));
            if (lv && id && id->value) {
                char safe[128];
                lv->name = xstrdup(id->value);
                lv->type = xstrdup(node_type_to_juc_type(t));
                sanitize_identifier(id->value, safe, sizeof(safe));
                snprintf(lv->ptr, sizeof(lv->ptr), "%%v_%s", safe);
                lv->next = ctx->locals;
                ctx->locals = lv;
            }
        }
        item = item->sibling;
    }
}

static void setup_params(CodegenCtx *ctx, MethodInfo *m) {
    int i;
    for (i = 0; i < m->param_count; i++) {
        LocalVar *lv = (LocalVar *)calloc(1, sizeof(LocalVar));
        char safe[128];
        if (!lv) continue;
        lv->name = xstrdup(m->param_names[i]);
        lv->type = xstrdup(m->param_types[i]);
        sanitize_identifier(m->param_names[i], safe, sizeof(safe));
        snprintf(lv->ptr, sizeof(lv->ptr), "%%v_%s", safe);
        lv->next = ctx->locals;
        ctx->locals = lv;
    }
}

static void emit_allocas_and_param_stores(CodegenCtx *ctx, MethodInfo *m) {
    LocalVar *lv = ctx->locals;
    int arg_index = 0;

    while (lv) {
        printf("  %s = alloca %s\n", lv->ptr, juc_to_llvm_type(lv->type));
        lv = lv->next;
    }

    lv = ctx->locals;
    while (lv) {
        int i;
        for (i = 0; i < m->param_count; i++) {
            if (strcmp(lv->name, m->param_names[i]) == 0) {
                if (m->is_entry_main && strcmp(lv->type, "String[]") == 0) {
                    printf("  store i8** %%argv, i8*** %s\n", lv->ptr);
                } else {
                    printf("  store %s %%arg%d, %s* %s\n", juc_to_llvm_type(lv->type), arg_index, juc_to_llvm_type(lv->type), lv->ptr);
                    arg_index++;
                }
                break;
            }
        }
        lv = lv->next;
    }
}

static void emit_method(CodegenCtx *ctx, MethodInfo *m) {
    Node *header = m->method_decl ? m->method_decl->child : NULL;
    Node *body = header ? header->sibling : NULL;
    Node *item;
    int i;

    ctx->current_method = m;
    ctx->locals = NULL;
    ctx->temp_counter = 0;
    ctx->terminated = 0;

    setup_params(ctx, m);
    collect_locals_from_method_body(ctx, body);

    if (m->is_entry_main) {
        printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
    } else {
        printf("define %s @%s(", juc_to_llvm_type(m->ret_type), m->llvm_name);
        for (i = 0; i < m->param_count; i++) {
            if (i > 0) printf(", ");
            printf("%s %%arg%d", juc_to_llvm_type(m->param_types[i]), i);
        }
        printf(") {\n");
    }

    emit_allocas_and_param_stores(ctx, m);

    item = body ? body->child : NULL;
    while (item) {
        if (ctx->terminated) break;
        if (strcmp(item->type, "VarDecl") != 0) {
            gen_statement(ctx, item, m->ret_type);
        }
        item = item->sibling;
    }

    if (ctx->terminated) {
        printf("}\n\n");
        return;
    }

    if (m->is_entry_main) {
        printf("  ret i32 0\n");
    } else if (strcmp(m->ret_type, "void") == 0) {
        printf("  ret void\n");
    } else if (strcmp(m->ret_type, "double") == 0) {
        printf("  ret double 0.0\n");
    } else if (strcmp(m->ret_type, "boolean") == 0) {
        printf("  ret i1 0\n");
    } else {
        printf("  ret i32 0\n");
    }

    printf("}\n\n");
}

static void emit_globals(Node *root) {
    Node *class_id = root ? root->child : NULL;
    Node *member = class_id ? class_id->sibling : NULL;

    while (member) {
        if (strcmp(member->type, "FieldDecl") == 0) {
            Node *t = member->child;
            Node *id = t ? t->sibling : NULL;
            const char *jt = node_type_to_juc_type(t);
            if (id && id->value) {
                if (strcmp(jt, "double") == 0) {
                    printf("@g_%s = global double 0.0\n", id->value);
                } else if (strcmp(jt, "boolean") == 0) {
                    printf("@g_%s = global i1 0\n", id->value);
                } else {
                    printf("@g_%s = global i32 0\n", id->value);
                }
            }
        }
        member = member->sibling;
    }
}

void generate_llvm(Node *root) {
    CodegenCtx ctx;
    MethodInfo *m;

    memset(&ctx, 0, sizeof(ctx));

    collect_methods(root, &ctx);

    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n\n");
    printf("@.fmt_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt_double = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt_str = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str_true = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str_false = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");

    emit_globals(root);
    if (root) printf("\n");

    for (m = ctx.methods; m; m = m->next) {
        emit_method(&ctx, m);
    }

    emit_string_constants(&ctx);
}

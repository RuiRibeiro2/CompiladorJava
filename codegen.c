#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

typedef struct MethodInfo MethodInfo;
typedef struct LocalVar LocalVar;
typedef struct StringConst StringConst;
typedef struct ExprValue ExprValue;

typedef struct
{
    MethodInfo *methods;
    MethodInfo *current_method;
    LocalVar *locals;
    StringConst *strings;
    int temp_counter;
    int label_counter;
    int logic_op_counter;
    int terminated;
} CodegenCtx;

struct MethodInfo
{
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

struct LocalVar
{
    char *name;
    char *type;
    char ptr[2048];
    LocalVar *next;
};

struct StringConst
{
    char *token;
    char *name;
    char *encoded;
    int length;
    StringConst *next;
};

struct ExprValue
{
    const char *type;
    char place[2048];
};

static char *xstrdup(const char *s)
{
    char *d;
    if (!s)
        return NULL;
    d = (char *)malloc(strlen(s) + 1);
    if (!d)
        return NULL;
    strcpy(d, s);
    return d;
}

static const char *node_type_to_juc_type(const Node *n)
{
    if (!n || !n->type)
        return "undef";
    if (strcmp(n->type, "Int") == 0)
        return "int";
    if (strcmp(n->type, "Double") == 0)
        return "double";
    if (strcmp(n->type, "Bool") == 0)
        return "boolean";
    if (strcmp(n->type, "Void") == 0)
        return "void";
    if (strcmp(n->type, "StringArray") == 0)
        return "String[]";
    return "undef";
}

static const char *juc_to_llvm_type(const char *t)
{
    if (!t)
        return "i32";
    if (strcmp(t, "int") == 0)
        return "i32";
    if (strcmp(t, "double") == 0)
        return "double";
    if (strcmp(t, "boolean") == 0)
        return "i1";
    if (strcmp(t, "void") == 0)
        return "void";
    if (strcmp(t, "String[]") == 0)
        return "i8**";
    return "i32";
}

static void sanitize_identifier(const char *in, char *out, size_t out_sz)
{
    size_t i, j = 0;
    if (!in || out_sz == 0)
        return;
    for (i = 0; in[i] != '\0' && j + 1 < out_sz; i++)
    {
        unsigned char c = (unsigned char)in[i];
        if (isalnum(c) || c == '_')
            out[j++] = (char)c;
        else if (j + 3 < out_sz)
        {
            out[j++] = '_';
            out[j++] = 'x';
            out[j++] = "0123456789ABCDEF"[(c >> 4) & 0xF];
            out[j++] = "0123456789ABCDEF"[c & 0xF];
        }
    }
    out[j] = '\0';
}

static const char *type_code(const char *t)
{
    if (strcmp(t, "int") == 0)
        return "i";
    if (strcmp(t, "double") == 0)
        return "d";
    if (strcmp(t, "boolean") == 0)
        return "b";
    if (strcmp(t, "String[]") == 0)
        return "s";
    if (strcmp(t, "void") == 0)
        return "v";
    return "u";
}

static char *build_signature(char **param_types, int param_count)
{
    size_t len = 3;
    for (int i = 0; i < param_count; i++)
        len += strlen(param_types[i]) + 1;

    char *s = (char *)malloc(len);
    s[0] = '\0';
    strcat(s, "(");

    for (int i = 0; i < param_count; i++)
    {
        strcat(s, param_types[i]);
        if (i + 1 < param_count)
            strcat(s, ",");
    }
    strcat(s, ")");
    return s;
}

static char *build_method_llvm_name(const char *name, char **param_types, int param_count, int is_entry_main, const char *ret_type)
{
    char base[1024] = {0};
    char buf[2048] = {0};

    if (is_entry_main && strcmp(name, "main") == 0)
    {
        return xstrdup("main");
    }

    sanitize_identifier(name, base, sizeof(base));
    snprintf(buf, sizeof(buf), "%s__", base);

    if (param_count == 0)
    {
        strcat(buf, "v");
    }
    else
    {
        for (int i = 0; i < param_count; i++)
        {
            strcat(buf, type_code(param_types[i]));
            if (i + 1 < param_count)
                strcat(buf, "_");
        }
    }
    return xstrdup(buf);
}

static MethodInfo *find_method(CodegenCtx *ctx, const char *name, const char *signature)
{
    for (MethodInfo *m = ctx->methods; m; m = m->next)
    {
        if (strcmp(m->name, name) == 0 && (!signature || strcmp(m->signature, signature) == 0))
        {
            return m;
        }
    }
    return NULL;
}

static LocalVar *find_local(CodegenCtx *ctx, const char *name)
{
    if (!name) return NULL;
    for (LocalVar *v = ctx->locals; v; v = v->next)
    {
        if (strcmp(v->name, name) == 0)
        {
            return v;
        }
    }
    return NULL;
}

static int new_temp(CodegenCtx *ctx)
{
    return ++ctx->temp_counter;
}

static int new_label(CodegenCtx *ctx)
{
    return ++ctx->label_counter;
}

static void strip_underscores(const char *in, char *out, size_t out_sz)
{
    size_t j = 0;
    for (size_t i = 0; in && in[i] != '\0' && j + 1 < out_sz; i++)
    {
        if (in[i] != '_')
            out[j++] = in[i];
    }
    out[j] = '\0';
}

static StringConst *intern_string(CodegenCtx *ctx, const char *token)
{
    for (StringConst *it = ctx->strings; it; it = it->next)
    {
        if (strcmp(it->token, token) == 0)
            return it;
    }

    size_t alloc = strlen(token) * 4 + 8;
    char *enc = (char *)malloc(alloc);

    int j = 0, bytes = 0;
    for (int i = 1; token[i] != '\0' && token[i + 1] != '\0'; i++)
    {
        unsigned char c = (unsigned char)token[i];
        if (c == '\\')
        {
            i++;
            switch (token[i])
            {
            case 'n': c = 0x0A; break;
            case 'r': c = 0x0D; break;
            case 't': c = 0x09; break;
            case 'f': c = 0x0C; break;
            case '\\': c = 0x5C; break;
            case '"': c = 0x22; break;
            default: c = (unsigned char)token[i]; break;
            }
        }
        enc[j++] = '\\';
        enc[j++] = "0123456789ABCDEF"[(c >> 4) & 0xF];
        enc[j++] = "0123456789ABCDEF"[c & 0xF];
        bytes++;
    }
    enc[j] = '\0';
    StringConst *sc = (StringConst *)malloc(sizeof(StringConst));

    char name[2048];
    snprintf(name, sizeof(name), ".str.%d", new_label(ctx));
    sc->token = xstrdup(token);
    sc->name = xstrdup(name);
    sc->encoded = enc;
    sc->length = bytes + 1;
    sc->next = ctx->strings;
    ctx->strings = sc;

    return sc;
}

static void collect_methods(Node *root, CodegenCtx *ctx)
{
    if (!root)
        return;
    Node *member = root->child ? root->child->sibling : NULL;
    MethodInfo *tail = NULL;
    while (member)
    {
        if (strcmp(member->type, "MethodDecl") == 0)
        {
            Node *ret = member->child->child;
            Node *id = ret->sibling;
            Node *params = id->sibling;
            int count = 0;
            for (Node *pd = params->child; pd; pd = pd->sibling)
                count++;
            
            MethodInfo *m = (MethodInfo *)calloc(1, sizeof(MethodInfo));
            m->name = xstrdup(id->value ? id->value : "");
            m->ret_type = xstrdup(node_type_to_juc_type(ret));
            m->param_count = count;
            m->param_types = (char **)malloc(sizeof(char *) * (count ? count : 1));
            m->param_names = (char **)malloc(sizeof(char *) * (count ? count : 1));
            m->method_decl = member;
            Node *pd = params->child;
            for (int i = 0; i < count; i++)
            {
                m->param_types[i] = xstrdup(node_type_to_juc_type(pd->child));
                m->param_names[i] = xstrdup(pd->child->sibling->value ? pd->child->sibling->value : "");
                pd = pd->sibling;
            }
            m->signature = build_signature(m->param_types, m->param_count);
            m->is_entry_main = (strcmp(m->name, "main") == 0 && count == 1 && strcmp(m->param_types[0], "String[]") == 0);
            m->llvm_name = build_method_llvm_name(m->name, m->param_types, m->param_count, m->is_entry_main, m->ret_type);
            if (!ctx->methods)
                ctx->methods = m;
            else
                tail->next = m;
            tail = m;
        }
        member = member->sibling;
    }
}

static int stmt_always_returns(Node *stmt)
{
    if (!stmt) return 0;
    if (strcmp(stmt->type, "Return") == 0) return 1;
    if (strcmp(stmt->type, "Block") == 0)
    {
        for (Node *c = stmt->child; c; c = c->sibling)
            if (stmt_always_returns(c)) return 1;
    }
    if (strcmp(stmt->type, "If") == 0)
    {
        return stmt_always_returns(stmt->child->sibling) && stmt_always_returns(stmt->child->sibling->sibling);
    }
    return 0;
}

// Scans ahead in the AST to safely pre-allocate stack slots for short-circuit evaluation 
static int count_logic_ops(Node *n) 
{
    if (!n) return 0;
    int count = 0;
    if (strcmp(n->type, "And") == 0 || strcmp(n->type, "Or") == 0) count = 1;
    return count + count_logic_ops(n->child) + count_logic_ops(n->sibling);
}

static ExprValue cast_value(CodegenCtx *ctx, ExprValue v, const char *to_type)
{
    if (strcmp(v.type, to_type) == 0) return v;
    if (strcmp(v.type, "int") == 0 && strcmp(to_type, "double") == 0)
    {
        int t = new_temp(ctx);
        printf("  %%t%d = sitofp i32 %s to double\n", t, v.place);
        v.type = "double";
        snprintf(v.place, 2048, "%%t%d", t);
    }
    return v;
}

static ExprValue gen_expr(CodegenCtx *ctx, Node *expr);

static ExprValue load_identifier(CodegenCtx *ctx, Node *expr)
{
    ExprValue v;
    const char *name = expr->value ? expr->value : "";
    LocalVar *lv = find_local(ctx, name);
    int t = new_temp(ctx);
    
    char sname[1024];
    sanitize_identifier(name, sname, sizeof(sname));
    
    v.type = lv ? lv->type : (expr->sem_type ? expr->sem_type : "int");
    printf("  %%t%d = load %s, %s* %s%s\n", t, juc_to_llvm_type(v.type), juc_to_llvm_type(v.type), lv ? "" : "@g_", lv ? lv->ptr : sname);
    snprintf(v.place, 2048, "%%t%d", t);
    return v;
}

static ExprValue gen_call(CodegenCtx *ctx, Node *call)
{
    ExprValue out;
    Node *callee = call->child, *arg = callee->sibling;
    ExprValue *args = (ExprValue *)calloc(1024, sizeof(ExprValue));
    int argc = 0;
    while (arg && argc < 1024)
    {
        args[argc++] = gen_expr(ctx, arg);
        arg = arg->sibling;
    }
    MethodInfo *m = find_method(ctx, callee->value, callee->sem_type);
    if (!m)
        for (m = ctx->methods; m; m = m->next)
            if (strcmp(m->name, callee->value) == 0 && m->param_count == argc)
                break;
    if (!m)
    {
        free(args);
        out.type = "int";
        strcpy(out.place, "0");
        return out;
    }
    for (int i = 0; i < argc; i++)
        args[i] = cast_value(ctx, args[i], m->param_types[i]);
    if (strcmp(m->ret_type, "void") == 0)
    {
        printf("  call void @%s(", m->llvm_name);
        out.type = "void";
        strcpy(out.place, "0");
    }
    else
    {
        int t = new_temp(ctx);
        printf("  %%t%d = call %s @%s(", t, juc_to_llvm_type(m->ret_type), m->llvm_name);
        out.type = m->ret_type;
        snprintf(out.place, 2048, "%%t%d", t);
    }
    for (int i = 0; i < argc; i++)
        printf("%s%s %s", i ? ", " : "", juc_to_llvm_type(args[i].type), args[i].place);
    printf(")\n");
    free(args);
    return out;
}

static ExprValue gen_expr(CodegenCtx *ctx, Node *expr)
{
    ExprValue v;
    char num[2048];
    v.type = "int";
    strcpy(v.place, "0");
    if (!expr || !expr->type) return v;
    
    if (strcmp(expr->type, "Natural") == 0)
    {
        strip_underscores(expr->value, num, 2048);
        snprintf(v.place, 2048, "%s", num);
        return v;
    }
    if (strcmp(expr->type, "Decimal") == 0)
    {
        strip_underscores(expr->value, num, 2048);
        v.type = "double";
        snprintf(v.place, 2048, "%.16e", strtod(num, NULL));
        return v;
    }
    if (strcmp(expr->type, "BoolLit") == 0)
    {
        v.type = "boolean";
        snprintf(v.place, 2048, "%s", strcmp(expr->value, "true") == 0 ? "1" : "0");
        return v;
    }
    if (strcmp(expr->type, "Identifier") == 0) return load_identifier(ctx, expr);
    if (strcmp(expr->type, "Call") == 0) return gen_call(ctx, expr);
    if (strcmp(expr->type, "ParseArgs") == 0)
    {
        ExprValue base = load_identifier(ctx, expr->child), i = gen_expr(ctx, expr->child->sibling);
        int t1 = new_temp(ctx), t2 = new_temp(ctx), t3 = new_temp(ctx);
        if (ctx->current_method->is_entry_main)
        {
            int t = new_temp(ctx);
            printf("  %%t%d = add i32 %s, 1\n", t, i.place);
            snprintf(i.place, 2048, "%%t%d", t);
        }
        printf("  %%t%d = getelementptr inbounds i8*, i8** %s, i32 %s\n  %%t%d = load i8*, i8** %%t%d\n  %%t%d = call i32 @atoi(i8* %%t%d)\n", t1, base.place, i.place, t2, t1, t3, t2);
        snprintf(v.place, 2048, "%%t%d", t3);
        return v;
    }
    if (strcmp(expr->type, "Length") == 0)
    {
        if (ctx->current_method->is_entry_main)
        {
            int t = new_temp(ctx);
            printf("  %%t%d = sub i32 %%argc, 1\n", t);
            snprintf(v.place, 2048, "%%t%d", t);
        }
        return v;
    }
    if (strcmp(expr->type, "Assign") == 0)
    {
        Node *lhs = expr->child, *rhs = lhs->sibling;
        LocalVar *lv = find_local(ctx, lhs->value);
        ExprValue rv = gen_expr(ctx, rhs);
        const char *ty = lv ? lv->type : (lhs->sem_type ? lhs->sem_type : "int");
        rv = cast_value(ctx, rv, ty);
        
        char sname[1024];
        sanitize_identifier(lhs->value, sname, sizeof(sname));
        
        printf("  store %s %s, %s* %s%s\n", juc_to_llvm_type(ty), rv.place, juc_to_llvm_type(ty), lv ? "" : "@g_", lv ? lv->ptr : sname);
        v.type = ty;
        strcpy(v.place, rv.place);
        return v;
    }
    if (strcmp(expr->type, "And") == 0 || strcmp(expr->type, "Or") == 0)
    {
        int tmp_idx = ctx->logic_op_counter++;
        int mid = new_label(ctx), end = new_label(ctx), fin = new_temp(ctx);
        ExprValue a = gen_expr(ctx, expr->child);
        printf("  store i1 %s, i1* %%logic_tmp_%d\n", a.place, tmp_idx);
        printf("  br i1 %s, label %%L%d, label %%L%d\nL%d:\n", a.place, strcmp(expr->type, "And") == 0 ? mid : end, strcmp(expr->type, "And") == 0 ? end : mid, mid);
        ExprValue b = gen_expr(ctx, expr->child->sibling);
        printf("  store i1 %s, i1* %%logic_tmp_%d\n  br label %%L%d\nL%d:\n  %%t%d = load i1, i1* %%logic_tmp_%d\n", b.place, tmp_idx, end, end, fin, tmp_idx);
        v.type = "boolean";
        snprintf(v.place, 2048, "%%t%d", fin);
        return v;
    }
    if (strcmp(expr->type, "Plus") == 0 || strcmp(expr->type, "Minus") == 0 || strcmp(expr->type, "Not") == 0)
    {
        ExprValue a = gen_expr(ctx, expr->child);
        if (strcmp(expr->type, "Plus") == 0)
            return a;
        int t = new_temp(ctx);
        v.type = a.type;
        if (strcmp(expr->type, "Not") == 0)
        {
            printf("  %%t%d = xor i1 %s, 1\n", t, a.place);
            v.type = "boolean";
        }
        else if (strcmp(a.type, "double") == 0)
            printf("  %%t%d = fsub double 0.0, %s\n", t, a.place);
        else
            printf("  %%t%d = sub i32 0, %s\n", t, a.place);
        snprintf(v.place, 2048, "%%t%d", t);
        return v;
    }
    Node *l = expr->child, *r = l->sibling;
    ExprValue a = gen_expr(ctx, l), b = gen_expr(ctx, r);
    int t = new_temp(ctx);
    if (strcmp(expr->type, "Add") == 0 || strcmp(expr->type, "Sub") == 0 || strcmp(expr->type, "Mul") == 0 || strcmp(expr->type, "Div") == 0 || strcmp(expr->type, "Mod") == 0)
    {
        int is_d = (strcmp(a.type, "double") == 0 || strcmp(b.type, "double") == 0);
        if (is_d)
        {
            a = cast_value(ctx, a, "double");
            b = cast_value(ctx, b, "double");
            v.type = "double";
        }
        const char *op = strcmp(expr->type, "Add") == 0 ? (is_d ? "fadd" : "add") : strcmp(expr->type, "Sub") == 0 ? (is_d ? "fsub" : "sub")
                                                                                : strcmp(expr->type, "Mul") == 0   ? (is_d ? "fmul" : "mul")
                                                                                : strcmp(expr->type, "Div") == 0   ? (is_d ? "fdiv" : "sdiv")
                                                                                                                   : (is_d ? "frem" : "srem");
        printf("  %%t%d = %s %s %s, %s\n", t, op, juc_to_llvm_type(v.type), a.place, b.place);
    }
    else if (strcmp(expr->type, "Eq") == 0 || strcmp(expr->type, "Ne") == 0 || strcmp(expr->type, "Lt") == 0 || strcmp(expr->type, "Le") == 0 || strcmp(expr->type, "Gt") == 0 || strcmp(expr->type, "Ge") == 0)
    {
        int is_d = (strcmp(a.type, "double") == 0 || strcmp(b.type, "double") == 0);
        if (is_d)
        {
            a = cast_value(ctx, a, "double");
            b = cast_value(ctx, b, "double");
        }
        const char *op = is_d ? "fcmp" : "icmp", *cond = strcmp(expr->type, "Eq") == 0 ? (is_d ? "oeq" : "eq") : strcmp(expr->type, "Ne") == 0 ? (is_d ? "one" : "ne")
                                                                                                             : strcmp(expr->type, "Lt") == 0   ? (is_d ? "olt" : "slt")
                                                                                                             : strcmp(expr->type, "Le") == 0   ? (is_d ? "ole" : "sle")
                                                                                                             : strcmp(expr->type, "Gt") == 0   ? (is_d ? "ogt" : "sgt")
                                                                                                                                               : (is_d ? "oge" : "sge");
        printf("  %%t%d = %s %s %s %s, %s\n", t, op, cond, is_d ? "double" : juc_to_llvm_type(a.type), a.place, b.place);
        v.type = "boolean";
    }
    else if (strcmp(expr->type, "Xor") == 0)
    {
        printf("  %%t%d = xor %s %s, %s\n", t, juc_to_llvm_type(a.type), a.place, b.place);
        v.type = a.type;
    }
    else if (strcmp(expr->type, "Lshift") == 0 || strcmp(expr->type, "Rshift") == 0)
        printf("  %%t%d = %s i32 %s, %s\n", t, strcmp(expr->type, "Lshift") == 0 ? "shl" : "ashr", a.place, b.place);
    snprintf(v.place, 2048, "%%t%d", t);
    return v;
}

static void gen_statement(CodegenCtx *ctx, Node *stmt, const char *ret_type)
{
    if (ctx->terminated || !stmt)
        return;
    if (strcmp(stmt->type, "Block") == 0)
        for (Node *c = stmt->child; c; c = c->sibling)
            gen_statement(ctx, c, ret_type);
    else if (strcmp(stmt->type, "If") == 0)
    {
        ExprValue c = gen_expr(ctx, stmt->child);
        int l1 = new_label(ctx), l2 = new_label(ctx), l3 = new_label(ctx);
        printf("  br i1 %s, label %%L%d, label %%L%d\nL%d:\n", c.place, l1, l2, l1);
        ctx->terminated = 0;
        gen_statement(ctx, stmt->child->sibling, ret_type);
        int t1 = ctx->terminated;
        if (!t1) printf("  br label %%L%d\n", l3);
        printf("L%d:\n", l2);
        ctx->terminated = 0;
        gen_statement(ctx, stmt->child->sibling->sibling, ret_type);
        int t2 = ctx->terminated;
        if (!t2) printf("  br label %%L%d\n", l3);
        if (t1 && t2) ctx->terminated = 1;
        else
        {
            ctx->terminated = 0;
            printf("L%d:\n", l3);
        }
    }
    else if (strcmp(stmt->type, "While") == 0)
    {
        int l1 = new_label(ctx), l2 = new_label(ctx), l3 = new_label(ctx);
        printf("  br label %%L%d\nL%d:\n", l1, l1);
        ExprValue c = gen_expr(ctx, stmt->child);
        printf("  br i1 %s, label %%L%d, label %%L%d\nL%d:\n", c.place, l2, l3, l2);
        ctx->terminated = 0;
        gen_statement(ctx, stmt->child->sibling, ret_type);
        if (!ctx->terminated) printf("  br label %%L%d\n", l1);
        printf("L%d:\n", l3);
        ctx->terminated = 0;
    }
    else if (strcmp(stmt->type, "Return") == 0)
    {
        if (strcmp(ret_type, "void") == 0)
            printf("  ret %s\n", ctx->current_method->is_entry_main ? "i32 0" : "void");
        else
        {
            ExprValue v = gen_expr(ctx, stmt->child);
            v = cast_value(ctx, v, ret_type);
            printf("  ret %s %s\n", juc_to_llvm_type(ret_type), v.place);
        }
        ctx->terminated = 1;
    }
    else if (strcmp(stmt->type, "Print") == 0)
    {
        if (strcmp(stmt->child->type, "StrLit") == 0)
        {
            StringConst *sc = intern_string(ctx, stmt->child->value);
            printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* getelementptr inbounds ([%d x i8], [%d x i8]* @%s, i32 0, i32 0))\n", sc->length, sc->length, sc->name);
        }
        else
        {
            ExprValue v = gen_expr(ctx, stmt->child);
            if (strcmp(v.type, "int") == 0)
                printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %s)\n", v.place);
            else if (strcmp(v.type, "double") == 0)
                printf("  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.fmt_double, i32 0, i32 0), double %s)\n", v.place);
            else
            {
                int t = new_temp(ctx);
                printf("  %%t%d = select i1 %s, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str_true, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str_false, i32 0, i32 0)\n  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %%t%d)\n", t, v.place, t);
            }
        }
    }
    else
        gen_expr(ctx, stmt);
}

static void collect_locals_from_method_body(CodegenCtx *ctx, Node *body)
{
    Node *item = body ? body->child : NULL;
    while (item)
    {
        if (strcmp(item->type, "VarDecl") == 0)
        {
            Node *t = item->child;
            Node *id = t ? t->sibling : NULL;
            LocalVar *lv = (LocalVar *)calloc(1, sizeof(LocalVar));
            if (lv && id && id->value)
            {
                char safe[1024];
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

static void emit_method(CodegenCtx *ctx, MethodInfo *m)
{
    Node *body = m->method_decl->child->sibling;
    ctx->current_method = m;
    ctx->locals = NULL;
    ctx->temp_counter = 0;
    ctx->logic_op_counter = 0;
    ctx->terminated = 0;
    
    for (int i = 0; i < m->param_count; i++)
    {
        LocalVar *lv = (LocalVar *)calloc(1, sizeof(LocalVar));
        char safe[1024];
        sanitize_identifier(m->param_names[i], safe, sizeof(safe));
        lv->name = xstrdup(m->param_names[i]);
        lv->type = xstrdup(m->param_types[i]);
        snprintf(lv->ptr, 2048, "%%v_%s", safe);
        lv->next = ctx->locals;
        ctx->locals = lv;
    }
    
    collect_locals_from_method_body(ctx, body);
    
    if (m->is_entry_main)
        printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
    else
    {
        printf("define %s @%s(", juc_to_llvm_type(m->ret_type), m->llvm_name);
        for (int i = 0; i < m->param_count; i++)
            printf("%s%s %%arg%d", i ? ", " : "", juc_to_llvm_type(m->param_types[i]), i);
        printf(") {\n");
    }
    
    // Allocate all normal variables
    for (LocalVar *lv = ctx->locals; lv; lv = lv->next)
        printf("  %s = alloca %s\n", lv->ptr, juc_to_llvm_type(lv->type));
        
    // Safely pre-allocate memory for logical short-circuiting to prevent stack overflow
    int logic_ops = count_logic_ops(body);
    for (int i = 0; i < logic_ops; i++) {
        printf("  %%logic_tmp_%d = alloca i1\n", i);
    }
    
    int arg_idx = 0;
    for (int i = 0; i < m->param_count; i++)
    {
        LocalVar *lv = find_local(ctx, m->param_names[i]);
        if (m->is_entry_main)
            printf("  store i8** %%argv, i8*** %s\n", lv->ptr);
        else
            printf("  store %s %%arg%d, %s* %s\n", juc_to_llvm_type(lv->type), arg_idx++, juc_to_llvm_type(lv->type), lv->ptr);
    }
    
    for (Node *it = body->child; it; it = it->sibling)
        if (strcmp(it->type, "VarDecl") != 0)
            gen_statement(ctx, it, m->ret_type);
            
    if (!ctx->terminated)
        printf("  ret %s\n", m->is_entry_main ? "i32 0" : (strcmp(m->ret_type, "void") == 0 ? "void" : (strcmp(m->ret_type, "double") == 0 ? "double 0.0" : "i32 0")));
    printf("}\n\n");
}

void generate_llvm(Node *root)
{
    CodegenCtx ctx;
    memset(&ctx, 0, sizeof(ctx));
    collect_methods(root, &ctx);
    
    printf("declare i32 @printf(i8*, ...)\ndeclare i32 @atoi(i8*)\n");
    printf("@.fmt_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt_double = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt_str = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str_true = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str_false = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");
    
    if (root && root->child)
        for (Node *m = root->child->sibling; m; m = m->sibling)
            if (strcmp(m->type, "FieldDecl") == 0)
            {
                const char *t = node_type_to_juc_type(m->child);
                char safe[1024];
                sanitize_identifier(m->child->sibling->value, safe, sizeof(safe));
                printf("@g_%s = global %s %s\n", safe, juc_to_llvm_type(t), strcmp(t, "double") == 0 ? "0.0" : "0");
            }
            
    for (MethodInfo *m = ctx.methods; m; m = m->next)
        emit_method(&ctx, m);
        
    for (StringConst *s = ctx.strings; s; s = s->next)
        printf("@%s = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n", s->name, s->length, s->encoded);
}
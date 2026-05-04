#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include "semantic.h"

static int semantic_errors = 0;

static const char *annotate_expr(Node *expr, ClassEntry *cls, MethodEntry *method);
static void annotate_statement(Node *stmt, ClassEntry *cls, MethodEntry *method);
static void annotate_method_body(Node *method_body, ClassEntry *cls, MethodEntry *method);

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

static void semantic_error_at(int line, int col, const char *fmt, ...)
{
    va_list ap;

    if (line < 1)
        line = 1;
    if (col < 1)
        col = 1;

    printf("Line %d, col %d: ", line, col);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");

    semantic_errors++;
}

static int is_reserved_name(const char *name)
{
    return name && strcmp(name, "_") == 0;
}

static int is_int(const char *t)
{
    return t && strcmp(t, "int") == 0;
}

static int is_double(const char *t)
{
    return t && strcmp(t, "double") == 0;
}

static int is_boolean(const char *t)
{
    return t && strcmp(t, "boolean") == 0;
}

static int is_numeric(const char *t)
{
    return is_int(t) || is_double(t);
}

static int is_printable(const char *t)
{
    return is_boolean(t) || is_int(t) || is_double(t);
}

static int type_equals(const char *a, const char *b)
{
    return a && b && strcmp(a, b) == 0;
}

static int is_type_assignable(const char *to_type, const char *from_type)
{
    int valid_to;
    int valid_from;

    if (!to_type || !from_type)
        return 0;

    valid_to = is_boolean(to_type) || is_int(to_type) || is_double(to_type);
    valid_from = is_boolean(from_type) || is_int(from_type) || is_double(from_type);

    if (!valid_to || !valid_from)
        return 0;

    if (type_equals(to_type, from_type))
        return 1;
    if (is_double(to_type) && is_int(from_type))
        return 1;

    return 0;
}

static const char *type_from_type_node(const Node *type_node)
{
    if (!type_node || !type_node->type)
        return "undef";
    if (strcmp(type_node->type, "Int") == 0)
        return "int";
    if (strcmp(type_node->type, "Double") == 0)
        return "double";
    if (strcmp(type_node->type, "Bool") == 0)
        return "boolean";
    if (strcmp(type_node->type, "Void") == 0)
        return "void";
    if (strcmp(type_node->type, "StringArray") == 0)
        return "String[]";
    return "undef";
}

static Symbol *find_symbol(Symbol *head, const char *name)
{
    Symbol *it = head;
    while (it)
    {
        if (strcmp(it->name, name) == 0)
            return it;
        it = it->next;
    }
    return NULL;
}

static const char *lookup_symbol_type(ClassEntry *cls, MethodEntry *method, const char *name)
{
    Symbol *sym;

    if (!name)
        return NULL;

    if (method)
    {
        sym = find_symbol(method->symbols, name);
        if (sym)
            return sym->type;
    }

    if (cls)
    {
        sym = find_symbol(cls->fields, name);
        if (sym)
            return sym->type;
    }

    return NULL;
}

static int same_signature(const MethodEntry *m, const char *name, const char **param_types, int param_count)
{
    int i;
    if (strcmp(m->name, name) != 0 || m->param_count != param_count)
        return 0;
    for (i = 0; i < param_count; i++)
    {
        if (strcmp(m->param_types[i], param_types[i]) != 0)
            return 0;
    }
    return 1;
}

static MethodEntry *find_method_by_signature(MethodEntry *head, const char *name, const char **param_types, int param_count)
{
    MethodEntry *it = head;
    while (it)
    {
        if (same_signature(it, name, param_types, param_count))
            return it;
        it = it->next;
    }
    return NULL;
}

static Symbol *append_symbol(Symbol **head, const char *name, const char *type, int is_param)
{
    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    Symbol *tail;

    if (!sym)
        return NULL;

    sym->name = xstrdup(name);
    sym->type = xstrdup(type);
    sym->is_param = is_param;
    sym->next = NULL;

    if (!*head)
    {
        *head = sym;
        return sym;
    }

    tail = *head;
    while (tail->next)
        tail = tail->next;
    tail->next = sym;
    return sym;
}

static MethodEntry *append_method(MethodEntry **head, const char *name, const char *return_type, const char **param_types, int param_count)
{
    MethodEntry *m = (MethodEntry *)malloc(sizeof(MethodEntry));
    MethodEntry *tail;
    int i;

    if (!m)
        return NULL;

    m->name = xstrdup(name);
    m->return_type = xstrdup(return_type);
    m->param_count = param_count;
    m->symbols = NULL;
    m->ast_node = NULL;
    m->next = NULL;

    if (param_count > 0)
    {
        m->param_types = (char **)malloc(sizeof(char *) * param_count);
        for (i = 0; i < param_count; i++)
            m->param_types[i] = xstrdup(param_types[i]);
    }
    else
    {
        m->param_types = NULL;
    }

    if (!*head)
    {
        *head = m;
        return m;
    }

    tail = *head;
    while (tail->next)
        tail = tail->next;
    tail->next = m;
    return m;
}

static Symbol *add_unique_symbol(Symbol **table, Node *id_node, const char *type, int is_param)
{
    const char *name;

    if (!id_node || !id_node->value)
        return NULL;

    name = id_node->value;

    if (is_reserved_name(name))
    {
        semantic_error_at(id_node->line, id_node->col, "Symbol _ is reserved");
        return NULL;
    }

    if (find_symbol(*table, name))
    {
        semantic_error_at(id_node->line, id_node->col, "Symbol %s already defined", name);
        return NULL;
    }

    return append_symbol(table, name, type, is_param);
}

static int collect_param_types(Node *params_node, const char **param_types, int max_params)
{
    int count = 0;
    Node *pd;

    if (!params_node)
        return 0;

    pd = params_node->child;
    while (pd && count < max_params)
    {
        Node *type_node = pd->child;
        param_types[count++] = type_from_type_node(type_node);
        pd = pd->sibling;
    }

    return count;
}

static void collect_field_decl(ClassEntry *cls, Node *field_decl)
{
    Node *type_node;
    Node *id_node;
    const char *type_name;

    if (!cls || !field_decl)
        return;

    type_node = field_decl->child;
    id_node = type_node ? type_node->sibling : NULL;
    if (!id_node || !id_node->value)
        return;

    type_name = type_from_type_node(type_node);
    Symbol *sym = add_unique_symbol(&cls->fields, id_node, type_name, 0);

    if (sym)
    {
        GlobalMember *gm = (GlobalMember *)malloc(sizeof(GlobalMember));
        gm->is_method = 0;
        gm->u.field = sym;
        gm->next = NULL;
        if (!cls->members)
            cls->members = gm;
        else
        {
            GlobalMember *tail = cls->members;
            while (tail->next)
                tail = tail->next;
            tail->next = gm;
        }
    }
}

static char *build_call_signature(const char *name, const char **arg_types, int argc)
{
    size_t len = strlen(name) + 3;
    for (int i = 0; i < argc; i++)
        len += strlen(arg_types[i]) + 1;
    char *sig = (char *)malloc(len);
    if (!sig)
        return NULL;

    strcpy(sig, name);
    strcat(sig, "(");
    for (int i = 0; i < argc; i++)
    {
        strcat(sig, arg_types[i]);
        if (i < argc - 1)
            strcat(sig, ",");
    }
    strcat(sig, ")");
    return sig;
}

static void collect_method_decl(ClassEntry *cls, Node *method_decl)
{
    Node *header;
    Node *return_type_node;
    Node *id_node;
    Node *params_node;
    const char *param_types_buffer[256];
    int param_count;
    const char *return_type;
    MethodEntry *method;
    Node *pd;

    if (!cls || !method_decl)
        return;

    header = method_decl->child;
    return_type_node = header ? header->child : NULL;
    id_node = return_type_node ? return_type_node->sibling : NULL;
    params_node = id_node ? id_node->sibling : NULL;

    if (!id_node || !id_node->value)
        return;

    if (is_reserved_name(id_node->value))
    {
        semantic_error_at(id_node->line, id_node->col,
                          "Symbol _ is reserved");
        return;
    }

    param_count = collect_param_types(
        params_node,
        param_types_buffer,
        256);

    if (find_method_by_signature(
            cls->methods,
            id_node->value,
            param_types_buffer,
            param_count))
    {
        Symbol *dummy_symbols = NULL;
        pd = params_node ? params_node->child : NULL;
        while (pd)
        {
            Node *type_node = pd->child;
            Node *param_id = type_node ? type_node->sibling : NULL;

            if (param_id && param_id->value)
            {
                add_unique_symbol(
                    &dummy_symbols,
                    param_id,
                    type_from_type_node(type_node),
                    1);
            }

            pd = pd->sibling;
        }

        while (dummy_symbols)
        {
            Symbol *next = dummy_symbols->next;
            free(dummy_symbols->name);
            free(dummy_symbols->type);
            free(dummy_symbols);
            dummy_symbols = next;
        }

        char *sig = build_call_signature(
            id_node->value,
            param_types_buffer,
            param_count);

        semantic_error_at(
            id_node->line,
            id_node->col,
            "Symbol %s already defined",
            sig ? sig : id_node->value);

        if (sig)
            free(sig);

        return;
    }

    return_type = type_from_type_node(return_type_node);

    method = append_method(
        &cls->methods,
        id_node->value,
        return_type,
        param_types_buffer,
        param_count);

    if (!method)
        return;

    method->ast_node = method_decl;

    GlobalMember *gm = (GlobalMember *)malloc(sizeof(GlobalMember));
    gm->is_method = 1;
    gm->u.method = method;
    gm->next = NULL;

    if (!cls->members)
    {
        cls->members = gm;
    }
    else
    {
        GlobalMember *tail = cls->members;
        while (tail->next)
            tail = tail->next;
        tail->next = gm;
    }

    append_symbol(&method->symbols, "return", return_type, 0);

    pd = params_node ? params_node->child : NULL;
    while (pd)
    {
        Node *type_node = pd->child;
        Node *param_id = type_node ? type_node->sibling : NULL;

        if (param_id && param_id->value)
        {
            add_unique_symbol(
                &method->symbols,
                param_id,
                type_from_type_node(type_node),
                1);
        }

        pd = pd->sibling;
    }
}

static ClassEntry *build_symbol_tables(Node *root)
{
    ClassEntry *cls;
    Node *class_id;
    Node *member;

    if (!root || strcmp(root->type, "Program") != 0)
        return NULL;

    cls = (ClassEntry *)malloc(sizeof(ClassEntry));
    if (!cls)
        return NULL;

    cls->fields = NULL;
    cls->methods = NULL;
    cls->members = NULL;

    class_id = root->child;
    cls->name = xstrdup(class_id && class_id->value ? class_id->value : "<anonymous>");

    member = class_id ? class_id->sibling : NULL;
    while (member)
    {
        if (strcmp(member->type, "FieldDecl") == 0)
        {
            collect_field_decl(cls, member);
        }
        else if (strcmp(member->type, "MethodDecl") == 0)
        {
            collect_method_decl(cls, member);
        }
        member = member->sibling;
    }

    return cls;
}

static void print_param_list(const MethodEntry *m)
{
    int i;
    printf("(");
    for (i = 0; i < m->param_count; i++)
    {
        printf("%s", m->param_types[i]);
        if (i + 1 < m->param_count)
            printf(",");
    }
    printf(")");
}

static void print_symbol_tables(const ClassEntry *cls)
{
    const MethodEntry *method;
    const Symbol *sym;

    if (!cls)
        return;

    printf("===== Class %s Symbol Table =====\n", cls->name);

    GlobalMember *gm = cls->members;
    while (gm)
    {
        if (gm->is_method)
        {
            MethodEntry *m = gm->u.method;
            printf("%s\t", m->name);
            print_param_list(m);
            printf("\t%s\n", m->return_type);
        }
        else
        {
            Symbol *f = gm->u.field;
            printf("%s\t\t%s\n", f->name, f->type);
        }
        gm = gm->next;
    }
    printf("\n");

    method = cls->methods;
    while (method)
    {
        printf("===== Method %s", method->name);
        print_param_list(method);
        printf(" Symbol Table =====\n");

        sym = method->symbols;
        while (sym)
        {
            if (sym->is_param)
            {
                printf("%s\t\t%s\tparam\n", sym->name, sym->type);
            }
            else
            {
                printf("%s\t\t%s\n", sym->name, sym->type);
            }
            sym = sym->next;
        }

        printf("\n");
        method = method->next;
    }
}

static void set_node_sem_type(Node *node, const char *type_name)
{
    if (!node)
        return;
    if (node->sem_type)
        free(node->sem_type);
    node->sem_type = xstrdup(type_name ? type_name : "undef");
}

static char *build_method_signature(const MethodEntry *m)
{
    char *sig;
    size_t size = 3; /* "(" + ")" + "\0" */
    int i;

    if (!m)
        return xstrdup("()");

    for (i = 0; i < m->param_count; i++)
    {
        size += strlen(m->param_types[i]);
        if (i + 1 < m->param_count)
            size += 1; /* comma */
    }

    sig = (char *)malloc(size);
    if (!sig)
        return NULL;

    sig[0] = '\0';

    strcat(sig, "(");
    for (i = 0; i < m->param_count; i++)
    {
        strcat(sig, m->param_types[i]);
        if (i + 1 < m->param_count)
            strcat(sig, ",");
    }
    strcat(sig, ")");

    return sig;
}

static const char *op_token_from_node(const char *node_type)
{
    if (!node_type)
        return "?";
    if (strcmp(node_type, "Add") == 0)
        return "+";
    if (strcmp(node_type, "Sub") == 0)
        return "-";
    if (strcmp(node_type, "Mul") == 0)
        return "*";
    if (strcmp(node_type, "Div") == 0)
        return "/";
    if (strcmp(node_type, "Mod") == 0)
        return "%";
    if (strcmp(node_type, "And") == 0)
        return "&&";
    if (strcmp(node_type, "Or") == 0)
        return "||";
    if (strcmp(node_type, "Xor") == 0)
        return "^";
    if (strcmp(node_type, "Lshift") == 0)
        return "<<";
    if (strcmp(node_type, "Rshift") == 0)
        return ">>";
    if (strcmp(node_type, "Eq") == 0)
        return "==";
    if (strcmp(node_type, "Ne") == 0)
        return "!=";
    if (strcmp(node_type, "Lt") == 0)
        return "<";
    if (strcmp(node_type, "Gt") == 0)
        return ">";
    if (strcmp(node_type, "Le") == 0)
        return "<=";
    if (strcmp(node_type, "Ge") == 0)
        return ">=";
    if (strcmp(node_type, "Minus") == 0)
        return "-";
    if (strcmp(node_type, "Plus") == 0)
        return "+";
    if (strcmp(node_type, "Not") == 0)
        return "!";
    return "?";
}

static void operator_error_one_type(Node *op, const char *token, const char *type_name)
{
    semantic_error_at(op ? op->line : 1, op ? op->col : 1,
                      "Operator %s cannot be applied to type %s",
                      token ? token : "?",
                      type_name ? type_name : "undef");
}

static void operator_error_two_types(Node *op, const char *token, const char *t1, const char *t2)
{
    semantic_error_at(op ? op->line : 1, op ? op->col : 1,
                      "Operator %s cannot be applied to types %s, %s",
                      token ? token : "?",
                      t1 ? t1 : "undef",
                      t2 ? t2 : "undef");
}

static int natural_out_of_bounds(const char *lit)
{
    char *clean;
    size_t i, j, len;
    int is_out_of_bounds = 0;

    if (!lit)
        return 0;

    len = strlen(lit);
    clean = (char *)malloc(len + 1);
    if (!clean)
        return 0;

    j = 0;
    for (i = 0; i < len; i++)
    {
        if (lit[i] != '_')
            clean[j++] = lit[i];
    }
    clean[j] = '\0';

    size_t clean_len = strlen(clean);

    if (clean_len > 10)
    {
        is_out_of_bounds = 1;
    }
    else if (clean_len < 10)
    {
        is_out_of_bounds = 0;
    }
    else
    {
        is_out_of_bounds = (strcmp(clean, "2147483647") > 0);
    }

    free(clean);
    return is_out_of_bounds;
}

static int decimal_out_of_bounds(const char *lit)
{
    char *clean;
    size_t i, j, len;
    double v;
    char *end;
    int has_nonzero_mantissa = 0;
    int in_exponent = 0;

    if (!lit)
        return 0;

    len = strlen(lit);
    clean = (char *)malloc(len + 1);
    if (!clean)
        return 0;

    j = 0;
    for (i = 0; i < len; i++)
    {
        if (lit[i] == 'e' || lit[i] == 'E')
            in_exponent = 1;

        if (lit[i] != '_')
        {
            clean[j++] = lit[i];
            if (!in_exponent && lit[i] >= '1' && lit[i] <= '9')
                has_nonzero_mantissa = 1;
        }
    }
    clean[j] = '\0';

    errno = 0;
    v = strtod(clean, &end);
    free(clean);

    if (v == 0.0 && has_nonzero_mantissa)
        return 1;

    if (v > 1.7976931348623157e+308 || v < -1.7976931348623157e+308)
        return 1;

    return 0;
}

static MethodEntry *find_method_entry_for_decl(
    ClassEntry *cls,
    Node *method_decl)
{
    Node *header;
    Node *return_type_node;
    Node *id_node;
    Node *params_node;
    const char *param_types_buffer[256];
    int param_count;

    if (!cls || !method_decl)
        return NULL;

    header = method_decl->child;
    return_type_node = header ? header->child : NULL;
    id_node = return_type_node ? return_type_node->sibling : NULL;
    params_node = id_node ? id_node->sibling : NULL;

    if (!id_node || !id_node->value)
        return NULL;

    param_count = collect_param_types(
        params_node,
        param_types_buffer,
        256);

    return find_method_by_signature(
        cls->methods,
        id_node->value,
        param_types_buffer,
        param_count);
}

static const char *annotate_call(Node *call, ClassEntry *cls, MethodEntry *method)
{
    Node *callee;
    Node *arg;
    const char **arg_types;
    int argc = 0;
    MethodEntry *m;
    MethodEntry *exact = NULL;
    MethodEntry *compatible = NULL;
    int compatible_count = 0;
    int i;

    if (!call)
        return "undef";

    callee = call->child;
    arg = callee ? callee->sibling : NULL;

    while (arg)
    {
        argc++;
        arg = arg->sibling;
    }

    arg_types = (const char **)malloc(sizeof(char *) * (argc > 0 ? argc : 1));
    if (!arg_types)
    {
        set_node_sem_type(call, "undef");
        if (callee)
            set_node_sem_type(callee, "undef");
        return call->sem_type;
    }

    arg = callee ? callee->sibling : NULL;
    i = 0;
    while (arg)
    {
        arg_types[i++] = annotate_expr(arg, cls, method);
        arg = arg->sibling;
    }

    if (cls && callee && callee->value)
    {
        for (m = cls->methods; m; m = m->next)
        {
            int ok = 1;
            if (strcmp(m->name, callee->value) != 0 || m->param_count != argc)
                continue;
            for (i = 0; i < argc; i++)
            {
                if (!type_equals(m->param_types[i], arg_types[i]))
                {
                    ok = 0;
                    break;
                }
            }
            if (ok)
            {
                exact = m;
                break;
            }
        }

        if (!exact)
        {
            for (m = cls->methods; m; m = m->next)
            {
                int ok = 1;
                if (strcmp(m->name, callee->value) != 0 || m->param_count != argc)
                    continue;
                for (i = 0; i < argc; i++)
                {
                    if (!is_type_assignable(m->param_types[i], arg_types[i]))
                    {
                        ok = 0;
                        break;
                    }
                }
                if (ok)
                {
                    compatible = m;
                    compatible_count++;
                }
            }
        }
    }

    if (exact || compatible_count == 1)
    {
        MethodEntry *selected = exact ? exact : compatible;
        char *sig = build_method_signature(selected);
        set_node_sem_type(call, selected->return_type);
        if (callee)
        {
            set_node_sem_type(callee, sig ? sig : "()");
        }
        if (sig)
            free(sig);
    }
    else
    {
        set_node_sem_type(call, "undef");
        if (callee)
            set_node_sem_type(callee, "undef");

        if (callee && callee->value)
        {
            char *sig = build_call_signature(callee->value, arg_types, argc);
            if (compatible_count > 1)
            {
                semantic_error_at(callee->line, callee->col, "Reference to method %s is ambiguous", sig ? sig : callee->value);
            }
            else
            {
                semantic_error_at(callee->line, callee->col, "Cannot find symbol %s", sig ? sig : callee->value);
            }
            if (sig)
                free(sig);
        }
    }

    free(arg_types);
    return call->sem_type;
}

static const char *annotate_binary(Node *expr, ClassEntry *cls, MethodEntry *method)
{
    Node *left = expr ? expr->child : NULL;
    Node *right = left ? left->sibling : NULL;
    const char *tl = annotate_expr(left, cls, method);
    const char *tr = annotate_expr(right, cls, method);
    const char *op = op_token_from_node(expr ? expr->type : NULL);

    if (!expr)
        return "undef";

    if (strcmp(expr->type, "Add") == 0 || strcmp(expr->type, "Sub") == 0 ||
        strcmp(expr->type, "Mul") == 0 || strcmp(expr->type, "Div") == 0 ||
        strcmp(expr->type, "Mod") == 0)
    {
        if (is_numeric(tl) && is_numeric(tr))
        {
            set_node_sem_type(expr, (is_double(tl) || is_double(tr)) ? "double" : "int");
        }
        else
        {
            operator_error_two_types(expr, op, tl, tr);
            set_node_sem_type(expr, "undef");
        }
        return expr->sem_type;
    }

    if (strcmp(expr->type, "And") == 0 || strcmp(expr->type, "Or") == 0)
    {
        if (is_boolean(tl) && is_boolean(tr))
        {
            set_node_sem_type(expr, "boolean");
        }
        else
        {
            operator_error_two_types(expr, op, tl, tr);
            set_node_sem_type(expr, "boolean");
        }
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Xor") == 0 ||
        strcmp(expr->type, "Lshift") == 0 ||
        strcmp(expr->type, "Rshift") == 0)
    {
        if (is_int(tl) && is_int(tr))
        {
            set_node_sem_type(expr, "int");
        }
        else
        {
            operator_error_two_types(expr, op, tl, tr);
            set_node_sem_type(expr, "int");
        }

        return expr->sem_type;
    }

    if (strcmp(expr->type, "Eq") == 0 || strcmp(expr->type, "Ne") == 0)
    {
        if ((is_numeric(tl) && is_numeric(tr)) || (is_boolean(tl) && is_boolean(tr)))
        {
            set_node_sem_type(expr, "boolean");
        }
        else
        {
            operator_error_two_types(expr, op, tl, tr);
            set_node_sem_type(expr, "boolean");
        }
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Lt") == 0 || strcmp(expr->type, "Le") == 0 ||
        strcmp(expr->type, "Gt") == 0 || strcmp(expr->type, "Ge") == 0)
    {
        if (is_numeric(tl) && is_numeric(tr))
        {
            set_node_sem_type(expr, "boolean");
        }
        else
        {
            operator_error_two_types(expr, op, tl, tr);
            set_node_sem_type(expr, "boolean");
        }
        return expr->sem_type;
    }

    set_node_sem_type(expr, "undef");
    return expr->sem_type;
}

static const char *annotate_unary(Node *expr, ClassEntry *cls, MethodEntry *method)
{
    Node *child = expr ? expr->child : NULL;
    const char *tc = annotate_expr(child, cls, method);
    const char *op = op_token_from_node(expr ? expr->type : NULL);

    if (!expr)
        return "undef";

    if (strcmp(expr->type, "Plus") == 0 || strcmp(expr->type, "Minus") == 0)
    {
        if (is_numeric(tc))
        {
            set_node_sem_type(expr, tc);
        }
        else
        {
            operator_error_one_type(expr, op, tc);
            set_node_sem_type(expr, "undef");
        }
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Not") == 0)
    {
        if (is_boolean(tc))
        {
            set_node_sem_type(expr, "boolean");
        }
        else
        {
            operator_error_one_type(expr, op, tc);
            set_node_sem_type(expr, "boolean");
        }
        return expr->sem_type;
    }

    set_node_sem_type(expr, "undef");
    return expr->sem_type;
}

static const char *annotate_expr(Node *expr, ClassEntry *cls, MethodEntry *method)
{
    Node *a;
    Node *b;
    const char *ta;
    const char *tb;

    if (!expr || !expr->type)
        return "undef";

    if (strcmp(expr->type, "StrLit") == 0)
    {
        set_node_sem_type(expr, "String");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Natural") == 0)
    {
        if (natural_out_of_bounds(expr->value))
        {
            semantic_error_at(expr->line, expr->col, "Number %s out of bounds", expr->value ? expr->value : "");
        }
        set_node_sem_type(expr, "int");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Decimal") == 0)
    {
        if (decimal_out_of_bounds(expr->value))
        {
            semantic_error_at(expr->line, expr->col, "Number %s out of bounds", expr->value ? expr->value : "");
        }
        set_node_sem_type(expr, "double");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "BoolLit") == 0)
    {
        set_node_sem_type(expr, "boolean");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Identifier") == 0)
    {
        if (is_reserved_name(expr->value)) 
        {
            semantic_error_at(expr->line, expr->col, "Symbol %s is reserved", expr->value);
            set_node_sem_type(expr, "undef");
            return expr->sem_type;
        }
        const char *resolved = lookup_symbol_type(cls, method, expr->value);
        if (!resolved)
        {
            semantic_error_at(expr->line, expr->col, "Cannot find symbol %s", expr->value ? expr->value : "");
            set_node_sem_type(expr, "undef");
        }
        else
        {
            set_node_sem_type(expr, resolved);
        }
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Length") == 0)
    {
        ta = annotate_expr(expr->child, cls, method);
        if (!type_equals(ta, "String[]"))
        {
            operator_error_one_type(expr, ".length", ta);
        }
        set_node_sem_type(expr, "int");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "ParseArgs") == 0)
    {
        a = expr->child;
        b = a ? a->sibling : NULL;

        ta = annotate_expr(a, cls, method);
        tb = annotate_expr(b, cls, method);

        if (!type_equals(ta, "String[]") || !is_int(tb))
        {
            operator_error_two_types(expr, "Integer.parseInt", ta, tb);
        }
        set_node_sem_type(expr, "int");
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Call") == 0)
    {
        return annotate_call(expr, cls, method);
    }

    if (strcmp(expr->type, "Assign") == 0)
    {
        a = expr->child;
        b = a ? a->sibling : NULL;

        ta = annotate_expr(a, cls, method);
        tb = annotate_expr(b, cls, method);

        if (!is_type_assignable(ta, tb))
        {
            operator_error_two_types(expr, "=", ta, tb);
        }

        set_node_sem_type(expr, ta);
        return expr->sem_type;
    }

    if (strcmp(expr->type, "Add") == 0 || strcmp(expr->type, "Sub") == 0 ||
        strcmp(expr->type, "Mul") == 0 || strcmp(expr->type, "Div") == 0 ||
        strcmp(expr->type, "Mod") == 0 || strcmp(expr->type, "And") == 0 ||
        strcmp(expr->type, "Or") == 0 || strcmp(expr->type, "Xor") == 0 ||
        strcmp(expr->type, "Lshift") == 0 || strcmp(expr->type, "Rshift") == 0 ||
        strcmp(expr->type, "Eq") == 0 || strcmp(expr->type, "Ne") == 0 ||
        strcmp(expr->type, "Lt") == 0 || strcmp(expr->type, "Le") == 0 ||
        strcmp(expr->type, "Gt") == 0 || strcmp(expr->type, "Ge") == 0)
    {
        return annotate_binary(expr, cls, method);
    }

    if (strcmp(expr->type, "Plus") == 0 || strcmp(expr->type, "Minus") == 0 ||
        strcmp(expr->type, "Not") == 0)
    {
        return annotate_unary(expr, cls, method);
    }

    set_node_sem_type(expr, "undef");
    return expr->sem_type;
}

static void annotate_statement(Node *stmt, ClassEntry *cls, MethodEntry *method)
{
    Node *child;

    if (!stmt || !stmt->type)
        return;

    if (strcmp(stmt->type, "Block") == 0)
    {
        child = stmt->child;
        while (child)
        {
            annotate_statement(child, cls, method);
            child = child->sibling;
        }
        return;
    }

    if (strcmp(stmt->type, "If") == 0)
    {
        Node *cond = stmt->child;
        Node *then_branch = cond ? cond->sibling : NULL;
        Node *else_branch = then_branch ? then_branch->sibling : NULL;
        const char *tc = annotate_expr(cond, cls, method);

        if (!is_boolean(tc))
        {
            semantic_error_at(cond ? cond->line : stmt->line, cond ? cond->col : stmt->col, "Incompatible type %s in if statement", tc);
        }

        annotate_statement(then_branch, cls, method);
        annotate_statement(else_branch, cls, method);
        return;
    }

    if (strcmp(stmt->type, "While") == 0)
    {
        Node *cond = stmt->child;
        const char *tc = annotate_expr(cond, cls, method);

        if (!is_boolean(tc))
        {
            semantic_error_at(cond->line, cond->col,
                              "Incompatible type %s in while statement", tc);
        }
        annotate_statement(cond->sibling, cls, method);
        return;
    }

    if (strcmp(stmt->type, "Return") == 0)
    {
        Node *expr = stmt->child;
        Symbol *ret_sym = method ? find_symbol(method->symbols, "return") : NULL;
        const char *ret_type = ret_sym ? ret_sym->type : "void";

        if (!expr)
        {
            if (!type_equals(ret_type, "void"))
            {
                semantic_error_at(stmt->line, stmt->col, "Incompatible type void in return statement");
            }
        }
        else
        {
            const char *te = annotate_expr(expr, cls, method);
            if (type_equals(ret_type, "void"))
            {
                semantic_error_at(expr->line, expr->col, "Incompatible type %s in return statement", te);
            }
            else if (!is_type_assignable(ret_type, te))
            {
                semantic_error_at(expr->line, expr->col, "Incompatible type %s in return statement", te);
            }
        }
        return;
    }

    if (strcmp(stmt->type, "Print") == 0)
    {
        Node *arg = stmt->child;
        if (!arg)
            return;

        const char *ta = annotate_expr(arg, cls, method);
        if (strcmp(arg->type, "StrLit") != 0)
        {
            if (!is_printable(ta))
            {
                semantic_error_at(arg->line, arg->col, "Incompatible type %s in System.out.print statement", ta);
            }
        }
        return;
    }

    if (strcmp(stmt->type, "Assign") == 0 || strcmp(stmt->type, "Call") == 0 ||
        strcmp(stmt->type, "ParseArgs") == 0)
    {
        annotate_expr(stmt, cls, method);
    }
}

static void annotate_method_body(Node *method_body, ClassEntry *cls, MethodEntry *method)
{
    Node *item;

    if (!method_body)
        return;

    item = method_body->child;
    while (item)
    {
        if (strcmp(item->type, "VarDecl") == 0)
        {
            Node *type_node = item->child;
            Node *id_node = type_node ? type_node->sibling : NULL;

            if (id_node && id_node->value)
            {
                add_unique_symbol(&method->symbols, id_node, type_from_type_node(type_node), 0);
            }
        }
        else
        {
            annotate_statement(item, cls, method);
        }
        item = item->sibling;
    }
}

static void annotate_program(Node *root, ClassEntry *cls)
{
    Node *member;
    Node *class_id;

    if (!root || !cls || strcmp(root->type, "Program") != 0)
        return;

    class_id = root->child;
    member = class_id ? class_id->sibling : NULL;

    while (member)
    {
        if (strcmp(member->type, "MethodDecl") == 0)
        {
            Node *header = member->child;
            Node *body = header ? header->sibling : NULL;
            MethodEntry *method = find_method_entry_for_decl(cls, member);

            if (method && method->ast_node == member)
            {
                annotate_method_body(body, cls, method);
            }
        }
        member = member->sibling;
    }
}

void free_symbol_tables(ClassEntry *class_entry)
{
    Symbol *f;
    MethodEntry *m;
    GlobalMember *gm;

    if (!class_entry)
        return;

    while (class_entry->fields)
    {
        f = class_entry->fields;
        class_entry->fields = f->next;
        free(f->name);
        free(f->type);
        free(f);
    }

    while (class_entry->methods)
    {
        int i;
        Symbol *s;

        m = class_entry->methods;
        class_entry->methods = m->next;

        if (m->param_types)
        {
            for (i = 0; i < m->param_count; i++)
            {
                free(m->param_types[i]);
            }
            free(m->param_types);
        }

        while (m->symbols)
        {
            s = m->symbols;
            m->symbols = s->next;
            free(s->name);
            free(s->type);
            free(s);
        }

        free(m->name);
        free(m->return_type);
        free(m);
    }

    gm = class_entry->members;
    while (gm)
    {
        GlobalMember *next = gm->next;
        free(gm);
        gm = next;
    }

    free(class_entry->name);
    free(class_entry);
}

int run_semantic_phase(Node *root, int print_tables)
{
    ClassEntry *class_entry;

    semantic_errors = 0;
    class_entry = build_symbol_tables(root);

    if (class_entry)
    {
        annotate_program(root, class_entry);

        if (print_tables)
        {
            print_symbol_tables(class_entry);
        }

        free_symbol_tables(class_entry);
    }

    return semantic_errors;
}
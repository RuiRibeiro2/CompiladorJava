#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

static int semantic_errors = 0;

static char *xstrdup(const char *s) {
    if (!s) return NULL;
    char *d = (char *)malloc(strlen(s) + 1);
    if (!d) return NULL;
    strcpy(d, s);
    return d;
}

static int is_reserved_name(const char *name) {
    return name && strcmp(name, "_") == 0;
}

static const char *type_from_type_node(const Node *type_node) {
    if (!type_node || !type_node->type) return "undef";
    if (strcmp(type_node->type, "Int") == 0) return "int";
    if (strcmp(type_node->type, "Double") == 0) return "double";
    if (strcmp(type_node->type, "Bool") == 0) return "boolean";
    if (strcmp(type_node->type, "Void") == 0) return "void";
    if (strcmp(type_node->type, "StringArray") == 0) return "String[]";
    return "undef";
}

static Symbol *find_symbol(Symbol *head, const char *name) {
    Symbol *it = head;
    while (it) {
        if (strcmp(it->name, name) == 0) return it;
        it = it->next;
    }
    return NULL;
}

static int same_signature(const MethodEntry *m, const char *name, const char **param_types, int param_count) {
    int i;
    if (strcmp(m->name, name) != 0 || m->param_count != param_count) return 0;
    for (i = 0; i < param_count; i++) {
        if (strcmp(m->param_types[i], param_types[i]) != 0) return 0;
    }
    return 1;
}

static MethodEntry *find_method_by_signature(MethodEntry *head, const char *name, const char **param_types, int param_count) {
    MethodEntry *it = head;
    while (it) {
        if (same_signature(it, name, param_types, param_count)) return it;
        it = it->next;
    }
    return NULL;
}

static Symbol *append_symbol(Symbol **head, const char *name, const char *type, int is_param) {
    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    Symbol *tail;
    if (!sym) return NULL;
    sym->name = xstrdup(name);
    sym->type = xstrdup(type);
    sym->is_param = is_param;
    sym->next = NULL;

    if (!*head) {
        *head = sym;
        return sym;
    }

    tail = *head;
    while (tail->next) tail = tail->next;
    tail->next = sym;
    return sym;
}

static MethodEntry *append_method(MethodEntry **head, const char *name, const char *return_type, const char **param_types, int param_count) {
    MethodEntry *m = (MethodEntry *)malloc(sizeof(MethodEntry));
    MethodEntry *tail;
    int i;
    if (!m) return NULL;

    m->name = xstrdup(name);
    m->return_type = xstrdup(return_type);
    m->param_count = param_count;
    m->symbols = NULL;
    m->next = NULL;

    if (param_count > 0) {
        m->param_types = (char **)malloc(sizeof(char *) * param_count);
        for (i = 0; i < param_count; i++) m->param_types[i] = xstrdup(param_types[i]);
    } else {
        m->param_types = NULL;
    }

    if (!*head) {
        *head = m;
        return m;
    }

    tail = *head;
    while (tail->next) tail = tail->next;
    tail->next = m;
    return m;
}

static int add_unique_symbol(Symbol **table, const char *name, const char *type, int is_param) {
    if (is_reserved_name(name)) {
        printf("Symbol _ is reserved\n");
        semantic_errors++;
        return 0;
    }

    if (find_symbol(*table, name)) {
        printf("Symbol %s already defined\n", name);
        semantic_errors++;
        return 0;
    }

    append_symbol(table, name, type, is_param);
    return 1;
}

static void collect_field_decl(ClassEntry *cls, Node *field_decl) {
    Node *type_node;
    Node *id_node;
    const char *type_name;
    if (!cls || !field_decl) return;

    type_node = field_decl->child;
    id_node = type_node ? type_node->sibling : NULL;
    if (!id_node || !id_node->value) return;

    type_name = type_from_type_node(type_node);
    add_unique_symbol(&cls->fields, id_node->value, type_name, 0);
}

static int collect_param_types(Node *params_node, const char **param_types, int max_params) {
    int count = 0;
    Node *pd;
    if (!params_node) return 0;

    pd = params_node->child;
    while (pd && count < max_params) {
        Node *type_node = pd->child;
        param_types[count++] = type_from_type_node(type_node);
        pd = pd->sibling;
    }

    return count;
}

static void collect_method_locals(MethodEntry *method, Node *method_body) {
    Node *item;
    if (!method || !method_body) return;

    item = method_body->child;
    while (item) {
        if (strcmp(item->type, "VarDecl") == 0) {
            Node *type_node = item->child;
            Node *id_node = type_node ? type_node->sibling : NULL;
            if (id_node && id_node->value) {
                add_unique_symbol(&method->symbols, id_node->value, type_from_type_node(type_node), 0);
            }
        }
        item = item->sibling;
    }
}

static void collect_method_decl(ClassEntry *cls, Node *method_decl) {
    Node *header;
    Node *body;
    Node *return_type_node;
    Node *id_node;
    Node *params_node;
    const char *param_types_buffer[256];
    int param_count;
    const char *return_type;
    MethodEntry *method;
    Node *pd;

    if (!cls || !method_decl) return;

    header = method_decl->child;
    body = header ? header->sibling : NULL;
    return_type_node = header ? header->child : NULL;
    id_node = return_type_node ? return_type_node->sibling : NULL;
    params_node = id_node ? id_node->sibling : NULL;

    if (!id_node || !id_node->value) return;

    if (is_reserved_name(id_node->value)) {
        printf("Symbol _ is reserved\n");
        semantic_errors++;
        return;
    }

    param_count = collect_param_types(params_node, param_types_buffer, 256);
    if (find_method_by_signature(cls->methods, id_node->value, param_types_buffer, param_count)) {
        printf("Symbol %s already defined\n", id_node->value);
        semantic_errors++;
        return;
    }

    return_type = type_from_type_node(return_type_node);
    method = append_method(&cls->methods, id_node->value, return_type, param_types_buffer, param_count);
    if (!method) return;

    append_symbol(&method->symbols, "return", return_type, 0);

    pd = params_node ? params_node->child : NULL;
    while (pd) {
        Node *type_node = pd->child;
        Node *param_id = type_node ? type_node->sibling : NULL;
        if (param_id && param_id->value) {
            add_unique_symbol(&method->symbols, param_id->value, type_from_type_node(type_node), 1);
        }
        pd = pd->sibling;
    }

    collect_method_locals(method, body);
}

static ClassEntry *build_symbol_tables(Node *root) {
    ClassEntry *cls;
    Node *class_id;
    Node *member;

    if (!root || strcmp(root->type, "Program") != 0) return NULL;

    cls = (ClassEntry *)malloc(sizeof(ClassEntry));
    if (!cls) return NULL;
    cls->fields = NULL;
    cls->methods = NULL;

    class_id = root->child;
    cls->name = xstrdup(class_id && class_id->value ? class_id->value : "<anonymous>");

    member = class_id ? class_id->sibling : NULL;
    while (member) {
        if (strcmp(member->type, "FieldDecl") == 0) {
            collect_field_decl(cls, member);
        } else if (strcmp(member->type, "MethodDecl") == 0) {
            collect_method_decl(cls, member);
        }
        member = member->sibling;
    }

    return cls;
}

static void print_param_list(const MethodEntry *m) {
    int i;
    printf("(");
    for (i = 0; i < m->param_count; i++) {
        printf("%s", m->param_types[i]);
        if (i + 1 < m->param_count) printf(",");
    }
    printf(")");
}

static void print_symbol_tables(const ClassEntry *cls) {
    const Symbol *field;
    const MethodEntry *method;
    const Symbol *sym;

    if (!cls) return;

    printf("===== Class %s Symbol Table =====\n", cls->name);

    field = cls->fields;
    while (field) {
        printf("%s\t%s\n", field->name, field->type);
        field = field->next;
    }

    method = cls->methods;
    while (method) {
        printf("%s\t", method->name);
        print_param_list(method);
        printf("\t%s\n", method->return_type);
        method = method->next;
    }

    printf("\n");

    method = cls->methods;
    while (method) {
        printf("===== Method %s", method->name);
        print_param_list(method);
        printf(" Symbol Table =====\n");

        sym = method->symbols;
        while (sym) {
            if (sym->is_param) {
                printf("%s\t\t%s\tparam\n", sym->name, sym->type);
            } else {
                printf("%s\t\t%s\n", sym->name, sym->type);
            }
            sym = sym->next;
        }

        printf("\n");
        method = method->next;
    }
}

void free_symbol_tables(ClassEntry *class_entry) {
    Symbol *f;
    MethodEntry *m;

    if (!class_entry) return;

    while (class_entry->fields) {
        f = class_entry->fields;
        class_entry->fields = f->next;
        free(f->name);
        free(f->type);
        free(f);
    }

    while (class_entry->methods) {
        int i;
        Symbol *s;
        m = class_entry->methods;
        class_entry->methods = m->next;

        for (i = 0; i < m->param_count; i++) free(m->param_types[i]);
        free(m->param_types);

        while (m->symbols) {
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

    free(class_entry->name);
    free(class_entry);
}

int run_semantic_phase(Node *root, int print_tables) {
    ClassEntry *class_entry;

    semantic_errors = 0;
    class_entry = build_symbol_tables(root);

    if (print_tables && class_entry) {
        print_symbol_tables(class_entry);
    }

    free_symbol_tables(class_entry);
    return semantic_errors;
}

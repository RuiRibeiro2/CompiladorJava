#ifndef SEMANTIC_H
#define SEMANTIC_H

typedef struct node {
    char *type;
    char *value;
    char *sem_type;
    int line;
    int col;
    struct node *child;
    struct node *sibling;
} Node;

typedef struct symbol {
    char *name;
    char *type;
    int is_param;
    struct symbol *next;
} Symbol;

typedef struct method_entry {
    char *name;
    char *return_type;
    char **param_types;
    int param_count;
    Symbol *symbols;
    struct method_entry *next;
} MethodEntry;

typedef struct class_entry {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
} ClassEntry;

int run_semantic_phase(Node *root, int print_tables);
void free_symbol_tables(ClassEntry *class_entry);

#endif

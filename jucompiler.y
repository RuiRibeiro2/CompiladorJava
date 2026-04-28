%{
    /* ---
    Disciplina de Compiladores
    ---
    Trabalho realizado por

    Leonardo Duarte
    2023213089

    Rui Ribeiro
    202118947

    --- */
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "semantic.h"

    extern int yylex(void);
    extern char *yytext;
    extern int row;
    extern int column;
    extern int flag_t;
    extern int flag_s;

    void yyerror(const char *s);

    static Node *new_node(const char *type, const char *value) {
        Node *n = (Node *)malloc(sizeof(Node));
        n->type = strdup(type);
        n->value = value ? strdup(value) : NULL;
        n->sem_type = NULL;
        n->line = 0;
        n->col = 0;
        n->child = NULL;
        n->sibling = NULL;
        return n;
    }

    static void free_ast(Node *n) {
        if (!n) return;
        free_ast(n->child);
        free_ast(n->sibling);
        free(n->type);
        if (n->value) free(n->value);
        if (n->sem_type) free(n->sem_type);
        free(n);
    }

    static Node *append_sibling(Node *a, Node *b) {
        Node *t;
        if (!a) return b;
        t = a;
        while (t->sibling) t = t->sibling;
        t->sibling = b;
        return a;
    }

    static void add_child(Node *parent, Node *child) {
        if (!parent || !child) return;
        if (!parent->child) parent->child = child;
        else append_sibling(parent->child, child);
    }

    static Node *make_binary(const char *type, Node *a, Node *b) {
        Node *n = new_node(type, NULL);
        add_child(n, a);
        add_child(n, b);
        return n;
    }

    static Node *make_unary(const char *type, Node *a) {
        Node *n = new_node(type, NULL);
        add_child(n, a);
        return n;
    }

    static Node *make_type_node(Node *t) {
        if (!t) return NULL;
        return new_node(t->type, t->value);
    }

    static void set_node_loc(Node *n, int line, int col) {
        if (!n) return;
        n->line = line;
        n->col = col;
    }

    static Node *make_leaf_at(const char *type, const char *value, int line, int col) {
        Node *n = new_node(type, value);
        set_node_loc(n, line, col);
        return n;
    }

    static Node *make_identifier_at(const char *name, int line, int col) {
        return make_leaf_at("Identifier", name, line, col);
    }

    static Node *make_binary_at(const char *type, Node *a, Node *b, int line, int col) {
        Node *n = make_binary(type, a, b);
        set_node_loc(n, line, col);
        return n;
    }

    static Node *make_unary_at(const char *type, Node *a, int line, int col) {
        Node *n = make_unary(type, a);
        set_node_loc(n, line, col);
        return n;
    }

    static int is_expression_node(const char *type) {
        return strcmp(type, "Add") == 0 || strcmp(type, "Sub") == 0 ||
               strcmp(type, "Mul") == 0 || strcmp(type, "Div") == 0 ||
               strcmp(type, "Mod") == 0 || strcmp(type, "And") == 0 ||
               strcmp(type, "Or") == 0 || strcmp(type, "Xor") == 0 ||
               strcmp(type, "Lshift") == 0 || strcmp(type, "Rshift") == 0 ||
               strcmp(type, "Eq") == 0 || strcmp(type, "Ge") == 0 ||
               strcmp(type, "Gt") == 0 || strcmp(type, "Le") == 0 ||
               strcmp(type, "Lt") == 0 || strcmp(type, "Ne") == 0 ||
               strcmp(type, "Minus") == 0 || strcmp(type, "Plus") == 0 ||
               strcmp(type, "Not") == 0 || strcmp(type, "Identifier") == 0 ||
               strcmp(type, "Natural") == 0 || strcmp(type, "Decimal") == 0 ||
               strcmp(type, "BoolLit") == 0 || strcmp(type, "Length") == 0 ||
               strcmp(type, "Assign") == 0 || strcmp(type, "Call") == 0 ||
               strcmp(type, "ParseArgs") == 0 || strcmp(type, "StrLit") == 0;
    }

    static void print_ast(Node *n, int depth) {
        int i;
        if (!n) return;
        for (i = 0; i < depth; i++) printf(".");
        if (n->value) {
            if (n->sem_type && is_expression_node(n->type)) {
                printf("%s(%s) - %s\n", n->type, n->value, n->sem_type);
            } else {
                printf("%s(%s)\n", n->type, n->value);
            }
        } else {
            if (n->sem_type && is_expression_node(n->type)) {
                printf("%s - %s\n", n->type, n->sem_type);
            } else {
                printf("%s\n", n->type);
            }
        }
        for (n = n->child; n; n = n->sibling) print_ast(n, depth + 2);
    }

    static int syntax_errors = 0;
    static int semantic_errors = 0;
    Node *ast_root = NULL;
%}

%code requires {
    #include "semantic.h"
}


%union {
    char *lexeme;
    Node *node;
}

%destructor { free($$); } <lexeme>

%token <lexeme> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT RESERVED
%token BOOL INT DOUBLE STRING VOID CLASS PUBLIC STATIC RETURN IF ELSE WHILE
%token PRINT PARSEINT DOTLENGTH
%token ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR
%token PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR AND
%locations

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT LE GT GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%right UPLUS UMINUS

%type <node> Program ProgramMembers ProgramMember MethodDecl FieldDecl FieldDeclTail
%type <node> Type MethodHeader MethodBody MethodBodyItems MethodBodyItem
%type <node> FormalParamsOpt FormalParams FormalParamsTail
%type <node> VarDecl VarDeclTail
%type <node> Statement MatchedStatement UnmatchedStatement StatementList ExprOpt PrintArg
%type <node> MethodInvocation ExprListOpt ExprList Assignment ParseArgs ExprOrAssign Expr


%start Program

%%

Program
    : CLASS IDENTIFIER LBRACE ProgramMembers RBRACE {
        Node *n = new_node("Program", NULL);
        add_child(n, make_identifier_at($2, @2.first_line, @2.first_column));
        free($2);
        add_child(n, $4);
        ast_root = n;
    }
    ;

ProgramMembers
    : ProgramMembers ProgramMember { $$ = append_sibling($1, $2); }
    | /* empty */ { $$ = NULL; }
    ;

ProgramMember
    : MethodDecl { $$ = $1; }
    | FieldDecl { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    ;

MethodDecl
    : PUBLIC STATIC MethodHeader MethodBody {
        Node *n = new_node("MethodDecl", NULL);
        add_child(n, $3);
        add_child(n, $4);
        $$ = n;
    }
    ;

FieldDecl
    : PUBLIC STATIC Type IDENTIFIER FieldDeclTail SEMICOLON {
        Node *head = NULL;
        Node *type = $3;
        Node *id = make_identifier_at($4, @4.first_line, @4.first_column);
        Node *fie_decl = new_node("FieldDecl", NULL);
        add_child(fie_decl, make_type_node(type));
        add_child(fie_decl, id);
        head = append_sibling(head, fie_decl);
        Node *cur = $5;
        while (cur) {
            Node * next = cur->sibling;
            cur->sibling = NULL;

            Node *fie_decl2 = new_node("FieldDecl", NULL);
            add_child(fie_decl2, make_type_node(type));
            add_child(fie_decl2, cur);
            head = append_sibling(head, fie_decl2);

            cur = next;
        }
        free_ast($3);
        free($4);
        $$ = head;
    }
    | error SEMICOLON { $$ = NULL; }
    ;

FieldDeclTail
    : FieldDeclTail COMMA IDENTIFIER {
        Node *id = make_identifier_at($3, @3.first_line, @3.first_column);
        free($3);
        $$ = append_sibling($1, id);
    }
    | /* empty */ { $$ = NULL; }
    ;

Type
    : BOOL { $$ = new_node("Bool", NULL); }
    | INT { $$ = new_node("Int", NULL); }
    | DOUBLE { $$ = new_node("Double", NULL); }
    ;


MethodHeader
    : Type IDENTIFIER LPAR FormalParamsOpt RPAR {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, $1);
        add_child(n, make_identifier_at($2, @2.first_line, @2.first_column));
        free($2);
        add_child(n, $4);
        $$ = n;
    }
    | VOID IDENTIFIER LPAR FormalParamsOpt RPAR {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, new_node("Void", NULL));
        add_child(n, make_identifier_at($2, @2.first_line, @2.first_column));
        add_child(n, $4);
        free($2);
        $$ = n;
    }
    ;

FormalParamsOpt
    : FormalParams { $$ = $1; }
    | /* empty */ { $$ = new_node("MethodParams", NULL); }
    ;

FormalParams
    : STRING LSQ RSQ IDENTIFIER {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, new_node("StringArray", NULL));
        add_child(pd, make_identifier_at($4, @4.first_line, @4.first_column));
        free($4);
        add_child(n, pd);
        $$ = n;
    }
    | Type IDENTIFIER FormalParamsTail {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, $1);
        add_child(pd, make_identifier_at($2, @2.first_line, @2.first_column));
        free($2);
        add_child(n, pd);
        add_child(n, $3);
        $$ = n;
    }
    ;

FormalParamsTail
    : FormalParamsTail COMMA Type IDENTIFIER {
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, $3);
        add_child(pd, make_identifier_at($4, @4.first_line, @4.first_column));
        free($4);
        $$ = append_sibling($1, pd);
    }
    | /* empty */ { $$ = NULL; }
    ;

MethodBody
    : LBRACE MethodBodyItems RBRACE {
        Node *n = new_node("MethodBody", NULL);
        add_child(n, $2);
        $$ = n;
    }
    ;

MethodBodyItems
    : MethodBodyItems MethodBodyItem { $$ = append_sibling($1, $2); }
    | /* empty */ { $$ = NULL; }
    ;

MethodBodyItem
    : VarDecl { $$ = $1; }
    | Statement { $$ = $1; }
    ;

VarDecl
    : Type IDENTIFIER VarDeclTail SEMICOLON {
        Node *head = NULL;
        Node *type = $1;
        Node *id = make_identifier_at($2, @2.first_line, @2.first_column);
        Node *var_decl = new_node("VarDecl", NULL);
        add_child(var_decl, make_type_node(type));
        add_child(var_decl, id);
        head = append_sibling(head, var_decl);
        Node *cur = $3;
        while (cur) {
            Node *next = cur->sibling;
            cur->sibling = NULL;

            Node *var_decl2 = new_node("VarDecl", NULL);
            add_child(var_decl2, make_type_node(type));
            add_child(var_decl2, cur);
            head = append_sibling(head, var_decl2);

            cur = next;
        }
        free_ast($1);
        free($2);
        $$ = head;
    }
    ;

VarDeclTail
    : VarDeclTail COMMA IDENTIFIER {
        Node *id = make_identifier_at($3, @3.first_line, @3.first_column);
        $$ = append_sibling($1, id);
        free($3);
    }
    | /* empty */ { $$ = NULL; }
    ;

Statement
    : MatchedStatement { $$ = $1; }
    | UnmatchedStatement { $$ = $1; }
    ;

MatchedStatement
    : LBRACE StatementList RBRACE {
        if (!$2) $$ = NULL;
        else if (!$2->sibling) $$ = $2;
        else {
            Node *b = new_node("Block", NULL);
            add_child(b, $2);
            $$ = b;
        }
    }
    | IF LPAR ExprOrAssign RPAR MatchedStatement ELSE MatchedStatement {
        Node *n = new_node("If", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);
        
        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        if ($7) add_child(n, $7);
        else add_child(n, new_node("Block", NULL));
        $$ = n;
    }
    | WHILE LPAR ExprOrAssign RPAR MatchedStatement {
        Node *n = new_node("While", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);

        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));

        $$ = n;
    }
    | RETURN ExprOpt SEMICOLON {
        Node *n = new_node("Return", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        if ($2) add_child(n, $2);
        $$ = n;
    }
    | MethodInvocation SEMICOLON { $$ = $1; }
    | Assignment SEMICOLON { $$ = $1; }
    | ParseArgs SEMICOLON { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    | PRINT LPAR PrintArg RPAR SEMICOLON {
        Node *n = new_node("Print", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);
        $$ = n;
    }
    | error SEMICOLON { $$ = NULL; }
    ;

UnmatchedStatement
    : IF LPAR ExprOrAssign RPAR Statement {
        Node *n = new_node("If", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);

        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        add_child(n, new_node("Block", NULL)); // Empty Else block
        $$ = n;
    }
    | IF LPAR ExprOrAssign RPAR MatchedStatement ELSE UnmatchedStatement {
        Node *n = new_node("If", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);
        
        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        if ($7) add_child(n, $7);
        else add_child(n, new_node("Block", NULL));
        
        $$ = n;
    }
    | WHILE LPAR ExprOrAssign RPAR UnmatchedStatement {
        Node *n = new_node("While", NULL);
        set_node_loc(n, @1.first_line, @1.first_column);
        add_child(n, $3);

        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));

        $$ = n;
    }
    ;

StatementList
    : StatementList Statement { $$ = append_sibling($1, $2); }
    | /* empty */ { $$ = NULL; }
    ;


ExprOpt
    : ExprOrAssign { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

PrintArg
    : ExprOrAssign { $$ = $1; }
    | STRLIT { $$ = new_node("StrLit", $1); free($1); }
    ;

MethodInvocation
    : IDENTIFIER LPAR ExprListOpt RPAR {
        Node *n = new_node("Call", NULL);
        add_child(n, make_identifier_at($1, @1.first_line, @1.first_column));
        set_node_loc(n, @1.first_line, @1.first_column);
        free($1);
        add_child(n, $3);
        $$ = n;
    }
    | IDENTIFIER LPAR error RPAR { free($1); $$ = NULL; }
    ;

ExprListOpt
    : ExprList { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

ExprList
    : ExprOrAssign { $$ = $1; }
    | ExprList COMMA ExprOrAssign { $$ = append_sibling($1, $3); }
    ;


Assignment
    : IDENTIFIER ASSIGN ExprOrAssign {
        Node *n = new_node("Assign", NULL);
        add_child(n, make_identifier_at($1, @1.first_line, @1.first_column));
        set_node_loc(n, @2.first_line, @2.first_column);
        free($1);
        add_child(n, $3);
        $$ = n;
    }
    ;

ParseArgs
    : PARSEINT LPAR IDENTIFIER LSQ ExprOrAssign RSQ RPAR {
        Node *n = new_node("ParseArgs", NULL);
        add_child(n, make_identifier_at($3, @3.first_line, @3.first_column));
        set_node_loc(n, @1.first_line, @1.first_column);
        free($3);
        add_child(n, $5);
        $$ = n;
    }
    | PARSEINT LPAR error RPAR { $$ = NULL; }
    ;

ExprOrAssign
    : Assignment { $$ = $1; }
    | Expr { $$ = $1; }
    ;

Expr
    : Expr PLUS Expr { $$ = make_binary_at("Add", $1, $3, @2.first_line, @2.first_column); }
    | Expr MINUS Expr { $$ = make_binary_at("Sub", $1, $3, @2.first_line, @2.first_column); }
    | Expr STAR Expr { $$ = make_binary_at("Mul", $1, $3, @2.first_line, @2.first_column); }
    | Expr DIV Expr { $$ = make_binary_at("Div", $1, $3, @2.first_line, @2.first_column); }
    | Expr MOD Expr { $$ = make_binary_at("Mod", $1, $3, @2.first_line, @2.first_column); }
    | Expr AND Expr { $$ = make_binary_at("And", $1, $3, @2.first_line, @2.first_column); }
    | Expr OR Expr { $$ = make_binary_at("Or", $1, $3, @2.first_line, @2.first_column); }
    | Expr XOR Expr { $$ = make_binary_at("Xor", $1, $3, @2.first_line, @2.first_column); }
    | Expr LSHIFT Expr { $$ = make_binary_at("Lshift", $1, $3, @2.first_line, @2.first_column); }
    | Expr RSHIFT Expr { $$ = make_binary_at("Rshift", $1, $3, @2.first_line, @2.first_column); }
    | Expr EQ Expr { $$ = make_binary_at("Eq", $1, $3, @2.first_line, @2.first_column); }
    | Expr GE Expr { $$ = make_binary_at("Ge", $1, $3, @2.first_line, @2.first_column); }
    | Expr GT Expr { $$ = make_binary_at("Gt", $1, $3, @2.first_line, @2.first_column); }
    | Expr LE Expr { $$ = make_binary_at("Le", $1, $3, @2.first_line, @2.first_column); }
    | Expr LT Expr { $$ = make_binary_at("Lt", $1, $3, @2.first_line, @2.first_column); }
    | Expr NE Expr { $$ = make_binary_at("Ne", $1, $3, @2.first_line, @2.first_column); }
    | MINUS Expr %prec UMINUS { $$ = make_unary_at("Minus", $2, @1.first_line, @1.first_column); }
    | PLUS Expr %prec UPLUS { $$ = make_unary_at("Plus", $2, @1.first_line, @1.first_column); }
    | NOT Expr { $$ = make_unary_at("Not", $2, @1.first_line, @1.first_column); }
    | LPAR ExprOrAssign RPAR { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | MethodInvocation { $$ = $1; }
    | ParseArgs { $$ = $1; }
    | IDENTIFIER DOTLENGTH {
        Node *n = new_node("Length", NULL);
        add_child(n, make_identifier_at($1, @1.first_line, @1.first_column));
        set_node_loc(n, @2.first_line, @2.first_column);
        $$ = n;
        free($1);
    }
    | IDENTIFIER { $$ = make_leaf_at("Identifier", $1, @1.first_line, @1.first_column); free($1); }
    | NATURAL { $$ = make_leaf_at("Natural", $1, @1.first_line, @1.first_column); free($1); }
    | DECIMAL { $$ = make_leaf_at("Decimal", $1, @1.first_line, @1.first_column); free($1); }
    | BOOLLIT { $$ = make_leaf_at("BoolLit", $1, @1.first_line, @1.first_column); free($1); }
    ;

%%

void yyerror(const char *s) {
    syntax_errors++;
    printf("Line %d, col %d: %s: %s\n", row, column - (int)strlen(yytext) + 1, s, yytext);
    // printf("Line %d, col %d: %s: %s\n", row, column, s, yytext);
}

void print_final_ast() {
    if (flag_t && syntax_errors == 0) {
        print_ast(ast_root, 0);
    }
}

int get_syntax_errors() {
    return syntax_errors;
}

int run_semantic(int print_tables) {
    if (syntax_errors != 0 || !ast_root) {
        semantic_errors = 0;
        return 0;
    }
    semantic_errors = run_semantic_phase(ast_root, print_tables);
    return semantic_errors;
}

void free_final_ast() {
    free_ast(ast_root);
    ast_root = NULL;
}
    
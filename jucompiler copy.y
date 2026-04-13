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

    extern int yylex(void);
    extern char *yytext;
    extern int row;
    extern int column;
    extern int flag_t;
    extern int flag_e2;

    void yyerror(const char *s);

    typedef struct node {
        char *type;
        char *value;
        struct node *child;
        struct node *sibling;
    } Node;

    static Node *new_node(const char *type, const char *value) {
        Node *n = (Node *)malloc(sizeof(Node));
        n->type = strdup(type);
        n->value = value ? strdup(value) : NULL;
        n->child = NULL;
        n->sibling = NULL;
        return n;
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

    static void print_ast(Node *n, int depth) {
        int i;
        if (!n) return;
        for (i = 0; i < depth; i++) printf(".");
        if (n->value)
            printf("%s(%s)\n", n->type, n->value);
        else
            printf("%s\n", n->type);
        for (n = n->child; n; n = n->sibling) print_ast(n, depth + 2);
    }

    static int syntax_errors = 0;
    Node *ast_root = NULL;
%}

%code requires {
    typedef struct node Node;
}


%union {
    char *lexeme;
    Node *node;
}

%token <lexeme> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT RESERVED
%token BOOL INT DOUBLE STRING VOID CLASS PUBLIC STATIC RETURN IF ELSE WHILE
%token PRINT PARSEINT DOTLENGTH
%token ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ LT MINUS MOD NE NOT OR
%token PLUS RBRACE RPAR RSQ SEMICOLON ARROW LSHIFT RSHIFT XOR AND

%left OR
%left XOR
%left AND
%left EQ NE
%left LT LE GT GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%right UPLUS UMINUS

%type <node> Program ProgramMembers ProgramMember MethodDecl FieldDecl FieldDeclTail
%type <node> Type ParamType MethodHeader MethodBody MethodBodyItems MethodBodyItem
%type <node> FormalParamsOpt FormalParams FormalParamsTail
%type <node> VarDecl VarDeclTail
%type <node> Statement MatchedStatement UnmatchedStatement StatementList ExprOpt PrintArg
%type <node> MethodInvocation ExprListOpt ExprList Assignment ParseArgs Expr ExprOrAssign


%start Program

%%

Program
    : CLASS IDENTIFIER LBRACE ProgramMembers RBRACE {
        Node *n = new_node("Program", NULL);
        add_child(n, new_node("Identifier", $2));
        add_child(n, $4);
        $$ = n;
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
        Node *t = $3;
        Node *id = new_node("Identifier", $4);
        Node *fd = new_node("FieldDecl", NULL);
        add_child(fd, make_type_node(t));
        add_child(fd, id);
        head = append_sibling(head, fd);
        Node *cur = $5;
        while (cur) {
            Node * next = cur->sibling;
            cur->sibling = NULL;

            Node *fd2 = new_node("FieldDecl", NULL);
            add_child(fd2, make_type_node(t));
            add_child(fd2, cur);
            head = append_sibling(head, fd2);

            cur = next;
        }
        $$ = head;
    }
    | error SEMICOLON { $$ = NULL; }
    ;

FieldDeclTail
    : FieldDeclTail COMMA IDENTIFIER {
        Node *id = new_node("Identifier", $3);
        $$ = append_sibling($1, id);
    }
    | /* empty */ { $$ = NULL; }
    ;

Type
    : BOOL { $$ = new_node("Bool", NULL); }
    | INT { $$ = new_node("Int", NULL); }
    | DOUBLE { $$ = new_node("Double", NULL); }
    ;

ParamType
    : Type { $$ = $1; }
    | STRING LSQ RSQ { $$ = new_node("StringArray", NULL); }
    ;

MethodHeader
    : Type IDENTIFIER LPAR FormalParamsOpt RPAR {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, $1);
        add_child(n, new_node("Identifier", $2));
        add_child(n, $4);
        $$ = n;
    }
    | VOID IDENTIFIER LPAR FormalParamsOpt RPAR {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, new_node("Void", NULL));
        add_child(n, new_node("Identifier", $2));
        add_child(n, $4);
        $$ = n;
    }
    ;

FormalParamsOpt
    : FormalParams { $$ = $1; }
    | /* empty */ { $$ = new_node("MethodParams", NULL); }
    ;

FormalParams
    : ParamType IDENTIFIER FormalParamsTail {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, $1);
        add_child(pd, new_node("Identifier", $2));
        add_child(n, pd);
        add_child(n, $3);
        $$ = n;
    }
    ;

FormalParamsTail
    : FormalParamsTail COMMA ParamType IDENTIFIER {
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, $3);
        add_child(pd, new_node("Identifier", $4));
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
        Node *id = new_node("Identifier", $2);
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
        $$ = head;
    }
    ;

VarDeclTail
    : VarDeclTail COMMA IDENTIFIER {
        Node *id = new_node("Identifier", $3);
        $$ = append_sibling($1, id);
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
        add_child(n, $3);
        
        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        if ($7) add_child(n, $7);
        else add_child(n, new_node("Block", NULL));
        $$ = n;
    }
    | WHILE LPAR ExprOrAssign RPAR MatchedStatement {
        Node *n = new_node("While", NULL);
        add_child(n, $3);

        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));

        $$ = n;
    }
    | RETURN ExprOpt SEMICOLON {
        Node *n = new_node("Return", NULL);
        if ($2) add_child(n, $2);
        $$ = n;
    }
    | MethodInvocation SEMICOLON { $$ = $1; }
    | Assignment SEMICOLON { $$ = $1; }
    | ParseArgs SEMICOLON { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    | PRINT LPAR PrintArg RPAR SEMICOLON {
        Node *n = new_node("Print", NULL);
        add_child(n, $3);
        $$ = n;
    }
    | error SEMICOLON { $$ = NULL; }
    ;

UnmatchedStatement
    : IF LPAR ExprOrAssign RPAR Statement {
        Node *n = new_node("If", NULL);
        add_child(n, $3);

        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        add_child(n, new_node("Block", NULL)); // Empty Else block
        $$ = n;
    }
    | IF LPAR ExprOrAssign RPAR MatchedStatement ELSE UnmatchedStatement {
        Node *n = new_node("If", NULL);
        add_child(n, $3);
        
        if ($5) add_child(n, $5);
        else add_child(n, new_node("Block", NULL));
        
        if ($7) add_child(n, $7);
        else add_child(n, new_node("Block", NULL));
        
        $$ = n;
    }
    | WHILE LPAR ExprOrAssign RPAR UnmatchedStatement {
        Node *n = new_node("While", NULL);
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
    | STRLIT { $$ = new_node("StrLit", $1); }
    ;

MethodInvocation
    : IDENTIFIER LPAR ExprListOpt RPAR {
        Node *n = new_node("Call", NULL);
        add_child(n, new_node("Identifier", $1));
        add_child(n, $3);
        $$ = n;
    }
    | IDENTIFIER LPAR error RPAR { $$ = NULL; }
    ;

ExprListOpt
    : ExprList { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

ExprList
    : ExprOrAssign { $$ = $1; }
    | ExprList COMMA Expr { $$ = append_sibling($1, $3); }
    ;

ExprOrAssign
    : Expr { $$ = $1; }
    | Assignment { $$ = $1; }
    ;

Assignment
    : IDENTIFIER ASSIGN ExprOrAssign {
        Node *n = new_node("Assign", NULL);
        add_child(n, new_node("Identifier", $1));
        add_child(n, $3);
        $$ = n;
    }
    ;

ParseArgs
    : PARSEINT LPAR IDENTIFIER LSQ ExprOrAssign RSQ RPAR {
        Node *n = new_node("ParseArgs", NULL);
        add_child(n, new_node("Identifier", $3));
        add_child(n, $5);
        $$ = n;
    }
    | PARSEINT LPAR error RPAR { $$ = NULL; }
    ;

Expr
    : Expr PLUS Expr { $$ = make_binary("Add", $1, $3); }
    | Expr MINUS Expr { $$ = make_binary("Sub", $1, $3); }
    | Expr STAR Expr { $$ = make_binary("Mul", $1, $3); }
    | Expr DIV Expr { $$ = make_binary("Div", $1, $3); }
    | Expr MOD Expr { $$ = make_binary("Mod", $1, $3); }
    | Expr AND Expr { $$ = make_binary("And", $1, $3); }
    | Expr OR Expr { $$ = make_binary("Or", $1, $3); }
    | Expr XOR Expr { $$ = make_binary("Xor", $1, $3); }
    | Expr LSHIFT Expr { $$ = make_binary("Lshift", $1, $3); }
    | Expr RSHIFT Expr { $$ = make_binary("Rshift", $1, $3); }
    | Expr EQ Expr { $$ = make_binary("Eq", $1, $3); }
    | Expr GE Expr { $$ = make_binary("Ge", $1, $3); }
    | Expr GT Expr { $$ = make_binary("Gt", $1, $3); }
    | Expr LE Expr { $$ = make_binary("Le", $1, $3); }
    | Expr LT Expr { $$ = make_binary("Lt", $1, $3); }
    | Expr NE Expr { $$ = make_binary("Ne", $1, $3); }
    | MINUS Expr %prec UMINUS { $$ = make_unary("Minus", $2); }
    | PLUS Expr %prec UPLUS { $$ = make_unary("Plus", $2); }
    | NOT Expr { $$ = make_unary("Not", $2); }
    //| LPAR Expr RPAR { $$ = $2; }
    | LPAR ExprOrAssign RPAR { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | MethodInvocation { $$ = $1; }
    // | Assignment { $$ = $1; }
    | ParseArgs { $$ = $1; }
    | IDENTIFIER DOTLENGTH {
        Node *n = new_node("Length", NULL);
        add_child(n, new_node("Identifier", $1));
        $$ = n;
    }
    | IDENTIFIER { $$ = new_node("Identifier", $1); }
    | NATURAL { $$ = new_node("Natural", $1); }
    | DECIMAL { $$ = new_node("Decimal", $1); }
    | BOOLLIT { $$ = new_node("BoolLit", $1); }
    ;

%%

void yyerror(const char *s) {
    syntax_errors++;
    printf("Line %d, col %d: %s: %s\n", row, column - (int)strlen(yytext) + 1, s, yytext);
    // printf("Line %d, col %d: %s: %s\n", row, column, s, yytext);
}

void print_final_ast() {
    if (flag_t && syntax_errors == 0 && !flag_e2) {
        print_ast(ast_root, 0);
    }
}
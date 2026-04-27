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

    /* place here any C variables and definitions */
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    #include "semantic.h"

    int row = 1;
    int column = 0;

    #define YY_USER_ACTION do { \
        yylloc.first_line = row; \
        yylloc.first_column = column + 1; \
        yylloc.last_line = row; \
        yylloc.last_column = column + yyleng; \
        column += yyleng; \
    } while (0);

    char str_lit[1024];
    char * str_ptr;

    int str_err = 0;

    int str_row = 0;
    int str_col = 0;
    int com_row = 0;
    int com_col = 0;

    int flag_l = 0;
    int lex_only = 0;
    int flag_t = 0;
    int flag_s = 0;

%}

digit   [0-9]
alpha   [_$a-zA-Z]
digits  {digit}([_0-9]*{digit})?
exponent    [eE][+-]?{digits}
whitespace [ \t\f]
newline (\r\n|\n|\r)

%X COMMENT
%X STRLITERAL

%%

{whitespace}+                             { ; }
{newline}                          { row++; column = 0; }


"true"|"false"                            { if(flag_l) printf("BOOLLIT(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return BOOLLIT; } }
"int"                                     { if(flag_l) printf("INT\n");        if (!lex_only) return INT; }
"double"                                  { if(flag_l) printf("DOUBLE\n");     if (!lex_only) return DOUBLE; }
"if"                                      { if(flag_l) printf("IF\n");         if (!lex_only) return IF; }
"else"                                    { if(flag_l) printf("ELSE\n");       if (!lex_only) return ELSE; }
"while"                                   { if(flag_l) printf("WHILE\n");      if (!lex_only) return WHILE; }
"return"                                  { if(flag_l) printf("RETURN\n");     if (!lex_only) return RETURN; }
"+"                                       { if(flag_l) printf("PLUS\n");       if (!lex_only) return PLUS; }
"-"                                       { if(flag_l) printf("MINUS\n");      if (!lex_only) return MINUS; }
"*"                                       { if(flag_l) printf("STAR\n");       if (!lex_only) return STAR; }
"/"                                       { if(flag_l) printf("DIV\n");        if (!lex_only) return DIV; }
"%"                                       { if(flag_l) printf("MOD\n");        if (!lex_only) return MOD; }
"="                                       { if(flag_l) printf("ASSIGN\n");     if (!lex_only) return ASSIGN; }
"("                                       { if(flag_l) printf("LPAR\n");       if (!lex_only) return LPAR; }
")"                                       { if(flag_l) printf("RPAR\n");       if (!lex_only) return RPAR; }
","                                       { if(flag_l) printf("COMMA\n");      if (!lex_only) return COMMA; }
"&&"                                      { if(flag_l) printf("AND\n");        if (!lex_only) return AND; }
"=="                                      { if(flag_l) printf("EQ\n");         if (!lex_only) return EQ; }
"<="                                      { if(flag_l) printf("LE\n");         if (!lex_only) return LE; }
">="                                      { if(flag_l) printf("GE\n");         if (!lex_only) return GE; }
"<"                                       { if(flag_l) printf("LT\n");         if (!lex_only) return LT; }
">"                                       { if(flag_l) printf("GT\n");         if (!lex_only) return GT; }
"{"                                       { if(flag_l) printf("LBRACE\n");     if (!lex_only) return LBRACE; }
"}"                                       { if(flag_l) printf("RBRACE\n");     if (!lex_only) return RBRACE; }
"["                                       { if(flag_l) printf("LSQ\n");        if (!lex_only) return LSQ; }
"]"                                       { if(flag_l) printf("RSQ\n");        if (!lex_only) return RSQ; }
"!="                                      { if(flag_l) printf("NE\n");         if (!lex_only) return NE; }
"!"                                       { if(flag_l) printf("NOT\n");        if (!lex_only) return NOT; }
"||"                                      { if(flag_l) printf("OR\n");         if (!lex_only) return OR; }
";"                                       { if(flag_l) printf("SEMICOLON\n");  if (!lex_only) return SEMICOLON; }
"->"                                      { if(flag_l) printf("ARROW\n");      if (!lex_only) return ARROW; }
">>"                                      { if(flag_l) printf("RSHIFT\n");     if (!lex_only) return RSHIFT; }
"<<"                                      { if(flag_l) printf("LSHIFT\n");     if (!lex_only) return LSHIFT; }
"^"                                       { if(flag_l) printf("XOR\n");        if (!lex_only) return XOR; }
"boolean"                                 { if(flag_l) printf("BOOL\n");       if (!lex_only) return BOOL; }
"class"                                   { if(flag_l) printf("CLASS\n");      if (!lex_only) return CLASS; }
".length"                                 { if(flag_l) printf("DOTLENGTH\n");  if (!lex_only) return DOTLENGTH; }
"System.out.print"                        { if(flag_l) printf("PRINT\n");      if (!lex_only) return PRINT; }
"Integer.parseInt"                        { if(flag_l) printf("PARSEINT\n");   if (!lex_only) return PARSEINT; }
"public"                                  { if(flag_l) printf("PUBLIC\n");     if (!lex_only) return PUBLIC; }
"static"                                  { if(flag_l) printf("STATIC\n");     if (!lex_only) return STATIC; }
"String"                                  { if(flag_l) printf("STRING\n");     if (!lex_only) return STRING; }
"void"                                    { if(flag_l) printf("VOID\n");       if (!lex_only) return VOID; }


"++"                    {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"--"                    {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"null"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"Integer"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"System"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"abstract"              {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"assert"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"break"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"byte"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"case"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"catch"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"char"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"const"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"continue"              {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"default"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"do"                    {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"enum"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"extends"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"final"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"finally"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"float"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"for"                   {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"goto"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"implements"            {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"import"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"instanceof"            {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"interface"             {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"long"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"native"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"new"                   {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"package"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"private"               {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"protected"             {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"short"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"strictfp"              {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"super"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"switch"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"synchronized"          {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"this"                  {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"throw"                 {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"throws"                {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"transient"             {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"try"                   {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}
"volatile"              {if(flag_l) printf("RESERVED(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; }}



{alpha}({alpha}|{digit})*           { if (flag_l) printf("IDENTIFIER(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return IDENTIFIER; } }
                                    
0|[1-9]([_0-9]*[0-9])?              { if (flag_l) printf("NATURAL(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return NATURAL; } }

{digits}"."({digits})?({exponent})?     { if (flag_l) printf("DECIMAL(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }
"."{digits}({exponent})?                { if (flag_l) printf("DECIMAL(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }
{digits}{exponent}                      { if (flag_l) printf("DECIMAL(%s)\n", yytext); if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }


"/*"                                { BEGIN(COMMENT); com_row = row; com_col = column - yyleng + 1; }
<COMMENT>{newline}               { row++; column = 0; }
<COMMENT>.                          { ; }
<COMMENT>"*/"                       { BEGIN(INITIAL); }
<COMMENT><<EOF>>                    { printf("Line %d, col %d: unterminated comment\n", com_row, com_col); BEGIN(INITIAL); return 0; }

"//"[^\n\r]* { ; }

\"      {
            BEGIN(STRLITERAL);
            str_err = 0;
            str_ptr = str_lit;
            str_row = row;
            str_col = column - yyleng + 1;
            *str_ptr++ = '\"';

        }

<STRLITERAL>\"  {
            *str_ptr++ = '\"';

            *str_ptr = '\0';

            if (!str_err) {
                // if (flag_l) printf("STRLIT(\"%s\")\n", str_lit);
                if (flag_l) printf("STRLIT(%s)\n", str_lit);

                if (!lex_only) {
                    yylval.lexeme = strdup(str_lit);
                    BEGIN(INITIAL); 
                    yytext = str_lit;
                    return STRLIT; 
                }
            }
            BEGIN(INITIAL);
        }

<STRLITERAL>\\[fnrt\\"]     { 
            *str_ptr++ = yytext[0];
            *str_ptr++ = yytext[1];
        }

<STRLITERAL>\\[^\n\r] {
            printf("Line %d, col %d: invalid escape sequence (%s)\n", row, column - yyleng + 1, yytext);
            str_err = 1;
        }

<STRLITERAL>\\  {
            printf("Line %d, col %d: invalid escape sequence (\\)\n", row, column - yyleng + 1);
            str_err = 1;
        }

<STRLITERAL>{newline}  {
            printf("Line %d, col %d: unterminated string literal\n", str_row, str_col);
            row++; column = 0;
            BEGIN(INITIAL);
        }

<STRLITERAL><<EOF>> {
            printf("Line %d, col %d: unterminated string literal\n", str_row, str_col);
            BEGIN(INITIAL);
            return 0;
        }

<STRLITERAL>[^\\\n\r\"]+ {
            char *yy_ptr = yytext;
            while ( *yy_ptr ) {
                    *str_ptr++ = *yy_ptr++;
            }
        }


.       { printf("Line %d, col %d: illegal character (%s)\n", row, column - yyleng + 1, yytext); }


%%
extern int yylex();
extern int yyparse();
extern void print_final_ast();
extern int get_syntax_errors();
extern int run_semantic(int print_tables);

void meta01() {
    lex_only = 1;
    yylex();
}

void meta01_l() {
    flag_l = 1;
    meta01();
}

void meta02() {
    yyparse();
    print_final_ast();
}

void meta02_t() {
    flag_t = 1;
    meta02();
}

void meta03() {
    yyparse();
    if (get_syntax_errors() == 0) {
        run_semantic(flag_s);
    }
    print_final_ast();
}

void meta03_s() {
    flag_s = 1;
    flag_t = 1;
    meta03();
}

int main(int argc, char **argv) {

    if (argc == 1){
        // meta mais recente
        meta03();
    } else if (argc > 1 && strcmp(argv[1], "-e1") == 0) {
        meta01();
    } else if (argc > 1 && strcmp(argv[1], "-l") == 0) {
        meta01_l();
    } else if (argc > 1 && strcmp(argv[1], "-e2") == 0) {
        meta02();
    } else if (argc > 1 && strcmp(argv[1], "-t") == 0) {
        meta02_t();
    } else if (argc > 1 && strcmp(argv[1], "-e3") == 0) {
        meta03();
    } else if (argc > 1 && strcmp(argv[1], "-s") == 0) {
        meta03_s();
    }

    return 0;
}


int yywrap() { 
    return 1;
}

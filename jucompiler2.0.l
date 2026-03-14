%{
    /* ---
    Disciplina de Compiladores
    ---
    Trabalho realizado por
    Leonardo Duarte
    2023213089
    --- */

    /* place here any C variables and definitions */
    #include <stdio.h>
    #define YY_USER_ACTION column += yyleng;

    #include <string.h>

    int row = 1;
    int column = 0;

    char str_lit[1024];
    char * str_ptr;

    int str_err = 0;

    int str_row = 0;
    int str_col = 0;
    int com_row = 0;
    int com_col = 0;

    int flag_l = 0;

%}

digit   [0-9]
alpha   [_$a-zA-Z]
digits  {digit}([_0-9]*{digit})?
exponencial    [eE][+-]?{digits}
whitespace [ \t\f]

%X COMMENT
%X STRLIT

%%

{whitespace}+                             { ; }
\r\n|\n|\r                          { row++; column = 0; }



"true"|"false"          { if (flag_l) printf("BOOLLIT(%s)\n", yytext); }
"&&"                    { if (flag_l) printf("AND\n"); }
"="                     { if (flag_l) printf("ASSIGN\n"); }
"*"                     { if (flag_l) printf("STAR\n"); }
","                     { if (flag_l) printf("COMMA\n"); }
"/"                     { if (flag_l) printf("DIV\n"); }
"=="                    { if (flag_l) printf("EQ\n"); }
">="                    { if (flag_l) printf("GE\n"); }
">"                     { if (flag_l) printf("GT\n"); }
"{"                     { if (flag_l) printf("LBRACE\n"); }
"<="                    { if (flag_l) printf("LE\n"); }
"("                     { if (flag_l) printf("LPAR\n"); }
"["                     { if (flag_l) printf("LSQ\n"); }
"<"                     { if (flag_l) printf("LT\n"); }
"-"                     { if (flag_l) printf("MINUS\n"); }
"%"                     { if (flag_l) printf("MOD\n"); }
"!="                    { if (flag_l) printf("NE\n"); }
"!"                     { if (flag_l) printf("NOT\n"); }
"||"                    { if (flag_l) printf("OR\n"); }
"+"                     { if (flag_l) printf("PLUS\n"); }
"}"                     { if (flag_l) printf("RBRACE\n"); }
")"                     { if (flag_l) printf("RPAR\n"); }
"]"                     { if (flag_l) printf("RSQ\n"); }
";"                     { if (flag_l) printf("SEMICOLON\n"); }
"->"                    { if (flag_l) printf("ARROW\n"); }
"<<"                    { if (flag_l) printf("LSHIFT\n"); }
">>"                    { if (flag_l) printf("RSHIFT\n"); }
"^"                     { if (flag_l) printf("XOR\n"); }
"boolean"               { if (flag_l) printf("BOOL\n"); }
"class"                 { if (flag_l) printf("CLASS\n"); }
".length"               { if (flag_l) printf("DOTLENGTH\n"); }
"double"                { if (flag_l) printf("DOUBLE\n"); }
"else"                  { if (flag_l) printf("ELSE\n"); }
"if"                    { if (flag_l) printf("IF\n"); }
"int"                   { if (flag_l) printf("INT\n"); }
"System.out.print"      { if (flag_l) printf("PRINT\n"); }
"Integer.parseInt"      { if (flag_l) printf("PARSEINT\n"); }
"public"                { if (flag_l) printf("PUBLIC\n"); }
"return"                { if (flag_l) printf("RETURN\n"); }
"static"                { if (flag_l) printf("STATIC\n"); }
"String"                { if (flag_l) printf("STRING\n"); }
"void"                  { if (flag_l) printf("VOID\n"); }
"while"                 { if (flag_l) printf("WHILE\n"); }


"++"                    { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"--"                    { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"null"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"Integer"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"System"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"abstract"              { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"assert"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"break"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"byte"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"case"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"catch"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"char"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"const"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"continue"              { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"default"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"do"                    { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"enum"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"extends"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"final"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"finally"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"float"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"for"                   { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"goto"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"implements"            { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"import"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"instanceof"            { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"interface"             { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"long"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"native"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"new"                   { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"package"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"private"               { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"protected"             { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"short"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"strictfp"              { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"super"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"switch"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"synchronized"          { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"this"                  { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"throw"                 { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"throws"                { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"transient"             { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"try"                   { if (flag_l) printf("RESERVED(%s)\n", yytext); }
"volatile"              { if (flag_l) printf("RESERVED(%s)\n", yytext); }



{alpha}({alpha}|{digit})*           { if (flag_l) printf("IDENTIFIER(%s)\n", yytext); }

0|[1-9]([_0-9]*[0-9])?              { if (flag_l) printf("NATURAL(%s)\n", yytext); }

{digits}"."({digits})?({exponencial})?     { if (flag_l) printf("DECIMAL(%s)\n", yytext); }
"."{digits}({exponencial})?                { if (flag_l) printf("DECIMAL(%s)\n", yytext); }
{digits}{exponencial}                      { if (flag_l) printf("DECIMAL(%s)\n", yytext); }

"/*"                                { BEGIN(COMMENT); com_row = row; com_col = column - yyleng + 1; }
<COMMENT>(\r\n|\n|\r)               { row++; column = 0; }
<COMMENT>.                          { ; }
<COMMENT>"*/"                       { BEGIN(INITIAL); }
<COMMENT><<EOF>>                    { printf("Line %d, col %d: unterminated comment\n", com_row, com_col); BEGIN(INITIAL); return 0; }

"//"[^\n\r]* { ; }

\"      {
            BEGIN(STRLIT);
            str_err = 0;
            str_ptr = str_lit;
            str_row = row;
            str_col = column - yyleng + 1;
        }

<STRLIT>\"  {
            *str_ptr = '\0';
            if (!str_err) {
                if (flag_l) printf("STRLIT(\"%s\")\n", str_lit);
            }
            BEGIN(INITIAL);
        }

<STRLIT>\\[fnrt\\"]     { 
            *str_ptr++ = yytext[0];
            *str_ptr++ = yytext[1];
        }

<STRLIT>\\[^\n\r] {
            printf("Line %d, col %d: invalid escape sequence (%s)\n", row, column - yyleng + 1, yytext);
            str_err = 1;
        }

<STRLIT>\\  {
            printf("Line %d, col %d: invalid escape sequence (\\)\n", row, column - yyleng + 1);
            str_err = 1;
        }

<STRLIT>(\r\n|\n|\r)  {
            printf("Line %d, col %d: unterminated string literal\n", str_row, str_col);
            row++; column = 0;
            BEGIN(INITIAL);
        }

<STRLIT><<EOF>> {
            printf("Line %d, col %d: unterminated string literal\n", str_row, str_col);
            BEGIN(INITIAL);
            return 0;
        }

<STRLIT>[^\\\n\r\"]+ {
            char *yy_ptr = yytext;
            while ( *yy_ptr ) {
                    *str_ptr++ = *yy_ptr++;
            }
        }


.       { printf("Line %d, col %d: illegal character (%s)\n", row, column - yyleng + 1, yytext); }


%%
extern int yylex();

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            flag_l = 1;
        } else if (strcmp(argv[1], "-e1") == 0) {
            flag_l = 0; 
        }
    }
    
    yylex();
    return 0;
}


int yywrap() { 
    return 1;
}

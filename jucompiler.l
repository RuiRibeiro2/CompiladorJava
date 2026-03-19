/* A simple lexer for a subset of the Ju compiler language, implemented using Flex. */
/*Rui Ribeiro, 23/20/2026, 2021189478*/

%{
    /* place here any C variables and definitions */
    #include <stdio.h>
    #include <ctype.h>
    #include <string.h>
    #include <stdlib.h>
    #include "jucompiler.tab.h"
    int line_number = 1; /* to keep track of line numbers for error reporting */
    int column_number = 1; /* to keep track of column numbers for error reporting */
    int str_start_col = 1; /* to keep track of the column where a string started */
    int str_has_error = 0; /* true if current string hit an invalid escape */
    int print_tokens = 0; /* print tokens in -l mode */
    int lex_only = 0;     /* true for -l or -e1 */
    int flag_t = 0;       /* true for -t */
    int flag_e2 = 0;      /* true for -e2 */
    char str_buffer[8192];
    int str_buffer_len = 0;
    int comment_line = 0; /* to keep track of the line where a comment started */
    int comment_col = 0; /* to keep track of the column where a comment started */

    static void str_reset(void) {
        str_buffer_len = 0;
        str_buffer[0] = '\0';
    }

    static void str_append(const char *text, int len) {
        int available = (int)sizeof(str_buffer) - 1 - str_buffer_len;
        if (available <= 0) return;
        if (len > available) len = available;
        memcpy(str_buffer + str_buffer_len, text, len);
        str_buffer_len += len;
        str_buffer[str_buffer_len] = '\0';
    }

%}

/*Definitions:*/
letter [a-zA-Z_$]
digit   [0-9]
digit_seq 0|[1-9]([0-9_]*{digit})?
digit_decimal {digit}([0-9_]*{digit})*
exponent   [eE][+-]?{digit_decimal}
newline (\r\n|\n|\r)
whitespace (" "|\t|\f)
reserved (abstract|assert|byte|case|catch|char|const|continue|default|do|enum|extends|final|finally|float|for|goto|implements|import|instanceof|interface|long|native|new|package|private|protected|short|super|switch|synchronized|this|throw|throws|transient|try|volatile|"++"|"--"|null|Integer|System|break|strictfp)

/*Rules*/
%X COMMENT STR LCOMENT
%%
"true"|"false"                            { if(print_tokens) printf("BOOLLIT(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return BOOLLIT; } }
"int"                                     { if(print_tokens) printf("INT\n"); column_number += yyleng; if (!lex_only) return INT; }
"double"                                  { if(print_tokens) printf("DOUBLE\n"); column_number += yyleng; if (!lex_only) return DOUBLE; }
"if"                                      { if(print_tokens) printf("IF\n"); column_number += yyleng; if (!lex_only) return IF; }
"else"                                    { if(print_tokens) printf("ELSE\n"); column_number += yyleng; if (!lex_only) return ELSE; }
"while"                                   { if(print_tokens) printf("WHILE\n"); column_number += yyleng; if (!lex_only) return WHILE; }
"return"                                  { if(print_tokens) printf("RETURN\n"); column_number += yyleng; if (!lex_only) return RETURN; }
"+"                                       { if(print_tokens) printf("PLUS\n"); column_number += yyleng; if (!lex_only) return PLUS; }
"-"                                       { if(print_tokens) printf("MINUS\n"); column_number += yyleng; if (!lex_only) return MINUS; }
"*"                                       { if(print_tokens) printf("STAR\n"); column_number += yyleng; if (!lex_only) return STAR; }
"/"                                       { if(print_tokens) printf("DIV\n"); column_number += yyleng; if (!lex_only) return DIV; }
"%"                                       { if(print_tokens) printf("MOD\n"); column_number += yyleng; if (!lex_only) return MOD; }
"="                                       { if(print_tokens) printf("ASSIGN\n"); column_number += yyleng; if (!lex_only) return ASSIGN; }
"("                                       { if(print_tokens) printf("LPAR\n"); column_number += yyleng; if (!lex_only) return LPAR; }
")"                                       { if(print_tokens) printf("RPAR\n"); column_number += yyleng; if (!lex_only) return RPAR; }
","                                       { if(print_tokens) printf("COMMA\n"); column_number += yyleng; if (!lex_only) return COMMA; }
"&&"                                      { if(print_tokens) printf("AND\n"); column_number += yyleng; if (!lex_only) return AND; }
"=="                                      { if(print_tokens) printf("EQ\n"); column_number += yyleng; if (!lex_only) return EQ; }
"<="                                      { if(print_tokens) printf("LE\n"); column_number += yyleng; if (!lex_only) return LE; }
">="                                      { if(print_tokens) printf("GE\n"); column_number += yyleng; if (!lex_only) return GE; }
"<"                                       { if(print_tokens) printf("LT\n"); column_number += yyleng; if (!lex_only) return LT; }
">"                                       { if(print_tokens) printf("GT\n"); column_number += yyleng; if (!lex_only) return GT; }
"{"                                       { if(print_tokens) printf("LBRACE\n"); column_number += yyleng; if (!lex_only) return LBRACE; }
"}"                                       { if(print_tokens) printf("RBRACE\n"); column_number += yyleng; if (!lex_only) return RBRACE; }
"["                                       { if(print_tokens) printf("LSQ\n"); column_number += yyleng; if (!lex_only) return LSQ; }
"]"                                       { if(print_tokens) printf("RSQ\n"); column_number += yyleng; if (!lex_only) return RSQ; }
"!="                                      { if(print_tokens) printf("NE\n"); column_number += yyleng; if (!lex_only) return NE; }
"!"                                       { if(print_tokens) printf("NOT\n"); column_number += yyleng; if (!lex_only) return NOT; }
"||"                                      { if(print_tokens) printf("OR\n"); column_number += yyleng; if (!lex_only) return OR; }
";"                                       { if(print_tokens) printf("SEMICOLON\n"); column_number += yyleng; if (!lex_only) return SEMICOLON; }
"->"                                      { if(print_tokens) printf("ARROW\n"); column_number += yyleng; if (!lex_only) return ARROW; }
">>"                                      { if(print_tokens) printf("RSHIFT\n"); column_number += yyleng; if (!lex_only) return RSHIFT; }
"<<"                                      { if(print_tokens) printf("LSHIFT\n"); column_number += yyleng; if (!lex_only) return LSHIFT; }
"^"                                       { if(print_tokens) printf("XOR\n"); column_number += yyleng; if (!lex_only) return XOR; }
"boolean"                                 { if(print_tokens) printf("BOOL\n"); column_number += yyleng; if (!lex_only) return BOOL; }
"class"                                   { if(print_tokens) printf("CLASS\n"); column_number += yyleng; if (!lex_only) return CLASS; }
".length"                                 { if(print_tokens) printf("DOTLENGTH\n"); column_number += yyleng; if (!lex_only) return DOTLENGTH; }
"System.out.print"                        { if(print_tokens) printf("PRINT\n"); column_number += yyleng; if (!lex_only) return PRINT; }
"Integer.parseInt"                        { if(print_tokens) printf("PARSEINT\n"); column_number += yyleng; if (!lex_only) return PARSEINT; }
"public"                                  { if(print_tokens) printf("PUBLIC\n"); column_number += yyleng; if (!lex_only) return PUBLIC; }
"static"                                  { if(print_tokens) printf("STATIC\n"); column_number += yyleng; if (!lex_only) return STATIC; }
"String"                                  { if(print_tokens) printf("STRING\n"); column_number += yyleng; if (!lex_only) return STRING; }
"void"                                    { if(print_tokens) printf("VOID\n"); column_number += yyleng; if (!lex_only) return VOID; }

{reserved}                                { if(print_tokens) printf("RESERVED(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return RESERVED; } }
{letter}({letter}|{digit})*               { if(print_tokens) printf("IDENTIFIER(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return IDENTIFIER; } }
{digit_seq}                               { if(print_tokens) printf("NATURAL(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return NATURAL; } }
{digit_decimal}"."{digit_decimal}?{exponent}? { if(print_tokens) printf("DECIMAL(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }
"."{digit_decimal}{exponent}?             { if(print_tokens) printf("DECIMAL(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }
{digit_decimal}{exponent}                 { if(print_tokens) printf("DECIMAL(%s)\n", yytext); column_number += yyleng; if (!lex_only) { yylval.lexeme = strdup(yytext); return DECIMAL; } }

{newline}                                 { line_number++; column_number = 1; }
{whitespace}                              { column_number += yyleng; /* ignore whitespace */ }

"/*"                                      { comment_line = line_number; comment_col = column_number; BEGIN(COMMENT); column_number += yyleng; } /* start of a comment */
<COMMENT>{newline}                        { line_number++; column_number = 1; /* ignore comment newline */ }
<COMMENT>.                                { column_number += yyleng; /* ignore comment content */ }
<COMMENT>"*/"                             { column_number += yyleng; BEGIN(INITIAL); } /* end of a comment */
<COMMENT><<EOF>>                          { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); BEGIN(INITIAL); /* EOF inside comment */ }

"//"                                      { BEGIN(LCOMENT); column_number += yyleng; } /* start of line comment */
<LCOMENT>{newline}                        { line_number++; column_number = 1; BEGIN(INITIAL); /* end of line comment */ }
<LCOMENT>.                                { column_number += yyleng; /* ignore line comment content */ }


\"                                        { str_start_col = column_number; str_has_error = 0; str_reset(); column_number += yyleng; BEGIN(STR); } /* start of a string */
<STR>([^"\\\n\r]|\\[fnrt\\"])+             { str_append(yytext, yyleng); column_number += yyleng; /* accepted: normal chars and valid escapes */ }
<STR>\\{newline}                           { printf("Line %d, col %d: invalid escape sequence (\\)\n", line_number, column_number); printf("Line %d, col %d: unterminated string literal\n", line_number, str_start_col);line_number++; column_number = 1; str_reset(); BEGIN(INITIAL); }
<STR>{newline}                             { printf("Line %d, col %d: unterminated string literal\n", line_number, str_start_col);line_number++; column_number = 1; str_reset(); BEGIN(INITIAL); }
<STR>\\[^fnrt\\\"\n\r]                     { str_has_error = 1; printf("Line %d, col %d: invalid escape sequence (%s)\n", line_number, column_number, yytext); column_number += yyleng; /* invalid escape, keep reading the same string */ }
<STR><<EOF>>                               { printf("Line %d, col %d: unterminated string literal\n", line_number, str_start_col); str_reset(); BEGIN(INITIAL); /* EOF inside string */ }
<STR>\"                                    { if (!str_has_error && print_tokens) printf("STRLIT(\"%s\")\n", str_buffer); if (!lex_only && !str_has_error) { yylval.lexeme = strdup(str_buffer); column_number += yyleng; str_reset(); BEGIN(INITIAL); return STRLIT; } column_number += yyleng; str_reset(); BEGIN(INITIAL); } /* end of a string */



.                                         { printf("Line %d, col %d: illegal character (%s)\n", line_number, column_number, yytext); column_number++; }

%%
/*Subroutines*/
extern int yylex();
extern int yyparse();
int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-l") == 0) {
        print_tokens = 1;
        lex_only = 1;
    } else if (argc > 1 && strcmp(argv[1], "-e1") == 0) {
        print_tokens = 0;
        lex_only = 1;
    } else if (argc > 1 && strcmp(argv[1], "-t") == 0) {
        flag_t = 1;
    } else if (argc > 1 && strcmp(argv[1], "-e2") == 0) {
        flag_e2 = 1;
    }

    if (lex_only)
        yylex();
    else
        yyparse();
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}

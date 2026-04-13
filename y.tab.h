/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 99 "jucompiler.y"

    typedef struct node Node;

#line 53 "y.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NATURAL = 259,                 /* NATURAL  */
    DECIMAL = 260,                 /* DECIMAL  */
    BOOLLIT = 261,                 /* BOOLLIT  */
    STRLIT = 262,                  /* STRLIT  */
    RESERVED = 263,                /* RESERVED  */
    BOOL = 264,                    /* BOOL  */
    INT = 265,                     /* INT  */
    DOUBLE = 266,                  /* DOUBLE  */
    STRING = 267,                  /* STRING  */
    VOID = 268,                    /* VOID  */
    CLASS = 269,                   /* CLASS  */
    PUBLIC = 270,                  /* PUBLIC  */
    STATIC = 271,                  /* STATIC  */
    RETURN = 272,                  /* RETURN  */
    IF = 273,                      /* IF  */
    ELSE = 274,                    /* ELSE  */
    WHILE = 275,                   /* WHILE  */
    PRINT = 276,                   /* PRINT  */
    PARSEINT = 277,                /* PARSEINT  */
    DOTLENGTH = 278,               /* DOTLENGTH  */
    ASSIGN = 279,                  /* ASSIGN  */
    STAR = 280,                    /* STAR  */
    COMMA = 281,                   /* COMMA  */
    DIV = 282,                     /* DIV  */
    EQ = 283,                      /* EQ  */
    GE = 284,                      /* GE  */
    GT = 285,                      /* GT  */
    LBRACE = 286,                  /* LBRACE  */
    LE = 287,                      /* LE  */
    LPAR = 288,                    /* LPAR  */
    LSQ = 289,                     /* LSQ  */
    LT = 290,                      /* LT  */
    MINUS = 291,                   /* MINUS  */
    MOD = 292,                     /* MOD  */
    NE = 293,                      /* NE  */
    NOT = 294,                     /* NOT  */
    OR = 295,                      /* OR  */
    PLUS = 296,                    /* PLUS  */
    RBRACE = 297,                  /* RBRACE  */
    RPAR = 298,                    /* RPAR  */
    RSQ = 299,                     /* RSQ  */
    SEMICOLON = 300,               /* SEMICOLON  */
    ARROW = 301,                   /* ARROW  */
    LSHIFT = 302,                  /* LSHIFT  */
    RSHIFT = 303,                  /* RSHIFT  */
    XOR = 304,                     /* XOR  */
    AND = 305,                     /* AND  */
    UPLUS = 306,                   /* UPLUS  */
    UMINUS = 307                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define NATURAL 259
#define DECIMAL 260
#define BOOLLIT 261
#define STRLIT 262
#define RESERVED 263
#define BOOL 264
#define INT 265
#define DOUBLE 266
#define STRING 267
#define VOID 268
#define CLASS 269
#define PUBLIC 270
#define STATIC 271
#define RETURN 272
#define IF 273
#define ELSE 274
#define WHILE 275
#define PRINT 276
#define PARSEINT 277
#define DOTLENGTH 278
#define ASSIGN 279
#define STAR 280
#define COMMA 281
#define DIV 282
#define EQ 283
#define GE 284
#define GT 285
#define LBRACE 286
#define LE 287
#define LPAR 288
#define LSQ 289
#define LT 290
#define MINUS 291
#define MOD 292
#define NE 293
#define NOT 294
#define OR 295
#define PLUS 296
#define RBRACE 297
#define RPAR 298
#define RSQ 299
#define SEMICOLON 300
#define ARROW 301
#define LSHIFT 302
#define RSHIFT 303
#define XOR 304
#define AND 305
#define UPLUS 306
#define UMINUS 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 104 "jucompiler.y"

    char *lexeme;
    Node *node;

#line 182 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

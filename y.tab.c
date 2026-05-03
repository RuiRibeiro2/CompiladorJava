/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "jucompiler.y"

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

#line 223 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 153 "jucompiler.y"

    #include "semantic.h"

#line 262 "y.tab.c"

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
#line 158 "jucompiler.y"

    char *lexeme;
    Node *node;

#line 391 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NATURAL = 4,                    /* NATURAL  */
  YYSYMBOL_DECIMAL = 5,                    /* DECIMAL  */
  YYSYMBOL_BOOLLIT = 6,                    /* BOOLLIT  */
  YYSYMBOL_STRLIT = 7,                     /* STRLIT  */
  YYSYMBOL_RESERVED = 8,                   /* RESERVED  */
  YYSYMBOL_BOOL = 9,                       /* BOOL  */
  YYSYMBOL_INT = 10,                       /* INT  */
  YYSYMBOL_DOUBLE = 11,                    /* DOUBLE  */
  YYSYMBOL_STRING = 12,                    /* STRING  */
  YYSYMBOL_VOID = 13,                      /* VOID  */
  YYSYMBOL_CLASS = 14,                     /* CLASS  */
  YYSYMBOL_PUBLIC = 15,                    /* PUBLIC  */
  YYSYMBOL_STATIC = 16,                    /* STATIC  */
  YYSYMBOL_RETURN = 17,                    /* RETURN  */
  YYSYMBOL_IF = 18,                        /* IF  */
  YYSYMBOL_ELSE = 19,                      /* ELSE  */
  YYSYMBOL_WHILE = 20,                     /* WHILE  */
  YYSYMBOL_PRINT = 21,                     /* PRINT  */
  YYSYMBOL_PARSEINT = 22,                  /* PARSEINT  */
  YYSYMBOL_DOTLENGTH = 23,                 /* DOTLENGTH  */
  YYSYMBOL_ASSIGN = 24,                    /* ASSIGN  */
  YYSYMBOL_STAR = 25,                      /* STAR  */
  YYSYMBOL_COMMA = 26,                     /* COMMA  */
  YYSYMBOL_DIV = 27,                       /* DIV  */
  YYSYMBOL_EQ = 28,                        /* EQ  */
  YYSYMBOL_GE = 29,                        /* GE  */
  YYSYMBOL_GT = 30,                        /* GT  */
  YYSYMBOL_LBRACE = 31,                    /* LBRACE  */
  YYSYMBOL_LE = 32,                        /* LE  */
  YYSYMBOL_LPAR = 33,                      /* LPAR  */
  YYSYMBOL_LSQ = 34,                       /* LSQ  */
  YYSYMBOL_LT = 35,                        /* LT  */
  YYSYMBOL_MINUS = 36,                     /* MINUS  */
  YYSYMBOL_MOD = 37,                       /* MOD  */
  YYSYMBOL_NE = 38,                        /* NE  */
  YYSYMBOL_NOT = 39,                       /* NOT  */
  YYSYMBOL_OR = 40,                        /* OR  */
  YYSYMBOL_PLUS = 41,                      /* PLUS  */
  YYSYMBOL_RBRACE = 42,                    /* RBRACE  */
  YYSYMBOL_RPAR = 43,                      /* RPAR  */
  YYSYMBOL_RSQ = 44,                       /* RSQ  */
  YYSYMBOL_SEMICOLON = 45,                 /* SEMICOLON  */
  YYSYMBOL_ARROW = 46,                     /* ARROW  */
  YYSYMBOL_LSHIFT = 47,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 48,                    /* RSHIFT  */
  YYSYMBOL_XOR = 49,                       /* XOR  */
  YYSYMBOL_AND = 50,                       /* AND  */
  YYSYMBOL_UPLUS = 51,                     /* UPLUS  */
  YYSYMBOL_UMINUS = 52,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_Program = 54,                   /* Program  */
  YYSYMBOL_ProgramMembers = 55,            /* ProgramMembers  */
  YYSYMBOL_ProgramMember = 56,             /* ProgramMember  */
  YYSYMBOL_MethodDecl = 57,                /* MethodDecl  */
  YYSYMBOL_FieldDecl = 58,                 /* FieldDecl  */
  YYSYMBOL_FieldDeclTail = 59,             /* FieldDeclTail  */
  YYSYMBOL_Type = 60,                      /* Type  */
  YYSYMBOL_MethodHeader = 61,              /* MethodHeader  */
  YYSYMBOL_FormalParamsOpt = 62,           /* FormalParamsOpt  */
  YYSYMBOL_FormalParams = 63,              /* FormalParams  */
  YYSYMBOL_FormalParamsTail = 64,          /* FormalParamsTail  */
  YYSYMBOL_MethodBody = 65,                /* MethodBody  */
  YYSYMBOL_MethodBodyItems = 66,           /* MethodBodyItems  */
  YYSYMBOL_MethodBodyItem = 67,            /* MethodBodyItem  */
  YYSYMBOL_VarDecl = 68,                   /* VarDecl  */
  YYSYMBOL_VarDeclTail = 69,               /* VarDeclTail  */
  YYSYMBOL_Statement = 70,                 /* Statement  */
  YYSYMBOL_MatchedStatement = 71,          /* MatchedStatement  */
  YYSYMBOL_UnmatchedStatement = 72,        /* UnmatchedStatement  */
  YYSYMBOL_StatementList = 73,             /* StatementList  */
  YYSYMBOL_ExprOpt = 74,                   /* ExprOpt  */
  YYSYMBOL_PrintArg = 75,                  /* PrintArg  */
  YYSYMBOL_MethodInvocation = 76,          /* MethodInvocation  */
  YYSYMBOL_ExprListOpt = 77,               /* ExprListOpt  */
  YYSYMBOL_ExprList = 78,                  /* ExprList  */
  YYSYMBOL_Assignment = 79,                /* Assignment  */
  YYSYMBOL_ParseArgs = 80,                 /* ParseArgs  */
  YYSYMBOL_ExprOrAssign = 81,              /* ExprOrAssign  */
  YYSYMBOL_Expr = 82                       /* Expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   373

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  173

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   197,   197,   207,   208,   212,   213,   214,   218,   227,
     251,   255,   260,   264,   265,   266,   271,   279,   290,   291,
     295,   304,   317,   324,   328,   336,   337,   341,   342,   346,
     373,   378,   382,   383,   387,   396,   408,   418,   424,   425,
     426,   427,   428,   434,   438,   449,   462,   475,   476,   481,
     482,   486,   487,   491,   499,   503,   504,   508,   509,   514,
     525,   533,   537,   538,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   572,   573,
     574,   575
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "NATURAL", "DECIMAL", "BOOLLIT", "STRLIT", "RESERVED", "BOOL", "INT",
  "DOUBLE", "STRING", "VOID", "CLASS", "PUBLIC", "STATIC", "RETURN", "IF",
  "ELSE", "WHILE", "PRINT", "PARSEINT", "DOTLENGTH", "ASSIGN", "STAR",
  "COMMA", "DIV", "EQ", "GE", "GT", "LBRACE", "LE", "LPAR", "LSQ", "LT",
  "MINUS", "MOD", "NE", "NOT", "OR", "PLUS", "RBRACE", "RPAR", "RSQ",
  "SEMICOLON", "ARROW", "LSHIFT", "RSHIFT", "XOR", "AND", "UPLUS",
  "UMINUS", "$accept", "Program", "ProgramMembers", "ProgramMember",
  "MethodDecl", "FieldDecl", "FieldDeclTail", "Type", "MethodHeader",
  "FormalParamsOpt", "FormalParams", "FormalParamsTail", "MethodBody",
  "MethodBodyItems", "MethodBodyItem", "VarDecl", "VarDeclTail",
  "Statement", "MatchedStatement", "UnmatchedStatement", "StatementList",
  "ExprOpt", "PrintArg", "MethodInvocation", "ExprListOpt", "ExprList",
  "Assignment", "ParseArgs", "ExprOrAssign", "Expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-133)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-57)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -5,    31,    42,     4,  -133,  -133,    18,     3,    45,  -133,
    -133,  -133,  -133,  -133,  -133,    83,  -133,  -133,  -133,    59,
      61,    36,    38,    40,  -133,  -133,    94,    94,   -19,     7,
      63,    66,    52,  -133,    56,    71,  -133,    65,    13,   205,
      68,    78,    80,    85,  -133,  -133,  -133,   131,  -133,  -133,
    -133,  -133,  -133,    90,    91,    92,    96,  -133,  -133,  -133,
    -133,  -133,   205,   144,    35,  -133,  -133,  -133,   156,   209,
     209,   209,    93,  -133,  -133,  -133,  -133,   224,   205,   205,
     166,    11,   111,  -133,  -133,  -133,  -133,   143,   125,  -133,
     112,   115,   139,  -133,  -133,   124,   133,    17,  -133,  -133,
    -133,  -133,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   136,   138,
    -133,   141,  -133,   147,   148,  -133,  -133,   -13,  -133,    98,
    -133,  -133,   205,  -133,  -133,  -133,  -133,   325,   127,   127,
     127,   127,    14,  -133,   325,   250,    14,    29,    29,   301,
     276,    99,    99,   146,  -133,   205,   183,  -133,   190,  -133,
    -133,   175,  -133,  -133,  -133,   152,  -133,  -133,    99,   155,
    -133,  -133,  -133
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     4,     0,     0,     0,     2,
       7,     3,     5,     6,    10,     0,    13,    14,    15,     0,
       0,     0,     0,    12,    26,     8,    19,    19,     0,     0,
       0,     0,     0,    18,     0,     0,     9,     0,     0,    50,
       0,     0,     0,     0,    48,    24,    41,     0,    25,    27,
      28,    32,    33,     0,     0,     0,     0,    23,    17,    16,
      11,    43,     0,     0,    88,    89,    90,    91,     0,     0,
       0,     0,     0,    85,    62,    86,    49,    63,     0,     0,
       0,     0,     0,    31,    38,    39,    40,     0,    21,    59,
       0,     0,    55,    57,    87,     0,     0,    88,    80,    82,
      81,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    51,     0,     0,    34,    47,     0,    20,     0,
      54,    53,     0,    84,    83,    66,    67,    74,    75,    76,
      77,    78,    65,    68,    79,    70,    64,    72,    73,    71,
      69,     0,     0,     0,    61,     0,     0,    29,     0,    58,
      44,    32,    36,    46,    42,     0,    30,    22,     0,     0,
      35,    45,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,  -133,  -133,  -133,  -133,  -133,  -133,   -14,  -133,   173,
    -133,  -133,  -133,  -133,  -133,  -133,  -133,   -79,  -121,  -132,
    -133,  -133,  -133,   -29,  -133,  -133,   -27,   -25,   -57,   -26
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,    11,    12,    13,    28,    31,    21,    32,
      33,    88,    25,    29,    48,    49,   127,    50,    51,    52,
      82,    72,   121,    73,    91,    92,    74,    75,    76,    77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,    20,    54,   126,    55,    89,    93,    35,    37,     1,
      38,    96,   123,   156,   124,    47,    16,    17,    18,     7,
     163,   118,   119,   122,    39,    40,    36,    41,    42,    43,
     161,   162,   157,     8,     3,     5,   171,    62,    44,   102,
      94,   103,     4,    98,    99,   100,    63,   170,    14,    45,
      63,   110,    46,    53,   102,    54,   103,    55,    94,    62,
       9,    15,    22,    10,    23,   109,   110,    24,    63,    57,
     113,    26,   160,    27,    60,   159,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    16,    17,    18,    58,    19,    56,   165,    59,
      37,    78,    38,    16,    17,    18,    30,    16,    17,    18,
      61,    79,    37,    80,    38,   158,    39,    40,    81,    41,
      42,    43,    53,    53,    54,    54,    55,    55,    39,    40,
      44,    41,    42,    43,    83,    84,    85,    86,   101,    53,
      87,    54,    44,    55,    46,    90,   128,    64,    65,    66,
      67,   129,   102,   125,   103,   130,    46,    95,   131,    64,
      65,    66,    67,   109,   110,   132,    43,   133,   113,    64,
      65,    66,    67,   120,   114,   115,   134,    68,    43,   151,
      69,   152,   155,    70,   153,    71,   166,   -56,    43,    68,
     154,   164,    69,   167,   168,    70,   169,    71,   172,    68,
      34,     0,    69,     0,     0,    70,     0,    71,    64,    65,
      66,    67,    97,    65,    66,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,    43,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    69,    68,     0,    70,    69,    71,     0,    70,   102,
      71,   103,   104,   105,   106,     0,   107,     0,     0,   108,
     109,   110,   111,     0,   112,   113,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   102,     0,   103,   104,   105,
     106,     0,   107,     0,     0,   108,   109,   110,   111,     0,
       0,   113,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   102,     0,   103,   104,   105,   106,     0,   107,     0,
       0,   108,   109,   110,   111,     0,     0,   113,     0,     0,
       0,     0,     0,   114,   115,   116,   102,     0,   103,   104,
     105,   106,     0,   107,     0,     0,   108,   109,   110,   111,
       0,     0,   113,     0,     0,     0,     0,     0,   114,   115,
     102,     0,   103,     0,   105,   106,     0,   107,     0,     0,
     108,   109,   110,     0,     0,     0,   113,     0,     0,     0,
       0,     0,   114,   115
};

static const yytype_int16 yycheck[] =
{
      29,    15,    29,    82,    29,    62,    63,    26,     1,    14,
       3,    68,     1,    26,     3,    29,     9,    10,    11,     1,
     152,    78,    79,    80,    17,    18,    45,    20,    21,    22,
     151,   152,    45,    15,     3,    31,   168,    24,    31,    25,
      23,    27,     0,    69,    70,    71,    33,   168,    45,    42,
      33,    37,    45,    82,    25,    82,    27,    82,    23,    24,
      42,    16,     3,    45,     3,    36,    37,    31,    33,     3,
      41,    33,   151,    33,     3,   132,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     9,    10,    11,    43,    13,    34,   155,    43,
       1,    33,     3,     9,    10,    11,    12,     9,    10,    11,
      45,    33,     1,    33,     3,   129,    17,    18,    33,    20,
      21,    22,   151,   152,   151,   152,   151,   152,    17,    18,
      31,    20,    21,    22,     3,    45,    45,    45,    45,   168,
      44,   168,    31,   168,    45,     1,     3,     3,     4,     5,
       6,    26,    25,    42,    27,    43,    45,     1,    43,     3,
       4,     5,     6,    36,    37,    26,    22,    43,    41,     3,
       4,     5,     6,     7,    47,    48,    43,    33,    22,    43,
      36,    43,    34,    39,    43,    41,     3,    43,    22,    33,
      43,    45,    36,     3,    19,    39,    44,    41,    43,    33,
      27,    -1,    36,    -1,    -1,    39,    -1,    41,     3,     4,
       5,     6,     3,     4,     5,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    36,    33,    -1,    39,    36,    41,    -1,    39,    25,
      41,    27,    28,    29,    30,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,    50,    25,    -1,    27,    28,    29,
      30,    -1,    32,    -1,    -1,    35,    36,    37,    38,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,
      50,    25,    -1,    27,    28,    29,    30,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    25,    -1,    27,    28,
      29,    30,    -1,    32,    -1,    -1,    35,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    48,
      25,    -1,    27,    -1,    29,    30,    -1,    32,    -1,    -1,
      35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    14,    54,     3,     0,    31,    55,     1,    15,    42,
      45,    56,    57,    58,    45,    16,     9,    10,    11,    13,
      60,    61,     3,     3,    31,    65,    33,    33,    59,    66,
      12,    60,    62,    63,    62,    26,    45,     1,     3,    17,
      18,    20,    21,    22,    31,    42,    45,    60,    67,    68,
      70,    71,    72,    76,    79,    80,    34,     3,    43,    43,
       3,    45,    24,    33,     3,     4,     5,     6,    33,    36,
      39,    41,    74,    76,    79,    80,    81,    82,    33,    33,
      33,    33,    73,     3,    45,    45,    45,    44,    64,    81,
       1,    77,    78,    81,    23,     1,    81,     3,    82,    82,
      82,    45,    25,    27,    28,    29,    30,    32,    35,    36,
      37,    38,    40,    41,    47,    48,    49,    50,    81,    81,
       7,    75,    81,     1,     3,    42,    70,    69,     3,    26,
      43,    43,    26,    43,    43,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    43,    43,    43,    43,    34,    26,    45,    60,    81,
      70,    71,    71,    72,    45,    81,     3,     3,    19,    44,
      71,    72,    43
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    55,    56,    56,    56,    57,    58,
      58,    59,    59,    60,    60,    60,    61,    61,    62,    62,
      63,    63,    64,    64,    65,    66,    66,    67,    67,    68,
      69,    69,    70,    70,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    72,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    78,    78,    79,
      80,    80,    81,    81,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     0,     1,     1,     1,     4,     6,
       2,     3,     0,     1,     1,     1,     5,     5,     1,     0,
       4,     3,     4,     0,     3,     2,     0,     1,     1,     4,
       3,     0,     1,     1,     3,     7,     5,     3,     2,     2,
       2,     1,     5,     2,     5,     7,     5,     2,     0,     1,
       0,     1,     1,     4,     4,     1,     0,     1,     3,     3,
       7,     4,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     3,     3,     1,     1,     2,     1,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1456 "y.tab.c"
        break;

    case YYSYMBOL_NATURAL: /* NATURAL  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1462 "y.tab.c"
        break;

    case YYSYMBOL_DECIMAL: /* DECIMAL  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1468 "y.tab.c"
        break;

    case YYSYMBOL_BOOLLIT: /* BOOLLIT  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1474 "y.tab.c"
        break;

    case YYSYMBOL_STRLIT: /* STRLIT  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1480 "y.tab.c"
        break;

    case YYSYMBOL_RESERVED: /* RESERVED  */
#line 163 "jucompiler.y"
            { free(((*yyvaluep).lexeme)); }
#line 1486 "y.tab.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: CLASS IDENTIFIER LBRACE ProgramMembers RBRACE  */
#line 197 "jucompiler.y"
                                                    {
        Node *n = new_node("Program", NULL);
        add_child(n, make_identifier_at((yyvsp[-3].lexeme), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
        free((yyvsp[-3].lexeme));
        add_child(n, (yyvsp[-1].node));
        ast_root = n;
    }
#line 1787 "y.tab.c"
    break;

  case 3: /* ProgramMembers: ProgramMembers ProgramMember  */
#line 207 "jucompiler.y"
                                   { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1793 "y.tab.c"
    break;

  case 4: /* ProgramMembers: %empty  */
#line 208 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1799 "y.tab.c"
    break;

  case 5: /* ProgramMember: MethodDecl  */
#line 212 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1805 "y.tab.c"
    break;

  case 6: /* ProgramMember: FieldDecl  */
#line 213 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1811 "y.tab.c"
    break;

  case 7: /* ProgramMember: SEMICOLON  */
#line 214 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 1817 "y.tab.c"
    break;

  case 8: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 218 "jucompiler.y"
                                            {
        Node *n = new_node("MethodDecl", NULL);
        add_child(n, (yyvsp[-1].node));
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1828 "y.tab.c"
    break;

  case 9: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldDeclTail SEMICOLON  */
#line 227 "jucompiler.y"
                                                            {
        Node *head = NULL;
        Node *type = (yyvsp[-3].node);
        Node *id = make_identifier_at((yyvsp[-2].lexeme), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        Node *fie_decl = new_node("FieldDecl", NULL);
        add_child(fie_decl, make_type_node(type));
        add_child(fie_decl, id);
        head = append_sibling(head, fie_decl);
        Node *cur = (yyvsp[-1].node);
        while (cur) {
            Node * next = cur->sibling;
            cur->sibling = NULL;

            Node *fie_decl2 = new_node("FieldDecl", NULL);
            add_child(fie_decl2, make_type_node(type));
            add_child(fie_decl2, cur);
            head = append_sibling(head, fie_decl2);

            cur = next;
        }
        free_ast((yyvsp[-3].node));
        free((yyvsp[-2].lexeme));
        (yyval.node) = head;
    }
#line 1857 "y.tab.c"
    break;

  case 10: /* FieldDecl: error SEMICOLON  */
#line 251 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 1863 "y.tab.c"
    break;

  case 11: /* FieldDeclTail: FieldDeclTail COMMA IDENTIFIER  */
#line 255 "jucompiler.y"
                                     {
        Node *id = make_identifier_at((yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column);
        free((yyvsp[0].lexeme));
        (yyval.node) = append_sibling((yyvsp[-2].node), id);
    }
#line 1873 "y.tab.c"
    break;

  case 12: /* FieldDeclTail: %empty  */
#line 260 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1879 "y.tab.c"
    break;

  case 13: /* Type: BOOL  */
#line 264 "jucompiler.y"
           { (yyval.node) = new_node("Bool", NULL); }
#line 1885 "y.tab.c"
    break;

  case 14: /* Type: INT  */
#line 265 "jucompiler.y"
          { (yyval.node) = new_node("Int", NULL); }
#line 1891 "y.tab.c"
    break;

  case 15: /* Type: DOUBLE  */
#line 266 "jucompiler.y"
             { (yyval.node) = new_node("Double", NULL); }
#line 1897 "y.tab.c"
    break;

  case 16: /* MethodHeader: Type IDENTIFIER LPAR FormalParamsOpt RPAR  */
#line 271 "jucompiler.y"
                                                {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, (yyvsp[-4].node));
        add_child(n, make_identifier_at((yyvsp[-3].lexeme), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
        free((yyvsp[-3].lexeme));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1910 "y.tab.c"
    break;

  case 17: /* MethodHeader: VOID IDENTIFIER LPAR FormalParamsOpt RPAR  */
#line 279 "jucompiler.y"
                                                {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, new_node("Void", NULL));
        add_child(n, make_identifier_at((yyvsp[-3].lexeme), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
        add_child(n, (yyvsp[-1].node));
        free((yyvsp[-3].lexeme));
        (yyval.node) = n;
    }
#line 1923 "y.tab.c"
    break;

  case 18: /* FormalParamsOpt: FormalParams  */
#line 290 "jucompiler.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1929 "y.tab.c"
    break;

  case 19: /* FormalParamsOpt: %empty  */
#line 291 "jucompiler.y"
                  { (yyval.node) = new_node("MethodParams", NULL); }
#line 1935 "y.tab.c"
    break;

  case 20: /* FormalParams: STRING LSQ RSQ IDENTIFIER  */
#line 295 "jucompiler.y"
                                {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, new_node("StringArray", NULL));
        add_child(pd, make_identifier_at((yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column));
        free((yyvsp[0].lexeme));
        add_child(n, pd);
        (yyval.node) = n;
    }
#line 1949 "y.tab.c"
    break;

  case 21: /* FormalParams: Type IDENTIFIER FormalParamsTail  */
#line 304 "jucompiler.y"
                                       {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, (yyvsp[-2].node));
        add_child(pd, make_identifier_at((yyvsp[-1].lexeme), (yylsp[-1]).first_line, (yylsp[-1]).first_column));
        free((yyvsp[-1].lexeme));
        add_child(n, pd);
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1964 "y.tab.c"
    break;

  case 22: /* FormalParamsTail: FormalParamsTail COMMA Type IDENTIFIER  */
#line 317 "jucompiler.y"
                                             {
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, (yyvsp[-1].node));
        add_child(pd, make_identifier_at((yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column));
        free((yyvsp[0].lexeme));
        (yyval.node) = append_sibling((yyvsp[-3].node), pd);
    }
#line 1976 "y.tab.c"
    break;

  case 23: /* FormalParamsTail: %empty  */
#line 324 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1982 "y.tab.c"
    break;

  case 24: /* MethodBody: LBRACE MethodBodyItems RBRACE  */
#line 328 "jucompiler.y"
                                    {
        Node *n = new_node("MethodBody", NULL);
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1992 "y.tab.c"
    break;

  case 25: /* MethodBodyItems: MethodBodyItems MethodBodyItem  */
#line 336 "jucompiler.y"
                                     { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1998 "y.tab.c"
    break;

  case 26: /* MethodBodyItems: %empty  */
#line 337 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2004 "y.tab.c"
    break;

  case 27: /* MethodBodyItem: VarDecl  */
#line 341 "jucompiler.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2010 "y.tab.c"
    break;

  case 28: /* MethodBodyItem: Statement  */
#line 342 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2016 "y.tab.c"
    break;

  case 29: /* VarDecl: Type IDENTIFIER VarDeclTail SEMICOLON  */
#line 346 "jucompiler.y"
                                            {
        Node *head = NULL;
        Node *type = (yyvsp[-3].node);
        Node *id = make_identifier_at((yyvsp[-2].lexeme), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        Node *var_decl = new_node("VarDecl", NULL);
        add_child(var_decl, make_type_node(type));
        add_child(var_decl, id);
        head = append_sibling(head, var_decl);
        Node *cur = (yyvsp[-1].node);
        while (cur) {
            Node *next = cur->sibling;
            cur->sibling = NULL;

            Node *var_decl2 = new_node("VarDecl", NULL);
            add_child(var_decl2, make_type_node(type));
            add_child(var_decl2, cur);
            head = append_sibling(head, var_decl2);

            cur = next;
        }
        free_ast((yyvsp[-3].node));
        free((yyvsp[-2].lexeme));
        (yyval.node) = head;
    }
#line 2045 "y.tab.c"
    break;

  case 30: /* VarDeclTail: VarDeclTail COMMA IDENTIFIER  */
#line 373 "jucompiler.y"
                                   {
        Node *id = make_identifier_at((yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.node) = append_sibling((yyvsp[-2].node), id);
        free((yyvsp[0].lexeme));
    }
#line 2055 "y.tab.c"
    break;

  case 31: /* VarDeclTail: %empty  */
#line 378 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2061 "y.tab.c"
    break;

  case 32: /* Statement: MatchedStatement  */
#line 382 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2067 "y.tab.c"
    break;

  case 33: /* Statement: UnmatchedStatement  */
#line 383 "jucompiler.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2073 "y.tab.c"
    break;

  case 34: /* MatchedStatement: LBRACE StatementList RBRACE  */
#line 387 "jucompiler.y"
                                  {
        if (!(yyvsp[-1].node)) (yyval.node) = NULL;
        else if (!(yyvsp[-1].node)->sibling) (yyval.node) = (yyvsp[-1].node);
        else {
            Node *b = new_node("Block", NULL);
            add_child(b, (yyvsp[-1].node));
            (yyval.node) = b;
        }
    }
#line 2087 "y.tab.c"
    break;

  case 35: /* MatchedStatement: IF LPAR ExprOrAssign RPAR MatchedStatement ELSE MatchedStatement  */
#line 396 "jucompiler.y"
                                                                       {
        Node *n = new_node("If", NULL);
        set_node_loc(n, (yylsp[-6]).first_line, (yylsp[-6]).first_column);
        add_child(n, (yyvsp[-4].node));
        
        if ((yyvsp[-2].node)) add_child(n, (yyvsp[-2].node));
        else add_child(n, new_node("Block", NULL));
        
        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));
        (yyval.node) = n;
    }
#line 2104 "y.tab.c"
    break;

  case 36: /* MatchedStatement: WHILE LPAR ExprOrAssign RPAR MatchedStatement  */
#line 408 "jucompiler.y"
                                                    {
        Node *n = new_node("While", NULL);
        set_node_loc(n, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        add_child(n, (yyvsp[-2].node));

        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));

        (yyval.node) = n;
    }
#line 2119 "y.tab.c"
    break;

  case 37: /* MatchedStatement: RETURN ExprOpt SEMICOLON  */
#line 418 "jucompiler.y"
                               {
        Node *n = new_node("Return", NULL);
        set_node_loc(n, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        if ((yyvsp[-1].node)) add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 2130 "y.tab.c"
    break;

  case 38: /* MatchedStatement: MethodInvocation SEMICOLON  */
#line 424 "jucompiler.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2136 "y.tab.c"
    break;

  case 39: /* MatchedStatement: Assignment SEMICOLON  */
#line 425 "jucompiler.y"
                           { (yyval.node) = (yyvsp[-1].node); }
#line 2142 "y.tab.c"
    break;

  case 40: /* MatchedStatement: ParseArgs SEMICOLON  */
#line 426 "jucompiler.y"
                          { (yyval.node) = (yyvsp[-1].node); }
#line 2148 "y.tab.c"
    break;

  case 41: /* MatchedStatement: SEMICOLON  */
#line 427 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 2154 "y.tab.c"
    break;

  case 42: /* MatchedStatement: PRINT LPAR PrintArg RPAR SEMICOLON  */
#line 428 "jucompiler.y"
                                         {
        Node *n = new_node("Print", NULL);
        set_node_loc(n, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        add_child(n, (yyvsp[-2].node));
        (yyval.node) = n;
    }
#line 2165 "y.tab.c"
    break;

  case 43: /* MatchedStatement: error SEMICOLON  */
#line 434 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 2171 "y.tab.c"
    break;

  case 44: /* UnmatchedStatement: IF LPAR ExprOrAssign RPAR Statement  */
#line 438 "jucompiler.y"
                                          {
        Node *n = new_node("If", NULL);
        set_node_loc(n, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        add_child(n, (yyvsp[-2].node));

        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));
        
        add_child(n, new_node("Block", NULL)); // Empty Else block
        (yyval.node) = n;
    }
#line 2187 "y.tab.c"
    break;

  case 45: /* UnmatchedStatement: IF LPAR ExprOrAssign RPAR MatchedStatement ELSE UnmatchedStatement  */
#line 449 "jucompiler.y"
                                                                         {
        Node *n = new_node("If", NULL);
        set_node_loc(n, (yylsp[-6]).first_line, (yylsp[-6]).first_column);
        add_child(n, (yyvsp[-4].node));
        
        if ((yyvsp[-2].node)) add_child(n, (yyvsp[-2].node));
        else add_child(n, new_node("Block", NULL));
        
        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));
        
        (yyval.node) = n;
    }
#line 2205 "y.tab.c"
    break;

  case 46: /* UnmatchedStatement: WHILE LPAR ExprOrAssign RPAR UnmatchedStatement  */
#line 462 "jucompiler.y"
                                                      {
        Node *n = new_node("While", NULL);
        set_node_loc(n, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        add_child(n, (yyvsp[-2].node));

        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));

        (yyval.node) = n;
    }
#line 2220 "y.tab.c"
    break;

  case 47: /* StatementList: StatementList Statement  */
#line 475 "jucompiler.y"
                              { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2226 "y.tab.c"
    break;

  case 48: /* StatementList: %empty  */
#line 476 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2232 "y.tab.c"
    break;

  case 49: /* ExprOpt: ExprOrAssign  */
#line 481 "jucompiler.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2238 "y.tab.c"
    break;

  case 50: /* ExprOpt: %empty  */
#line 482 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2244 "y.tab.c"
    break;

  case 51: /* PrintArg: ExprOrAssign  */
#line 486 "jucompiler.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2250 "y.tab.c"
    break;

  case 52: /* PrintArg: STRLIT  */
#line 487 "jucompiler.y"
             { (yyval.node) = new_node("StrLit", (yyvsp[0].lexeme)); free((yyvsp[0].lexeme)); }
#line 2256 "y.tab.c"
    break;

  case 53: /* MethodInvocation: IDENTIFIER LPAR ExprListOpt RPAR  */
#line 491 "jucompiler.y"
                                       {
        Node *n = new_node("Call", NULL);
        add_child(n, make_identifier_at((yyvsp[-3].lexeme), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
        set_node_loc(n, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
        free((yyvsp[-3].lexeme));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 2269 "y.tab.c"
    break;

  case 54: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 499 "jucompiler.y"
                                 { free((yyvsp[-3].lexeme)); (yyval.node) = NULL; }
#line 2275 "y.tab.c"
    break;

  case 55: /* ExprListOpt: ExprList  */
#line 503 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2281 "y.tab.c"
    break;

  case 56: /* ExprListOpt: %empty  */
#line 504 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 2287 "y.tab.c"
    break;

  case 57: /* ExprList: ExprOrAssign  */
#line 508 "jucompiler.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2293 "y.tab.c"
    break;

  case 58: /* ExprList: ExprList COMMA ExprOrAssign  */
#line 509 "jucompiler.y"
                                  { (yyval.node) = append_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2299 "y.tab.c"
    break;

  case 59: /* Assignment: IDENTIFIER ASSIGN ExprOrAssign  */
#line 514 "jucompiler.y"
                                     {
        Node *n = new_node("Assign", NULL);
        add_child(n, make_identifier_at((yyvsp[-2].lexeme), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
        set_node_loc(n, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-2].lexeme));
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 2312 "y.tab.c"
    break;

  case 60: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ ExprOrAssign RSQ RPAR  */
#line 525 "jucompiler.y"
                                                         {
        Node *n = new_node("ParseArgs", NULL);
        add_child(n, make_identifier_at((yyvsp[-4].lexeme), (yylsp[-4]).first_line, (yylsp[-4]).first_column));
        set_node_loc(n, (yylsp[-6]).first_line, (yylsp[-6]).first_column);
        free((yyvsp[-4].lexeme));
        add_child(n, (yyvsp[-2].node));
        (yyval.node) = n;
    }
#line 2325 "y.tab.c"
    break;

  case 61: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 533 "jucompiler.y"
                               { (yyval.node) = NULL; }
#line 2331 "y.tab.c"
    break;

  case 62: /* ExprOrAssign: Assignment  */
#line 537 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2337 "y.tab.c"
    break;

  case 63: /* ExprOrAssign: Expr  */
#line 538 "jucompiler.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2343 "y.tab.c"
    break;

  case 64: /* Expr: Expr PLUS Expr  */
#line 542 "jucompiler.y"
                     { (yyval.node) = make_binary_at("Add", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2349 "y.tab.c"
    break;

  case 65: /* Expr: Expr MINUS Expr  */
#line 543 "jucompiler.y"
                      { (yyval.node) = make_binary_at("Sub", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2355 "y.tab.c"
    break;

  case 66: /* Expr: Expr STAR Expr  */
#line 544 "jucompiler.y"
                     { (yyval.node) = make_binary_at("Mul", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2361 "y.tab.c"
    break;

  case 67: /* Expr: Expr DIV Expr  */
#line 545 "jucompiler.y"
                    { (yyval.node) = make_binary_at("Div", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2367 "y.tab.c"
    break;

  case 68: /* Expr: Expr MOD Expr  */
#line 546 "jucompiler.y"
                    { (yyval.node) = make_binary_at("Mod", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2373 "y.tab.c"
    break;

  case 69: /* Expr: Expr AND Expr  */
#line 547 "jucompiler.y"
                    { (yyval.node) = make_binary_at("And", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2379 "y.tab.c"
    break;

  case 70: /* Expr: Expr OR Expr  */
#line 548 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Or", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2385 "y.tab.c"
    break;

  case 71: /* Expr: Expr XOR Expr  */
#line 549 "jucompiler.y"
                    { (yyval.node) = make_binary_at("Xor", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2391 "y.tab.c"
    break;

  case 72: /* Expr: Expr LSHIFT Expr  */
#line 550 "jucompiler.y"
                       { (yyval.node) = make_binary_at("Lshift", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2397 "y.tab.c"
    break;

  case 73: /* Expr: Expr RSHIFT Expr  */
#line 551 "jucompiler.y"
                       { (yyval.node) = make_binary_at("Rshift", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2403 "y.tab.c"
    break;

  case 74: /* Expr: Expr EQ Expr  */
#line 552 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Eq", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2409 "y.tab.c"
    break;

  case 75: /* Expr: Expr GE Expr  */
#line 553 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Ge", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2415 "y.tab.c"
    break;

  case 76: /* Expr: Expr GT Expr  */
#line 554 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Gt", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2421 "y.tab.c"
    break;

  case 77: /* Expr: Expr LE Expr  */
#line 555 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Le", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2427 "y.tab.c"
    break;

  case 78: /* Expr: Expr LT Expr  */
#line 556 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Lt", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2433 "y.tab.c"
    break;

  case 79: /* Expr: Expr NE Expr  */
#line 557 "jucompiler.y"
                   { (yyval.node) = make_binary_at("Ne", (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2439 "y.tab.c"
    break;

  case 80: /* Expr: MINUS Expr  */
#line 558 "jucompiler.y"
                              { (yyval.node) = make_unary_at("Minus", (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2445 "y.tab.c"
    break;

  case 81: /* Expr: PLUS Expr  */
#line 559 "jucompiler.y"
                            { (yyval.node) = make_unary_at("Plus", (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2451 "y.tab.c"
    break;

  case 82: /* Expr: NOT Expr  */
#line 560 "jucompiler.y"
               { (yyval.node) = make_unary_at("Not", (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2457 "y.tab.c"
    break;

  case 83: /* Expr: LPAR ExprOrAssign RPAR  */
#line 561 "jucompiler.y"
                             { (yyval.node) = (yyvsp[-1].node); }
#line 2463 "y.tab.c"
    break;

  case 84: /* Expr: LPAR error RPAR  */
#line 562 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 2469 "y.tab.c"
    break;

  case 85: /* Expr: MethodInvocation  */
#line 563 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2475 "y.tab.c"
    break;

  case 86: /* Expr: ParseArgs  */
#line 564 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2481 "y.tab.c"
    break;

  case 87: /* Expr: IDENTIFIER DOTLENGTH  */
#line 565 "jucompiler.y"
                           {
        Node *n = new_node("Length", NULL);
        add_child(n, make_identifier_at((yyvsp[-1].lexeme), (yylsp[-1]).first_line, (yylsp[-1]).first_column));
        set_node_loc(n, (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.node) = n;
        free((yyvsp[-1].lexeme));
    }
#line 2493 "y.tab.c"
    break;

  case 88: /* Expr: IDENTIFIER  */
#line 572 "jucompiler.y"
                 { (yyval.node) = make_leaf_at("Identifier", (yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column); free((yyvsp[0].lexeme)); }
#line 2499 "y.tab.c"
    break;

  case 89: /* Expr: NATURAL  */
#line 573 "jucompiler.y"
              { (yyval.node) = make_leaf_at("Natural", (yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column); free((yyvsp[0].lexeme)); }
#line 2505 "y.tab.c"
    break;

  case 90: /* Expr: DECIMAL  */
#line 574 "jucompiler.y"
              { (yyval.node) = make_leaf_at("Decimal", (yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column); free((yyvsp[0].lexeme)); }
#line 2511 "y.tab.c"
    break;

  case 91: /* Expr: BOOLLIT  */
#line 575 "jucompiler.y"
              { (yyval.node) = make_leaf_at("BoolLit", (yyvsp[0].lexeme), (yylsp[0]).first_line, (yylsp[0]).first_column); free((yyvsp[0].lexeme)); }
#line 2517 "y.tab.c"
    break;


#line 2521 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 578 "jucompiler.y"


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

Node *get_ast_root() {
    return ast_root;
}
    

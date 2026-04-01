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

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    extern int yylex(void);
    extern char *yytext;
    extern int line_number;
    extern int column_number;
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

#line 148 "y.tab.c"

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
#line 78 "jucompiler.y"

    typedef struct node Node;

#line 187 "y.tab.c"

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
#line 82 "jucompiler.y"

    char *lexeme;
    Node *node;

#line 316 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


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
  YYSYMBOL_StatementList = 71,             /* StatementList  */
  YYSYMBOL_ElseOpt = 72,                   /* ElseOpt  */
  YYSYMBOL_ExprOpt = 73,                   /* ExprOpt  */
  YYSYMBOL_PrintArg = 74,                  /* PrintArg  */
  YYSYMBOL_MethodInvocation = 75,          /* MethodInvocation  */
  YYSYMBOL_ExprListOpt = 76,               /* ExprListOpt  */
  YYSYMBOL_ExprList = 77,                  /* ExprList  */
  YYSYMBOL_Assignment = 78,                /* Assignment  */
  YYSYMBOL_ParseArgs = 79,                 /* ParseArgs  */
  YYSYMBOL_Expr = 80                       /* Expr  */
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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   464

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

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
       0,   116,   116,   126,   127,   131,   132,   133,   137,   146,
     164,   168,   172,   176,   177,   178,   179,   183,   190,   200,
     201,   205,   217,   223,   227,   235,   236,   240,   241,   245,
     266,   270,   274,   283,   291,   297,   302,   303,   304,   305,
     306,   311,   315,   316,   320,   321,   325,   326,   330,   331,
     335,   341,   345,   346,   350,   351,   355,   364,   370,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   403,   404,   405,   406
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
  "Statement", "StatementList", "ElseOpt", "ExprOpt", "PrintArg",
  "MethodInvocation", "ExprListOpt", "ExprList", "Assignment", "ParseArgs",
  "Expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-78)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-54)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -8,    10,    14,    -2,   -78,   -78,     4,    -5,    21,   -78,
     -78,   -78,   -78,   -78,   -78,    65,   -78,   -78,   -78,     7,
      40,    41,    20,    28,    47,    52,   -78,   -78,   -78,   125,
     125,   -18,    78,    83,    49,   -78,    51,    94,   -78,    56,
       2,   197,    71,    73,    75,    79,   -78,   -78,   -78,   108,
     -78,   -78,   -78,    68,    76,    80,   -78,   -78,   -78,   -78,
     -78,   197,   156,    -3,   -78,   -78,   -78,     6,   197,   197,
     197,    81,   -78,   -78,   -78,   316,   197,   197,   176,    35,
     121,   -78,   -78,   -78,   -78,   101,   316,    89,    97,   120,
     316,   -78,   107,   212,   -78,   -78,   -78,   -78,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   197,   197,   238,   264,   -78,   111,   316,   113,
     117,   -78,   -78,   -11,   125,   -78,   -78,   197,   -78,   -78,
     -78,   -78,   416,   128,   128,   128,   128,    46,   -78,   416,
     342,    46,    66,    66,   368,   392,   127,   127,   122,   -78,
     197,   165,   -78,   167,   316,   152,   -78,   -78,   290,   -78,
     -78,   127,   -78,   130,   -78,   -78
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     4,     0,     0,     0,     2,
       7,     3,     5,     6,    10,     0,    13,    14,    15,     0,
       0,     0,     0,     0,     0,    12,    26,     8,    16,    20,
      20,     0,     0,     0,     0,    19,     0,     0,     9,     0,
       0,    47,     0,     0,     0,     0,    43,    24,    39,     0,
      25,    27,    28,     0,     0,     0,    23,    18,    17,    11,
      41,     0,     0,    84,    85,    86,    87,     0,     0,     0,
       0,     0,    80,    81,    82,    46,     0,     0,     0,     0,
       0,    31,    36,    37,    38,    21,    56,     0,     0,    52,
      54,    83,     0,     0,    75,    77,    76,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,     0,    48,     0,
       0,    32,    42,     0,     0,    51,    50,     0,    79,    78,
      61,    62,    69,    70,    71,    72,    73,    60,    63,    74,
      65,    59,    67,    68,    66,    64,     0,     0,     0,    58,
       0,     0,    29,     0,    55,    45,    34,    40,     0,    30,
      22,     0,    33,     0,    44,    57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -14,   -78,   144,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -77,   -78,   -78,
     -78,   -78,   -32,   -78,   -78,   -30,   -28,   -45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     6,    11,    12,    13,    31,    33,    22,    34,
      35,    85,    27,    32,    50,    51,   123,    52,    80,   162,
      71,   117,    72,    88,    89,    73,    74,    75
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,    21,    54,   122,    55,     7,     1,    92,    37,    63,
      64,    65,    66,     3,     4,   151,    86,    90,    49,     8,
      91,    61,    93,    94,    95,    96,    61,    38,    45,     5,
      62,   114,   115,   118,   152,    62,   119,    15,   120,    67,
      14,    23,    68,    24,    25,    69,     9,    70,    53,    10,
      54,    26,    55,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   155,
     156,    98,    28,    99,    16,    17,    18,    19,    20,    39,
      29,    40,   154,   106,   164,    30,    56,    16,    17,    18,
      19,    98,    57,    99,    58,    41,    42,    59,    43,    44,
      45,    60,   105,   106,    76,   158,    77,   109,    78,    46,
     153,    81,    79,    82,    53,    53,    54,    54,    55,    55,
      47,    83,    39,    48,    40,    84,    97,   124,    39,    53,
      40,    54,   125,    55,    16,    17,    18,    19,    41,    42,
     126,    43,    44,    45,    41,    42,   127,    43,    44,    45,
     128,   150,    46,    98,   148,    99,   149,    87,    46,    63,
      64,    65,    66,   121,   105,   106,    48,   157,   159,   109,
     160,   161,    48,   165,    36,   110,   111,     0,    45,    63,
      64,    65,    66,   116,     0,     0,     0,     0,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,    45,   -53,
      63,    64,    65,    66,     0,     0,     0,     0,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,    68,     0,     0,    69,    98,    70,    99,
     100,   101,   102,     0,   103,     0,     0,   104,   105,   106,
     107,     0,   108,   109,     0,   129,     0,     0,     0,   110,
     111,   112,   113,    98,     0,    99,   100,   101,   102,     0,
     103,     0,     0,   104,   105,   106,   107,     0,   108,   109,
       0,   146,     0,     0,     0,   110,   111,   112,   113,    98,
       0,    99,   100,   101,   102,     0,   103,     0,     0,   104,
     105,   106,   107,     0,   108,   109,     0,   147,     0,     0,
       0,   110,   111,   112,   113,    98,     0,    99,   100,   101,
     102,     0,   103,     0,     0,   104,   105,   106,   107,     0,
     108,   109,     0,     0,   163,     0,     0,   110,   111,   112,
     113,    98,     0,    99,   100,   101,   102,     0,   103,     0,
       0,   104,   105,   106,   107,     0,   108,   109,     0,     0,
       0,     0,     0,   110,   111,   112,   113,    98,     0,    99,
     100,   101,   102,     0,   103,     0,     0,   104,   105,   106,
     107,     0,     0,   109,     0,     0,     0,     0,     0,   110,
     111,   112,   113,    98,     0,    99,   100,   101,   102,     0,
     103,     0,     0,   104,   105,   106,   107,     0,     0,   109,
       0,     0,     0,     0,     0,   110,   111,    98,   113,    99,
     100,   101,   102,     0,   103,     0,     0,   104,   105,   106,
     107,     0,     0,   109,     0,     0,     0,     0,     0,   110,
     111,    98,     0,    99,     0,   101,   102,     0,   103,     0,
       0,   104,   105,   106,     0,     0,     0,   109,     0,     0,
       0,     0,     0,   110,   111
};

static const yytype_int16 yycheck[] =
{
      32,    15,    32,    80,    32,     1,    14,     1,    26,     3,
       4,     5,     6,     3,     0,    26,    61,    62,    32,    15,
      23,    24,    67,    68,    69,    70,    24,    45,    22,    31,
      33,    76,    77,    78,    45,    33,     1,    16,     3,    33,
      45,    34,    36,     3,     3,    39,    42,    41,    80,    45,
      80,    31,    80,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   146,
     147,    25,    44,    27,     9,    10,    11,    12,    13,     1,
      33,     3,   127,    37,   161,    33,     3,     9,    10,    11,
      12,    25,    43,    27,    43,    17,    18,     3,    20,    21,
      22,    45,    36,    37,    33,   150,    33,    41,    33,    31,
     124,     3,    33,    45,   146,   147,   146,   147,   146,   147,
      42,    45,     1,    45,     3,    45,    45,    26,     1,   161,
       3,   161,    43,   161,     9,    10,    11,    12,    17,    18,
      43,    20,    21,    22,    17,    18,    26,    20,    21,    22,
      43,    34,    31,    25,    43,    27,    43,     1,    31,     3,
       4,     5,     6,    42,    36,    37,    45,    45,     3,    41,
       3,    19,    45,    43,    30,    47,    48,    -1,    22,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    36,    -1,    -1,    39,    -1,    41,    22,    43,
       3,     4,     5,     6,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    36,    -1,    -1,    39,    -1,    41,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    36,    -1,    -1,    39,    25,    41,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    25,    -1,    27,    28,    29,    30,    -1,
      32,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    -1,    47,    48,    49,    50,    25,
      -1,    27,    28,    29,    30,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      -1,    47,    48,    49,    50,    25,    -1,    27,    28,    29,
      30,    -1,    32,    -1,    -1,    35,    36,    37,    38,    -1,
      40,    41,    -1,    -1,    44,    -1,    -1,    47,    48,    49,
      50,    25,    -1,    27,    28,    29,    30,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    50,    25,    -1,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    25,    -1,    27,    28,    29,    30,    -1,
      32,    -1,    -1,    35,    36,    37,    38,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    47,    48,    25,    50,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    25,    -1,    27,    -1,    29,    30,    -1,    32,    -1,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    14,    54,     3,     0,    31,    55,     1,    15,    42,
      45,    56,    57,    58,    45,    16,     9,    10,    11,    12,
      13,    60,    61,    34,     3,     3,    31,    65,    44,    33,
      33,    59,    66,    60,    62,    63,    62,    26,    45,     1,
       3,    17,    18,    20,    21,    22,    31,    42,    45,    60,
      67,    68,    70,    75,    78,    79,     3,    43,    43,     3,
      45,    24,    33,     3,     4,     5,     6,    33,    36,    39,
      41,    73,    75,    78,    79,    80,    33,    33,    33,    33,
      71,     3,    45,    45,    45,    64,    80,     1,    76,    77,
      80,    23,     1,    80,    80,    80,    80,    45,    25,    27,
      28,    29,    30,    32,    35,    36,    37,    38,    40,    41,
      47,    48,    49,    50,    80,    80,     7,    74,    80,     1,
       3,    42,    70,    69,    26,    43,    43,    26,    43,    43,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    43,    43,    43,    43,
      34,    26,    45,    60,    80,    70,    70,    45,    80,     3,
       3,    19,    72,    44,    70,    43
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    55,    56,    56,    56,    57,    58,
      58,    59,    59,    60,    60,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    65,    66,    66,    67,    67,    68,
      69,    69,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    74,
      75,    75,    76,    76,    77,    77,    78,    79,    79,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     0,     1,     1,     1,     4,     6,
       2,     3,     0,     1,     1,     1,     3,     5,     5,     1,
       0,     3,     4,     0,     3,     2,     0,     1,     1,     4,
       3,     0,     3,     6,     5,     3,     2,     2,     2,     1,
       5,     2,     2,     0,     2,     0,     1,     0,     1,     1,
       4,     4,     1,     0,     1,     3,     3,     7,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     3,     3,
       1,     1,     1,     2,     1,     1,     1,     1
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: CLASS IDENTIFIER LBRACE ProgramMembers RBRACE  */
#line 116 "jucompiler.y"
                                                    {
        Node *n = new_node("Program", NULL);
        add_child(n, new_node("Identifier", (yyvsp[-3].lexeme)));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
        if (flag_t && syntax_errors == 0 && !flag_e2) print_ast(n, 0);
    }
#line 1552 "y.tab.c"
    break;

  case 3: /* ProgramMembers: ProgramMembers ProgramMember  */
#line 126 "jucompiler.y"
                                   { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1558 "y.tab.c"
    break;

  case 4: /* ProgramMembers: %empty  */
#line 127 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1564 "y.tab.c"
    break;

  case 5: /* ProgramMember: MethodDecl  */
#line 131 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1570 "y.tab.c"
    break;

  case 6: /* ProgramMember: FieldDecl  */
#line 132 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1576 "y.tab.c"
    break;

  case 7: /* ProgramMember: SEMICOLON  */
#line 133 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 1582 "y.tab.c"
    break;

  case 8: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 137 "jucompiler.y"
                                            {
        Node *n = new_node("MethodDecl", NULL);
        add_child(n, (yyvsp[-1].node));
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1593 "y.tab.c"
    break;

  case 9: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldDeclTail SEMICOLON  */
#line 146 "jucompiler.y"
                                                            {
        Node *head = NULL;
        Node *t = (yyvsp[-3].node);
        Node *id = new_node("Identifier", (yyvsp[-2].lexeme));
        Node *fd = new_node("FieldDecl", NULL);
        add_child(fd, make_type_node(t));
        add_child(fd, id);
        head = append_sibling(head, fd);
        Node *cur = (yyvsp[-1].node);
        while (cur) {
            Node *fd2 = new_node("FieldDecl", NULL);
            add_child(fd2, make_type_node(t));
            add_child(fd2, cur);
            head = append_sibling(head, fd2);
            cur = cur->sibling;
        }
        (yyval.node) = head;
    }
#line 1616 "y.tab.c"
    break;

  case 10: /* FieldDecl: error SEMICOLON  */
#line 164 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 1622 "y.tab.c"
    break;

  case 11: /* FieldDeclTail: FieldDeclTail COMMA IDENTIFIER  */
#line 168 "jucompiler.y"
                                     {
        Node *id = new_node("Identifier", (yyvsp[0].lexeme));
        (yyval.node) = append_sibling((yyvsp[-2].node), id);
    }
#line 1631 "y.tab.c"
    break;

  case 12: /* FieldDeclTail: %empty  */
#line 172 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1637 "y.tab.c"
    break;

  case 13: /* Type: BOOL  */
#line 176 "jucompiler.y"
           { (yyval.node) = new_node("Bool", NULL); }
#line 1643 "y.tab.c"
    break;

  case 14: /* Type: INT  */
#line 177 "jucompiler.y"
          { (yyval.node) = new_node("Int", NULL); }
#line 1649 "y.tab.c"
    break;

  case 15: /* Type: DOUBLE  */
#line 178 "jucompiler.y"
             { (yyval.node) = new_node("Double", NULL); }
#line 1655 "y.tab.c"
    break;

  case 16: /* Type: STRING LSQ RSQ  */
#line 179 "jucompiler.y"
                     { (yyval.node) = new_node("StringArray", NULL); }
#line 1661 "y.tab.c"
    break;

  case 17: /* MethodHeader: Type IDENTIFIER LPAR FormalParamsOpt RPAR  */
#line 183 "jucompiler.y"
                                                {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, (yyvsp[-4].node));
        add_child(n, new_node("Identifier", (yyvsp[-3].lexeme)));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1673 "y.tab.c"
    break;

  case 18: /* MethodHeader: VOID IDENTIFIER LPAR FormalParamsOpt RPAR  */
#line 190 "jucompiler.y"
                                                {
        Node *n = new_node("MethodHeader", NULL);
        add_child(n, new_node("Void", NULL));
        add_child(n, new_node("Identifier", (yyvsp[-3].lexeme)));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1685 "y.tab.c"
    break;

  case 19: /* FormalParamsOpt: FormalParams  */
#line 200 "jucompiler.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1691 "y.tab.c"
    break;

  case 20: /* FormalParamsOpt: %empty  */
#line 201 "jucompiler.y"
                  { (yyval.node) = new_node("MethodParams", NULL); }
#line 1697 "y.tab.c"
    break;

  case 21: /* FormalParams: Type IDENTIFIER FormalParamsTail  */
#line 205 "jucompiler.y"
                                       {
        Node *n = new_node("MethodParams", NULL);
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, (yyvsp[-2].node));
        add_child(pd, new_node("Identifier", (yyvsp[-1].lexeme)));
        add_child(n, pd);
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1711 "y.tab.c"
    break;

  case 22: /* FormalParamsTail: FormalParamsTail COMMA Type IDENTIFIER  */
#line 217 "jucompiler.y"
                                             {
        Node *pd = new_node("ParamDecl", NULL);
        add_child(pd, (yyvsp[-1].node));
        add_child(pd, new_node("Identifier", (yyvsp[0].lexeme)));
        (yyval.node) = append_sibling((yyvsp[-3].node), pd);
    }
#line 1722 "y.tab.c"
    break;

  case 23: /* FormalParamsTail: %empty  */
#line 223 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1728 "y.tab.c"
    break;

  case 24: /* MethodBody: LBRACE MethodBodyItems RBRACE  */
#line 227 "jucompiler.y"
                                    {
        Node *n = new_node("MethodBody", NULL);
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1738 "y.tab.c"
    break;

  case 25: /* MethodBodyItems: MethodBodyItems MethodBodyItem  */
#line 235 "jucompiler.y"
                                     { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1744 "y.tab.c"
    break;

  case 26: /* MethodBodyItems: %empty  */
#line 236 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1750 "y.tab.c"
    break;

  case 27: /* MethodBodyItem: VarDecl  */
#line 240 "jucompiler.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1756 "y.tab.c"
    break;

  case 28: /* MethodBodyItem: Statement  */
#line 241 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1762 "y.tab.c"
    break;

  case 29: /* VarDecl: Type IDENTIFIER VarDeclTail SEMICOLON  */
#line 245 "jucompiler.y"
                                            {
        Node *head = NULL;
        Node *t = (yyvsp[-3].node);
        Node *id = new_node("Identifier", (yyvsp[-2].lexeme));
        Node *vd = new_node("VarDecl", NULL);
        add_child(vd, make_type_node(t));
        add_child(vd, id);
        head = append_sibling(head, vd);
        Node *cur = (yyvsp[-1].node);
        while (cur) {
            Node *vd2 = new_node("VarDecl", NULL);
            add_child(vd2, make_type_node(t));
            add_child(vd2, cur);
            head = append_sibling(head, vd2);
            cur = cur->sibling;
        }
        (yyval.node) = head;
    }
#line 1785 "y.tab.c"
    break;

  case 30: /* VarDeclTail: VarDeclTail COMMA IDENTIFIER  */
#line 266 "jucompiler.y"
                                   {
        Node *id = new_node("Identifier", (yyvsp[0].lexeme));
        (yyval.node) = append_sibling((yyvsp[-2].node), id);
    }
#line 1794 "y.tab.c"
    break;

  case 31: /* VarDeclTail: %empty  */
#line 270 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1800 "y.tab.c"
    break;

  case 32: /* Statement: LBRACE StatementList RBRACE  */
#line 274 "jucompiler.y"
                                  {
        if (!(yyvsp[-1].node)) (yyval.node) = new_node("Block", NULL);
        else if (!(yyvsp[-1].node)->sibling) (yyval.node) = (yyvsp[-1].node);
        else {
            Node *b = new_node("Block", NULL);
            add_child(b, (yyvsp[-1].node));
            (yyval.node) = b;
        }
    }
#line 1814 "y.tab.c"
    break;

  case 33: /* Statement: IF LPAR Expr RPAR Statement ElseOpt  */
#line 283 "jucompiler.y"
                                          {
        Node *n = new_node("If", NULL);
        add_child(n, (yyvsp[-3].node));
        add_child(n, (yyvsp[-1].node));
        if ((yyvsp[0].node)) add_child(n, (yyvsp[0].node));
        else add_child(n, new_node("Block", NULL));
        (yyval.node) = n;
    }
#line 1827 "y.tab.c"
    break;

  case 34: /* Statement: WHILE LPAR Expr RPAR Statement  */
#line 291 "jucompiler.y"
                                     {
        Node *n = new_node("While", NULL);
        add_child(n, (yyvsp[-2].node));
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1838 "y.tab.c"
    break;

  case 35: /* Statement: RETURN ExprOpt SEMICOLON  */
#line 297 "jucompiler.y"
                               {
        Node *n = new_node("Return", NULL);
        if ((yyvsp[-1].node)) add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1848 "y.tab.c"
    break;

  case 36: /* Statement: MethodInvocation SEMICOLON  */
#line 302 "jucompiler.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1854 "y.tab.c"
    break;

  case 37: /* Statement: Assignment SEMICOLON  */
#line 303 "jucompiler.y"
                           { (yyval.node) = (yyvsp[-1].node); }
#line 1860 "y.tab.c"
    break;

  case 38: /* Statement: ParseArgs SEMICOLON  */
#line 304 "jucompiler.y"
                          { (yyval.node) = (yyvsp[-1].node); }
#line 1866 "y.tab.c"
    break;

  case 39: /* Statement: SEMICOLON  */
#line 305 "jucompiler.y"
                { (yyval.node) = NULL; }
#line 1872 "y.tab.c"
    break;

  case 40: /* Statement: PRINT LPAR PrintArg RPAR SEMICOLON  */
#line 306 "jucompiler.y"
                                         {
        Node *n = new_node("Print", NULL);
        add_child(n, (yyvsp[-2].node));
        (yyval.node) = n;
    }
#line 1882 "y.tab.c"
    break;

  case 41: /* Statement: error SEMICOLON  */
#line 311 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 1888 "y.tab.c"
    break;

  case 42: /* StatementList: StatementList Statement  */
#line 315 "jucompiler.y"
                              { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1894 "y.tab.c"
    break;

  case 43: /* StatementList: %empty  */
#line 316 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1900 "y.tab.c"
    break;

  case 44: /* ElseOpt: ELSE Statement  */
#line 320 "jucompiler.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1906 "y.tab.c"
    break;

  case 45: /* ElseOpt: %empty  */
#line 321 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1912 "y.tab.c"
    break;

  case 46: /* ExprOpt: Expr  */
#line 325 "jucompiler.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1918 "y.tab.c"
    break;

  case 47: /* ExprOpt: %empty  */
#line 326 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1924 "y.tab.c"
    break;

  case 48: /* PrintArg: Expr  */
#line 330 "jucompiler.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1930 "y.tab.c"
    break;

  case 49: /* PrintArg: STRLIT  */
#line 331 "jucompiler.y"
             { (yyval.node) = new_node("StrLit", (yyvsp[0].lexeme)); }
#line 1936 "y.tab.c"
    break;

  case 50: /* MethodInvocation: IDENTIFIER LPAR ExprListOpt RPAR  */
#line 335 "jucompiler.y"
                                       {
        Node *n = new_node("Call", NULL);
        add_child(n, new_node("Identifier", (yyvsp[-3].lexeme)));
        add_child(n, (yyvsp[-1].node));
        (yyval.node) = n;
    }
#line 1947 "y.tab.c"
    break;

  case 51: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 341 "jucompiler.y"
                                 { (yyval.node) = NULL; }
#line 1953 "y.tab.c"
    break;

  case 52: /* ExprListOpt: ExprList  */
#line 345 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1959 "y.tab.c"
    break;

  case 53: /* ExprListOpt: %empty  */
#line 346 "jucompiler.y"
                  { (yyval.node) = NULL; }
#line 1965 "y.tab.c"
    break;

  case 54: /* ExprList: Expr  */
#line 350 "jucompiler.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1971 "y.tab.c"
    break;

  case 55: /* ExprList: ExprList COMMA Expr  */
#line 351 "jucompiler.y"
                          { (yyval.node) = append_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1977 "y.tab.c"
    break;

  case 56: /* Assignment: IDENTIFIER ASSIGN Expr  */
#line 355 "jucompiler.y"
                             {
        Node *n = new_node("Assign", NULL);
        add_child(n, new_node("Identifier", (yyvsp[-2].lexeme)));
        add_child(n, (yyvsp[0].node));
        (yyval.node) = n;
    }
#line 1988 "y.tab.c"
    break;

  case 57: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR  */
#line 364 "jucompiler.y"
                                                 {
        Node *n = new_node("ParseArgs", NULL);
        add_child(n, new_node("Identifier", (yyvsp[-4].lexeme)));
        add_child(n, (yyvsp[-2].node));
        (yyval.node) = n;
    }
#line 1999 "y.tab.c"
    break;

  case 58: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 370 "jucompiler.y"
                               { (yyval.node) = NULL; }
#line 2005 "y.tab.c"
    break;

  case 59: /* Expr: Expr PLUS Expr  */
#line 374 "jucompiler.y"
                     { (yyval.node) = make_binary("Add", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2011 "y.tab.c"
    break;

  case 60: /* Expr: Expr MINUS Expr  */
#line 375 "jucompiler.y"
                      { (yyval.node) = make_binary("Sub", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2017 "y.tab.c"
    break;

  case 61: /* Expr: Expr STAR Expr  */
#line 376 "jucompiler.y"
                     { (yyval.node) = make_binary("Mul", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2023 "y.tab.c"
    break;

  case 62: /* Expr: Expr DIV Expr  */
#line 377 "jucompiler.y"
                    { (yyval.node) = make_binary("Div", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2029 "y.tab.c"
    break;

  case 63: /* Expr: Expr MOD Expr  */
#line 378 "jucompiler.y"
                    { (yyval.node) = make_binary("Mod", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2035 "y.tab.c"
    break;

  case 64: /* Expr: Expr AND Expr  */
#line 379 "jucompiler.y"
                    { (yyval.node) = make_binary("And", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2041 "y.tab.c"
    break;

  case 65: /* Expr: Expr OR Expr  */
#line 380 "jucompiler.y"
                   { (yyval.node) = make_binary("Or", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2047 "y.tab.c"
    break;

  case 66: /* Expr: Expr XOR Expr  */
#line 381 "jucompiler.y"
                    { (yyval.node) = make_binary("Xor", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2053 "y.tab.c"
    break;

  case 67: /* Expr: Expr LSHIFT Expr  */
#line 382 "jucompiler.y"
                       { (yyval.node) = make_binary("Lshift", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2059 "y.tab.c"
    break;

  case 68: /* Expr: Expr RSHIFT Expr  */
#line 383 "jucompiler.y"
                       { (yyval.node) = make_binary("Rshift", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2065 "y.tab.c"
    break;

  case 69: /* Expr: Expr EQ Expr  */
#line 384 "jucompiler.y"
                   { (yyval.node) = make_binary("Eq", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2071 "y.tab.c"
    break;

  case 70: /* Expr: Expr GE Expr  */
#line 385 "jucompiler.y"
                   { (yyval.node) = make_binary("Ge", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2077 "y.tab.c"
    break;

  case 71: /* Expr: Expr GT Expr  */
#line 386 "jucompiler.y"
                   { (yyval.node) = make_binary("Gt", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2083 "y.tab.c"
    break;

  case 72: /* Expr: Expr LE Expr  */
#line 387 "jucompiler.y"
                   { (yyval.node) = make_binary("Le", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2089 "y.tab.c"
    break;

  case 73: /* Expr: Expr LT Expr  */
#line 388 "jucompiler.y"
                   { (yyval.node) = make_binary("Lt", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2095 "y.tab.c"
    break;

  case 74: /* Expr: Expr NE Expr  */
#line 389 "jucompiler.y"
                   { (yyval.node) = make_binary("Ne", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2101 "y.tab.c"
    break;

  case 75: /* Expr: MINUS Expr  */
#line 390 "jucompiler.y"
                              { (yyval.node) = make_unary("Minus", (yyvsp[0].node)); }
#line 2107 "y.tab.c"
    break;

  case 76: /* Expr: PLUS Expr  */
#line 391 "jucompiler.y"
                            { (yyval.node) = make_unary("Plus", (yyvsp[0].node)); }
#line 2113 "y.tab.c"
    break;

  case 77: /* Expr: NOT Expr  */
#line 392 "jucompiler.y"
               { (yyval.node) = make_unary("Not", (yyvsp[0].node)); }
#line 2119 "y.tab.c"
    break;

  case 78: /* Expr: LPAR Expr RPAR  */
#line 393 "jucompiler.y"
                     { (yyval.node) = (yyvsp[-1].node); }
#line 2125 "y.tab.c"
    break;

  case 79: /* Expr: LPAR error RPAR  */
#line 394 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 2131 "y.tab.c"
    break;

  case 80: /* Expr: MethodInvocation  */
#line 395 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2137 "y.tab.c"
    break;

  case 81: /* Expr: Assignment  */
#line 396 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2143 "y.tab.c"
    break;

  case 82: /* Expr: ParseArgs  */
#line 397 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2149 "y.tab.c"
    break;

  case 83: /* Expr: IDENTIFIER DOTLENGTH  */
#line 398 "jucompiler.y"
                           {
        Node *n = new_node("Length", NULL);
        add_child(n, new_node("Identifier", (yyvsp[-1].lexeme)));
        (yyval.node) = n;
    }
#line 2159 "y.tab.c"
    break;

  case 84: /* Expr: IDENTIFIER  */
#line 403 "jucompiler.y"
                 { (yyval.node) = new_node("Identifier", (yyvsp[0].lexeme)); }
#line 2165 "y.tab.c"
    break;

  case 85: /* Expr: NATURAL  */
#line 404 "jucompiler.y"
              { (yyval.node) = new_node("Natural", (yyvsp[0].lexeme)); }
#line 2171 "y.tab.c"
    break;

  case 86: /* Expr: DECIMAL  */
#line 405 "jucompiler.y"
              { (yyval.node) = new_node("Decimal", (yyvsp[0].lexeme)); }
#line 2177 "y.tab.c"
    break;

  case 87: /* Expr: BOOLLIT  */
#line 406 "jucompiler.y"
              { (yyval.node) = new_node("BoolLit", (yyvsp[0].lexeme)); }
#line 2183 "y.tab.c"
    break;


#line 2187 "y.tab.c"

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 409 "jucompiler.y"


void yyerror(const char *s) {
    syntax_errors++;
    printf("Line %d, col %d: %s: %s\n", line_number, column_number, s, yytext);
}

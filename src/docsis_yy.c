/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_IDENTIFIER = 258,
     T_INTEGER = 259,
     T_IDENT_COS = 260,
     T_IDENT_BPI = 261,
     T_IDENT_SNMPW = 262,
     T_IDENT_SNMPSET = 263,
     T_IDENT_GENERIC = 264,
     T_IDENT_CVC = 265,
     T_ETHERMASK = 266,
     T_LABEL_OID = 267,
     T_SUBMGT_FILTERS = 268,
     T_IP = 269,
     T_MAC = 270,
     T_MAIN = 271,
     T_STRING = 272,
     T_HEX_STRING = 273,
     T_TIMETICKS = 274,
     T_ASNTYPE_INT = 275,
     T_ASNTYPE_UINT = 276,
     T_ASNTYPE_GAUGE = 277,
     T_ASNTYPE_COUNTER = 278,
     T_ASNTYPE_TIMETICKS = 279,
     T_ASNTYPE_IP = 280,
     T_ASNTYPE_OBJID = 281,
     T_ASNTYPE_STRING = 282,
     T_ASNTYPE_HEXSTR = 283,
     T_ASNTYPE_DECSTR = 284,
     T_ASNTYPE_BITSTR = 285,
     T_ASNTYPE_BIGINT = 286,
     T_ASNTYPE_UBIGINT = 287,
     T_ASNTYPE_FLOAT = 288,
     T_ASNTYPE_DOUBLE = 289,
     T_TLV_CODE = 290,
     T_TLV_LENGTH = 291,
     T_TLV_VALUE = 292,
     T_TLV_STR_VALUE = 293,
     T_TLV_STRZERO_VALUE = 294
   };
#endif
#define T_IDENTIFIER 258
#define T_INTEGER 259
#define T_IDENT_COS 260
#define T_IDENT_BPI 261
#define T_IDENT_SNMPW 262
#define T_IDENT_SNMPSET 263
#define T_IDENT_GENERIC 264
#define T_IDENT_CVC 265
#define T_ETHERMASK 266
#define T_LABEL_OID 267
#define T_SUBMGT_FILTERS 268
#define T_IP 269
#define T_MAC 270
#define T_MAIN 271
#define T_STRING 272
#define T_HEX_STRING 273
#define T_TIMETICKS 274
#define T_ASNTYPE_INT 275
#define T_ASNTYPE_UINT 276
#define T_ASNTYPE_GAUGE 277
#define T_ASNTYPE_COUNTER 278
#define T_ASNTYPE_TIMETICKS 279
#define T_ASNTYPE_IP 280
#define T_ASNTYPE_OBJID 281
#define T_ASNTYPE_STRING 282
#define T_ASNTYPE_HEXSTR 283
#define T_ASNTYPE_DECSTR 284
#define T_ASNTYPE_BITSTR 285
#define T_ASNTYPE_BIGINT 286
#define T_ASNTYPE_UBIGINT 287
#define T_ASNTYPE_FLOAT 288
#define T_ASNTYPE_DOUBLE 289
#define T_TLV_CODE 290
#define T_TLV_LENGTH 291
#define T_TLV_VALUE 292
#define T_TLV_STR_VALUE 293
#define T_TLV_STRZERO_VALUE 294




/* Copy the first part of user declarations.  */
#line 23 "docsis_yy.y"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "docsis.h"
#include "docsis_encode.h"
#include "docsis_snmp.h"
#if YYTEXT_POINTER
extern char *yytext;
#else
extern char yytext[];
#endif

extern unsigned int line; 	/* current line number, defined in a.l */
extern struct tlv *global_tlvtree_head; /* Global list of all config TLVs */
extern symbol_type *global_symtable;
extern FILE *yyin;
extern char prog_name[255];

struct tlv *_my_tlvtree_head;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 46 "docsis_yy.y"
typedef union YYSTYPE { 	/* Token types */
	int intval;			/* For integers */
	unsigned int uintval;		/* For unsigned integers */
	symbol_type *symptr; 		/* For token identifiers */
	char *strval;		 	/* For strings */
	unsigned char *ustrval; 	/* For any other types */
	unsigned int ip;		/* For IP Addresses */
	struct tlv_list *tlvlist; 	/* For for struct tlvlist pointers */
	struct tlv *tlvptr;		/* For struct tlv pointers; */
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 187 "docsis_yy.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 199 "docsis_yy.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   87

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  5
/* YYNRULES -- Number of rules. */
#define YYNRULES  29
/* YYNRULES -- Number of states. */
#define YYNSTATES  81

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,    11,    13,    16,    18,    23,    27,
      31,    35,    39,    43,    47,    51,    55,    59,    64,    70,
      76,    82,    88,    94,   100,   106,   112,   118,   127,   134
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      44,     0,    -1,    16,    40,    45,    41,    -1,    45,    47,
      -1,    47,    -1,    45,    46,    -1,    46,    -1,     3,    40,
      45,    41,    -1,     3,     4,    42,    -1,     3,    17,    42,
      -1,     3,    18,    42,    -1,     3,    13,    42,    -1,     3,
      14,    42,    -1,     3,    15,    42,    -1,     3,    11,    42,
      -1,     3,    12,    42,    -1,    10,    17,    42,    -1,     7,
      12,     4,    42,    -1,     8,    12,    20,     4,    42,    -1,
       8,    12,    22,     4,    42,    -1,     8,    12,    21,     4,
      42,    -1,     8,    12,    23,     4,    42,    -1,     8,    12,
      25,    14,    42,    -1,     8,    12,    27,    17,    42,    -1,
       8,    12,    28,    18,    42,    -1,     8,    12,    26,    12,
      42,    -1,     8,    12,    24,     4,    42,    -1,     9,    35,
       4,    36,     4,    37,    18,    42,    -1,     9,    35,     4,
      38,    17,    42,    -1,     9,    35,     4,    39,    17,    42,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,   133,   133,   138,   139,   140,   142,   146,   150,   152,
     154,   156,   158,   160,   162,   164,   166,   168,   170,   172,
     174,   176,   178,   180,   182,   184,   186,   188,   190,   192
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_INTEGER", "T_IDENT_COS",
  "T_IDENT_BPI", "T_IDENT_SNMPW", "T_IDENT_SNMPSET", "T_IDENT_GENERIC",
  "T_IDENT_CVC", "T_ETHERMASK", "T_LABEL_OID", "T_SUBMGT_FILTERS", "T_IP",
  "T_MAC", "T_MAIN", "T_STRING", "T_HEX_STRING", "T_TIMETICKS",
  "T_ASNTYPE_INT", "T_ASNTYPE_UINT", "T_ASNTYPE_GAUGE",
  "T_ASNTYPE_COUNTER", "T_ASNTYPE_TIMETICKS", "T_ASNTYPE_IP",
  "T_ASNTYPE_OBJID", "T_ASNTYPE_STRING", "T_ASNTYPE_HEXSTR",
  "T_ASNTYPE_DECSTR", "T_ASNTYPE_BITSTR", "T_ASNTYPE_BIGINT",
  "T_ASNTYPE_UBIGINT", "T_ASNTYPE_FLOAT", "T_ASNTYPE_DOUBLE",
  "T_TLV_CODE", "T_TLV_LENGTH", "T_TLV_VALUE", "T_TLV_STR_VALUE",
  "T_TLV_STRZERO_VALUE", "'{'", "'}'", "';'", "$accept", "main_stmt",
  "assignment_list", "subsettings_stmt", "assignment_stmt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    43,    44,    45,    45,    45,    45,    46,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     2,     1,     2,     1,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     8,     6,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,     0,
       0,     6,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     5,     3,     8,
      14,    15,    11,    12,    13,     9,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       7,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,    20,    19,    21,    26,    22,
      25,    23,    24,     0,     0,     0,     0,    28,    29,     0,
      27
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,    10,    11,    12
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -32
static const yysigned_char yypact[] =
{
     -13,   -31,    10,    23,   -32,     7,     4,    11,   -18,    18,
      -3,   -32,   -32,   -15,    -1,     0,     1,     2,     3,    15,
      16,    23,    35,    28,    36,    17,   -32,   -32,   -32,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,     5,    19,    56,
      58,    59,    60,    61,    52,    55,    51,    53,    -2,   -32,
     -32,   -32,    27,    30,    31,    32,    33,    34,    37,    38,
      39,    66,    65,    67,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,   -32,   -32,    40,    41,    43,    68,   -32,   -32,    45,
     -32
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -32,   -32,    57,    -9,    -8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
       5,    27,    28,     1,     6,     7,     8,     9,     5,     3,
       4,    13,     6,     7,     8,     9,    22,    24,    14,    15,
      16,    17,    18,    23,    19,    20,     5,    29,    27,    28,
       6,     7,     8,     9,    61,    25,    62,    63,    26,    38,
      48,    30,    31,    32,    33,    34,    50,    21,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    35,    36,    49,
      52,    51,    53,    54,    55,    56,    57,    58,    59,    64,
      73,    60,    65,    66,    67,    68,    69,    76,    37,    70,
      71,    72,    74,    77,    75,    78,    79,    80
};

static const unsigned char yycheck[] =
{
       3,    10,    10,    16,     7,     8,     9,    10,     3,    40,
       0,     4,     7,     8,     9,    10,    12,    35,    11,    12,
      13,    14,    15,    12,    17,    18,     3,    42,    37,    37,
       7,     8,     9,    10,    36,    17,    38,    39,    41,     4,
       4,    42,    42,    42,    42,    42,    41,    40,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    42,    42,    42,
       4,    42,     4,     4,     4,     4,    14,    12,    17,    42,
       4,    18,    42,    42,    42,    42,    42,    37,    21,    42,
      42,    42,    17,    42,    17,    42,    18,    42
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    16,    44,    40,     0,     3,     7,     8,     9,    10,
      45,    46,    47,     4,    11,    12,    13,    14,    15,    17,
      18,    40,    12,    12,    35,    17,    41,    46,    47,    42,
      42,    42,    42,    42,    42,    42,    42,    45,     4,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     4,    42,
      41,    42,     4,     4,     4,     4,     4,    14,    12,    17,
      18,    36,    38,    39,    42,    42,    42,    42,    42,    42,
      42,    42,    42,     4,    17,    17,    37,    42,    42,    18,
      42
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 133 "docsis_yy.y"
    {
			_my_tlvtree_head = yyvsp[-1].tlvptr;
			_my_tlvtree_head->parent = NULL; }
    break;

  case 3:
#line 138 "docsis_yy.y"
    { yyval.tlvptr = add_tlv_sibling (yyvsp[-1].tlvptr,yyvsp[0].tlvptr); }
    break;

  case 4:
#line 139 "docsis_yy.y"
    {  yyval.tlvptr=add_tlv_sibling(NULL,yyvsp[0].tlvptr); }
    break;

  case 5:
#line 140 "docsis_yy.y"
    {
			yyval.tlvptr = merge_tlvlist(yyvsp[-1].tlvptr,yyvsp[0].tlvptr);  }
    break;

  case 6:
#line 142 "docsis_yy.y"
    {
			yyval.tlvptr = merge_tlvlist (NULL, yyvsp[0].tlvptr); }
    break;

  case 7:
#line 146 "docsis_yy.y"
    {
			yyval.tlvptr = assemble_tlv_in_parent ( yyvsp[-3].symptr, yyvsp[-1].tlvptr ); }
    break;

  case 8:
#line 150 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].uintval);}
    break;

  case 9:
#line 152 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 10:
#line 154 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 11:
#line 156 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 12:
#line 158 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 13:
#line 160 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 14:
#line 162 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 15:
#line 164 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 16:
#line 166 "docsis_yy.y"
    {
			yyval.tlvptr = create_external_file_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 17:
#line 168 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpw_tlv ( yyvsp[-3].symptr, yyvsp[-2].strval, (union t_val *) &yyvsp[-1].uintval ); }
    break;

  case 18:
#line 170 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'i',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 19:
#line 172 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'g',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 20:
#line 174 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'u',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 21:
#line 176 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'c',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 22:
#line 178 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'a',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 23:
#line 180 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'s',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 24:
#line 182 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'x',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 25:
#line 184 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'o',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 26:
#line 186 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'t',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 27:
#line 188 "docsis_yy.y"
    {
			yyval.tlvptr = create_generic_tlv(yyvsp[-7].symptr,yyvsp[-5].uintval,yyvsp[-3].uintval, (union t_val *)&yyvsp[-1].strval); }
    break;

  case 28:
#line 190 "docsis_yy.y"
    {
			yyval.tlvptr = create_generic_str_tlv(yyvsp[-5].symptr,yyvsp[-3].uintval, (union t_val *)&yyvsp[-1].strval); }
    break;

  case 29:
#line 192 "docsis_yy.y"
    {
			yyval.tlvptr = create_generic_strzero_tlv(yyvsp[-5].symptr,yyvsp[-3].uintval, (union t_val *)&yyvsp[-1].strval); }
    break;


    }

/* Line 991 of yacc.c.  */
#line 1306 "docsis_yy.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__) \
    && !defined __cplusplus
  __attribute__ ((__unused__))
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 195 "docsis_yy.y"


int yyerror(char *s) {
	fprintf(stderr, "%d:%s token %s\n",line,s,yytext );
	return 0;
}

#define TLVINIT(p) if(p) p->first_child=NULL; p->next_sibling=NULL; p->parent=NULL

/*
 * Given a symbol identifier, AND a buffer containing the raw "value",
 * returns a pointer to a tlv structure containing TLV buffer and the
 * length of the buffer, including docsis code and value length fields.
 * The length is relevant when we "assemble" two tlv_lists from an
 * an assignment_list + assignment_stmt
 */

struct tlv *
create_tlv(struct symbol_entry *sym_ptr, union t_val *value)
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv) ) ;
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = sym_ptr->encode_func(tlvbuf->tlv_value,value,sym_ptr);
		if (tlvbuf->tlv_len <= 0 ) {
			printf ("Got 0-length value while scanning for %s at line %d\n",sym_ptr->sym_ident,line );
			exit (-1);
   		}
  return tlvbuf;
}

/* Given a symbol identifier ( always SnmpMibObject ), a string
** containing an OID name, a string which sets the value of the OID and a t_val
** which contains the raw value (of type oid_asntype), creates a TLV with the
** corresponding variable binding.
*/

struct tlv *
create_snmpset_tlv ( struct symbol_entry *sym_ptr,
				char *oid_string,
				char oid_asntype,
				union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_vbind (oid_string, oid_asntype, value,
			            tlvbuf->tlv_value,TLV_VSIZE );

		if (tlvbuf->tlv_len <= 0 ) {
			printf ("got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,line );
			exit (-1);
   		}
  free(oid_string);
/* We only free non-string values since we parse strings into a static buffer in docsis_lex.l */
  if (	oid_asntype == 'x' || oid_asntype == 'a' ||
	oid_asntype == 'd' || oid_asntype == 'o' )  {
	 	free(value->strval);
  }
  return tlvbuf;
}


/* Given a symbol identifier ( always SnmpWrite ), a string
** containing an OID, and an integer, creates a TLV containing the
** OID ASN encoding + 1 control byte to permit/deny SNMP Write operations on a
** MIB variable which has the OID as prefix.
*/

struct tlv *
create_snmpw_tlv ( struct symbol_entry *sym_ptr,
                               char *oid_string,
                               union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_snmp_oid ( oid_string, tlvbuf->tlv_value, TLV_VSIZE );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,line );
                        exit (-1);
                }
  tlvbuf->tlv_value[tlvbuf->tlv_len] = (unsigned char) value->intval ;
  tlvbuf->tlv_len++;
  free(oid_string);
  return tlvbuf;
}

/* Given a code, length and raw value (encoded as a hex string),
** creates a TLV encoding. This can be used for e.g. VendorSpecific
** Information or unsupported (future?) configuration settings.
*/
struct tlv *
create_generic_tlv ( struct symbol_entry *sym_ptr,
				 int tlv_code,
                                 int tlv_length,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_hexstr ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len != tlv_length ) {
                        printf ("Length mismatch while encoding GenericTLV: given length %d, value length %d at line %d\n", tlvbuf->tlv_len, tlv_length, line );

                        exit (-1);
		}
  return tlvbuf;
}

/*
** Same as above, except the value is encoded as a string.
*/

struct tlv *
create_generic_str_tlv ( struct symbol_entry *sym_ptr,
                                 int tlv_code,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_string ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len > 255 ) {
                        printf ("Warning: string length %d longer than 255, line %d\n",tlvbuf->tlv_len, line );

                }
  /* don't free strings - we use a static buffer to parse them */
  return tlvbuf;
}

/*
** Same as above, except the value is encoded as a zero-terminated string.
*/

struct tlv *
create_generic_strzero_tlv ( struct symbol_entry *sym_ptr,
                                 int tlv_code,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_string ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got 0-length value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len > 254 ) {
                        printf ("Warning: total string length %d longer than 255, line %d\n",tlvbuf->tlv_len, line );

                } else {
			tlvbuf->tlv_len = tlvbuf->tlv_len + 1; /* add terminating 0 */
		}
  tlvbuf->tlv_value[tlvbuf->tlv_len]=0;
  return tlvbuf;
}

/* Given symbol name and a strings that represents a filename
** creates a TLV encoding from the raw data in the file.
** If the file is longer than 255, it is split into "parts" of
** 255 octets max.
*/
struct tlv *
create_external_file_tlv ( struct symbol_entry *sym_ptr,
                                 union t_val *value )
{
  struct tlv *new_tlvbuf=NULL, *first_tlvbuf = NULL, *old_tlvbuf = NULL;
  size_t read_len;
  unsigned char read_buffer[255];
  FILE *ext_file;

  if ((ext_file = fopen (value->strval, "rb")) == NULL) {
  	printf ("Error: can't open external file %s at line %d\n", value->strval, line);
	exit (-5);
  }

  while ( !feof(ext_file) ) {
 	if (! (read_len = fread (read_buffer, 1, 254, ext_file)) ) {
	  fprintf (stderr, "Error reading data from %s\n", value->strval) ;
  	  fclose (ext_file);
	  exit(-5);
  	}

	new_tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  	TLVINIT(new_tlvbuf);
	if (first_tlvbuf == NULL) first_tlvbuf = new_tlvbuf;

  	new_tlvbuf->docs_code = sym_ptr->docsis_code;
  	new_tlvbuf->tlv_len = read_len;

	memcpy (new_tlvbuf->tlv_value, read_buffer, read_len);
	if(old_tlvbuf) old_tlvbuf->next_sibling = new_tlvbuf;
	old_tlvbuf = new_tlvbuf;
  }
  fclose (ext_file);
  return first_tlvbuf;
}


/*
 * Adds a TLV to the current tlv "list". If the tlv pointer we are called with is NULL,
 * we assume this is the "first" tlv and just return the initialized newtlv.
 */

struct tlv *add_tlv_sibling (struct tlv *tlv, struct tlv *newtlv)
{
  struct tlv *tlvptr, *last_sibling;

  if (newtlv == NULL ) {
	printf("Error: add_tlv_sibling called with NULL tlv sibling ! \n " ) ;
	exit(-23);
  }

  if (tlv != NULL ) {
/*  	tlv->parent=NULL;  */
	/* find "last" sibling in this assignment list */

	for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling)
		last_sibling=tlvptr;
	/* Add the new TLV at the end of the list */
	last_sibling->next_sibling = newtlv;
	return tlv;
  } else  {
	return newtlv;
  }
}

/* Merge two parse "subtrees" into one  */

struct tlv *
merge_tlvlist(struct tlv *tlv1, struct tlv *tlv2)
{
  struct tlv *tlvptr, *last_sibling;
  if ( tlv2 == NULL ) {
	printf ("merge_tlvlist called with NULL tlv2 !\n");
	exit(-2);
  }

  if ( tlv1 == NULL ) {
	/* subsettings_stmt can be empty; in that case its value is NULL */
	return tlv2;
  }
  /* find "last" sibling in this assignment list */

  for (tlvptr=tlv1; tlvptr; tlvptr=tlvptr->next_sibling)
	last_sibling=tlvptr;
  last_sibling->next_sibling = tlv2;
  return tlv1;

}

/*
 * Creates the "parent" tlv that holds the "subtree" (e.g. the parent TLV's first_child will point to the
 * first element of this subtree.
 */

struct tlv *
assemble_tlv_in_parent (struct symbol_entry *sym_ptr, struct tlv *child_tlv)
{
  struct tlv *parent_tlv;

  parent_tlv = (struct tlv *) malloc ( sizeof ( struct tlv) );
  memset(parent_tlv, 0, sizeof(struct tlv));

  parent_tlv->docs_code = sym_ptr->docsis_code;
  parent_tlv->tlv_len = 0;
  parent_tlv->first_child = child_tlv;
  child_tlv->parent = parent_tlv;
  /* TODO: make al children's "parent" point back to parent_tlv */
  return parent_tlv;
}

/*
 * Creates a buffer filled with the TLV bytes sequentially, as they should be
 * found in the final configuration file.
 * The function walkts the tree recursively in "depth-last" mode and aggregates
 * the parsed values into an output "binary".
 */

unsigned int flatten_tlvsubtree ( unsigned char *buf, unsigned int used_size, struct tlv *tlv)
{
  struct tlv *tlvptr;
  unsigned int rsize;
  unsigned short netshort;
  register unsigned char *cp;

  if ( buf == NULL ) {
	printf( "Error: can't flatten tlvlist in a NULL destination  buffer!\n" );
	exit (-2);
  }

  cp = buf + used_size;

  for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling ) {
	if (tlvptr->first_child )  { /* Sub-Settings */
		/* we don't know the size yet, so we delay writing type & length */
		rsize = flatten_tlvsubtree(buf, (cp-buf)+2, tlvptr->first_child);
		if (rsize > 255) {
			printf("Warning: at line %d: aggregate size of settings block larger than 255, skipping\n", line);
			continue;
		}
		*cp = (unsigned char) tlvptr->docs_code; cp++;
		*cp = (unsigned char) rsize; cp++;
		cp = cp + rsize;
  	} else if (tlvptr->tlv_len <= 255 ) {

			*cp = (unsigned char) tlvptr->docs_code; cp++;
			*cp = (unsigned char) tlvptr->tlv_len; cp++;
			if (tlvptr->tlv_len > 0)
				memcpy ( cp, tlvptr->tlv_value, tlvptr->tlv_len );
  	     		cp = cp + tlvptr->tlv_len;
		} else {
			/* convert TLV11 to TLV64 */
			if (tlvptr->docs_code == 11) {
				*cp = 64; cp++;
			        netshort = htons(tlvptr->tlv_len);
				memcpy ( cp, &netshort, sizeof(unsigned short)); cp=cp+2;
       				memcpy ( cp, tlvptr->tlv_value, tlvptr->tlv_len );
             			cp = cp + tlvptr->tlv_len;
			} else {
				printf("Warning at line %d: Non-SnmpMibObject TLV larger than 255... skipping.\n", line);
				continue;
			}
		}
	}
  return (unsigned int) (cp - buf - used_size); /* return the number of bytes encoded this time  */
}

/*
 *  Recursively walk the tree and calculate the total size of the "concatenated" TLVs
 */
unsigned int tlvtreelen (struct tlv *tlv)
{
   struct tlv *tlvptr;
   unsigned int current_size=0;

   if (tlv == NULL) return 0;

   for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling)  {
        current_size += tlvptr->first_child ? 2+tlvtreelen(tlvptr->first_child) : 2+tlvptr->tlv_len;
   }
   return current_size;
}

int parse_config_file ( char *file, struct tlv **parse_tree_result )
{
  FILE *cf;
  int rval;

  if ( (cf = fopen ( file, "r" ))== NULL )
  {
	printf ("%s: Can't open input file %s\n", prog_name, file );
	return -1;
  }

  yyin = cf ;
#ifdef DEBUG
  yydebug = 1;
#endif
  rval = yyparse();
  if (!rval) *parse_tree_result = _my_tlvtree_head;
  fclose(cf);
  return rval;
}


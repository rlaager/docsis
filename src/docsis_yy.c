/* A Bison parser, made from docsis_yy.y, by GNU bison 1.75.  */

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
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	0

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
     T_ETHERMASK = 265,
     T_LABEL_OID = 266,
     T_SUBMGT_FILTERS = 267,
     T_IP = 268,
     T_MAC = 269,
     T_MAIN = 270,
     T_STRING = 271,
     T_HEX_STRING = 272,
     T_ASNTYPE_INT = 273,
     T_ASNTYPE_UINT = 274,
     T_ASNTYPE_GAUGE = 275,
     T_ASNTYPE_TICKS = 276,
     T_ASNTYPE_IP = 277,
     T_ASNTYPE_OBJID = 278,
     T_ASNTYPE_STRING = 279,
     T_ASNTYPE_HEXSTR = 280,
     T_ASNTYPE_DECSTR = 281,
     T_ASNTYPE_BITSTR = 282,
     T_ASNTYPE_BIGINT = 283,
     T_ASNTYPE_UBIGINT = 284,
     T_ASNTYPE_FLOAT = 285,
     T_ASNTYPE_DOUBLE = 286,
     T_TLV_CODE = 287,
     T_TLV_LENGTH = 288,
     T_TLV_VALUE = 289
   };
#endif
#define T_IDENTIFIER 258
#define T_INTEGER 259
#define T_IDENT_COS 260
#define T_IDENT_BPI 261
#define T_IDENT_SNMPW 262
#define T_IDENT_SNMPSET 263
#define T_IDENT_GENERIC 264
#define T_ETHERMASK 265
#define T_LABEL_OID 266
#define T_SUBMGT_FILTERS 267
#define T_IP 268
#define T_MAC 269
#define T_MAIN 270
#define T_STRING 271
#define T_HEX_STRING 272
#define T_ASNTYPE_INT 273
#define T_ASNTYPE_UINT 274
#define T_ASNTYPE_GAUGE 275
#define T_ASNTYPE_TICKS 276
#define T_ASNTYPE_IP 277
#define T_ASNTYPE_OBJID 278
#define T_ASNTYPE_STRING 279
#define T_ASNTYPE_HEXSTR 280
#define T_ASNTYPE_DECSTR 281
#define T_ASNTYPE_BITSTR 282
#define T_ASNTYPE_BIGINT 283
#define T_ASNTYPE_UBIGINT 284
#define T_ASNTYPE_FLOAT 285
#define T_ASNTYPE_DOUBLE 286
#define T_TLV_CODE 287
#define T_TLV_LENGTH 288
#define T_TLV_VALUE 289




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
extern struct tlv_list *global_tlvlist; /* Global list of all config TLVs */
extern symbol_type *global_symtable;
extern FILE *yyin;
extern char prog_name[255];
 


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

#ifndef YYSTYPE
#line 44 "docsis_yy.y"
typedef union { 	/* Token types */
	int intval;			/* For integers */
	unsigned int uintval;		/* For unsigned integers */
	symbol_type *symptr; 		/* For token identifiers */
	char *strval;		 	/* For strings */
	unsigned char *ustrval; 	/* For any other types */
	unsigned int ip;		/* For IP Addresses */
	struct tlv_list *tlvlist; 	/* For for struct tlvlist pointers */
	struct tlv *tlvptr;		/* For struct tlv pointers; */
} yystype;
/* Line 193 of /usr/share/bison/yacc.c.  */
#line 173 "docsis_yy.c"
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */


/* Line 213 of /usr/share/bison/yacc.c.  */
#line 194 "docsis_yy.c"

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
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

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
	    (To)[yyi] = (From)[yyi];	\
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
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
#define YYLAST   67

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  5
/* YYNRULES -- Number of rules. */
#define YYNRULES  22
/* YYNRULES -- Number of states. */
#define YYNSTATES  60

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    37,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     8,    11,    13,    16,    18,    23,    27,
      31,    35,    39,    43,    47,    51,    55,    60,    66,    72,
      78,    84,    90
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      39,     0,    -1,    15,    35,    40,    36,    -1,    40,    42,
      -1,    42,    -1,    40,    41,    -1,    41,    -1,     3,    35,
      40,    36,    -1,     3,     4,    37,    -1,     3,    16,    37,
      -1,     3,    17,    37,    -1,     3,    12,    37,    -1,     3,
      13,    37,    -1,     3,    14,    37,    -1,     3,    10,    37,
      -1,     3,    11,    37,    -1,     7,    11,     4,    37,    -1,
       8,    11,    18,     4,    37,    -1,     8,    11,    20,     4,
      37,    -1,     8,    11,    22,    13,    37,    -1,     8,    11,
      24,    16,    37,    -1,     8,    11,    25,    17,    37,    -1,
       9,    32,     4,    33,     4,    34,    17,    37,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,   128,   128,   132,   133,   134,   136,   140,   144,   146,
     148,   150,   152,   154,   156,   158,   160,   162,   164,   166,
     168,   170,   172
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_INTEGER", "T_IDENT_COS", 
  "T_IDENT_BPI", "T_IDENT_SNMPW", "T_IDENT_SNMPSET", "T_IDENT_GENERIC", 
  "T_ETHERMASK", "T_LABEL_OID", "T_SUBMGT_FILTERS", "T_IP", "T_MAC", 
  "T_MAIN", "T_STRING", "T_HEX_STRING", "T_ASNTYPE_INT", "T_ASNTYPE_UINT", 
  "T_ASNTYPE_GAUGE", "T_ASNTYPE_TICKS", "T_ASNTYPE_IP", "T_ASNTYPE_OBJID", 
  "T_ASNTYPE_STRING", "T_ASNTYPE_HEXSTR", "T_ASNTYPE_DECSTR", 
  "T_ASNTYPE_BITSTR", "T_ASNTYPE_BIGINT", "T_ASNTYPE_UBIGINT", 
  "T_ASNTYPE_FLOAT", "T_ASNTYPE_DOUBLE", "T_TLV_CODE", "T_TLV_LENGTH", 
  "T_TLV_VALUE", "'{'", "'}'", "';'", "$accept", "main_stmt", 
  "assignment_list", "config_stmt", "assignment_stmt", 0
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
     285,   286,   287,   288,   289,   123,   125,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    38,    39,    40,    40,    40,    40,    41,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     4,     2,     1,     2,     1,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     5,     5,     5,
       5,     5,     8
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,     0,
       6,     4,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     5,     3,     8,    14,    15,
      11,    12,    13,     9,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     7,    16,     0,     0,     0,     0,     0,
       0,    17,    18,    19,    20,    21,     0,     0,     0,    22
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     9,    10,    11
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -24
static const yysigned_char yypact[] =
{
      -4,   -23,    13,    22,   -24,     6,     3,     4,   -11,    -3,
     -24,   -24,   -13,    -5,    -2,     2,     9,    10,    11,    12,
      22,    30,    20,    33,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,     0,    14,    39,    46,    40,
      36,    37,    23,   -24,   -24,    18,    21,    24,    25,    26,
      53,   -24,   -24,   -24,   -24,   -24,    31,    42,    27,   -24
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -24,   -24,    47,    -8,    -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
       5,    25,    26,     5,     6,     7,     8,     6,     7,     8,
      12,     1,     3,     4,    21,    22,    13,    14,    15,    16,
      17,    23,    18,    19,    27,     5,     0,    25,    26,     6,
       7,     8,    28,    24,    36,    29,    43,    42,    37,    30,
      38,    20,    39,    45,    40,    41,    31,    32,    33,    34,
      46,    44,    48,    47,    49,    51,    50,    56,    52,    58,
       0,    53,    54,    55,    59,    57,     0,    35
};

static const yysigned_char yycheck[] =
{
       3,     9,     9,     3,     7,     8,     9,     7,     8,     9,
       4,    15,    35,     0,    11,    11,    10,    11,    12,    13,
      14,    32,    16,    17,    37,     3,    -1,    35,    35,     7,
       8,     9,    37,    36,     4,    37,    36,     4,    18,    37,
      20,    35,    22,     4,    24,    25,    37,    37,    37,    37,
       4,    37,    16,    13,    17,    37,    33,     4,    37,    17,
      -1,    37,    37,    37,    37,    34,    -1,    20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    15,    39,    35,     0,     3,     7,     8,     9,    40,
      41,    42,     4,    10,    11,    12,    13,    14,    16,    17,
      35,    11,    11,    32,    36,    41,    42,    37,    37,    37,
      37,    37,    37,    37,    37,    40,     4,    18,    20,    22,
      24,    25,     4,    36,    37,     4,     4,    13,    16,    17,
      33,    37,    37,    37,    37,    37,     4,    34,    17,    37
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
#define YYEMPTY		-2
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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#define YYLEX	yylex ()

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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
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
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;


int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

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

  if (yyssp >= yyss + yystacksize - 1)
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
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
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

      if (yyssp >= yyss + yystacksize - 1)
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

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
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
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

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



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
#line 128 "docsis_yy.y"
    {
			global_tlvlist = yyvsp[-1].tlvlist; }
    break;

  case 3:
#line 132 "docsis_yy.y"
    { yyval.tlvlist = add_tlv_to_list (yyvsp[-1].tlvlist,yyvsp[0].tlvptr); }
    break;

  case 4:
#line 133 "docsis_yy.y"
    {  yyval.tlvlist=add_tlv_to_list(NULL,yyvsp[0].tlvptr); }
    break;

  case 5:
#line 134 "docsis_yy.y"
    { 
			yyval.tlvlist = merge_tlvlist(yyvsp[-1].tlvlist,yyvsp[0].tlvlist);  }
    break;

  case 6:
#line 136 "docsis_yy.y"
    { 
			yyval.tlvlist = merge_tlvlist (NULL, yyvsp[0].tlvlist); }
    break;

  case 7:
#line 140 "docsis_yy.y"
    { 
			yyval.tlvlist = assemble_list_in_parent ( yyvsp[-3].symptr, yyvsp[-1].tlvlist ); }
    break;

  case 8:
#line 144 "docsis_yy.y"
    { 
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].uintval);}
    break;

  case 9:
#line 146 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 10:
#line 148 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 11:
#line 150 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 12:
#line 152 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 13:
#line 154 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 14:
#line 156 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 15:
#line 158 "docsis_yy.y"
    {
			yyval.tlvptr = create_tlv (yyvsp[-2].symptr, (union t_val *)&yyvsp[-1].strval);}
    break;

  case 16:
#line 160 "docsis_yy.y"
    {    
			yyval.tlvptr = create_snmpw_tlv ( yyvsp[-3].symptr, yyvsp[-2].strval, (union t_val *) &yyvsp[-1].uintval ); }
    break;

  case 17:
#line 162 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'i',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 18:
#line 164 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'g',(union t_val *)&yyvsp[-1].uintval); }
    break;

  case 19:
#line 166 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'a',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 20:
#line 168 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'s',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 21:
#line 170 "docsis_yy.y"
    {
			yyval.tlvptr = create_snmpset_tlv(yyvsp[-4].symptr,yyvsp[-3].strval,'x',(union t_val *)&yyvsp[-1].strval); }
    break;

  case 22:
#line 172 "docsis_yy.y"
    {
			yyval.tlvptr = create_generic_tlv(yyvsp[-7].symptr,yyvsp[-5].uintval,yyvsp[-3].uintval, (union t_val *)&yyvsp[-1].strval); }
    break;


    }

/* Line 1016 of /usr/share/bison/yacc.c.  */
#line 1176 "docsis_yy.c"

  yyvsp -= yylen;
  yyssp -= yylen;


#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

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
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
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
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
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
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

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

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--yyssp;


#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
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


#line 175 "docsis_yy.y"


int yyerror(char *s) {
	fprintf(stderr, "%d:%s token %s\n",line,s,yytext ); 
	return 0;
}

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
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_vbind (oid_string, oid_asntype, value,
			            tlvbuf->tlv_value,TLV_VSIZE );
				    
		if (tlvbuf->tlv_len <= 0 ) { 
			printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,line );
			exit (-1);
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
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_snmp_oid ( oid_string, tlvbuf->tlv_value, TLV_VSIZE );
                                    
                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,line );
                        exit (-1);
                }
  tlvbuf->tlv_value[tlvbuf->tlv_len] = (unsigned char) value->intval ;
  tlvbuf->tlv_len++;
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
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_hexstr ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len != tlv_length ) {
                        printf ("Length mismatch while encoding GenericTLV: given length %d, value length %d at line %d",tlvbuf->tlv_len, tlv_length, line );

                        exit (-1);
		}
  return tlvbuf;
}

/* Adds a TLV to a tlvlist. If the tlvlist pointer we are called with is NULL, 
 * we create the new tlvlist. 
 */ 

struct tlv_list *add_tlv_to_list(struct tlv_list *list, struct tlv *newtlv) 
{
  struct tlv_list *newtlvlist;
  newtlvlist = (struct tlv_list *) malloc ( sizeof(struct tlv_list) );
  if ( list == NULL ) {  /* assignment_list :== assignment_stmt  */
/* create list of tlv pointers with 1 tlv pointer */
	newtlvlist->tlv_count = 1;	
	newtlvlist->tlvlist   = (struct tlv **) malloc ( sizeof (struct tlv *));
  	newtlvlist->tlvlist[0] = newtlv;
	return newtlvlist;
  } else { /* assignment_list :== assignment_list assignment_stmt */
/* add a new tlv pointer to the existing list of tlv pointers */
	newtlvlist->tlv_count = list->tlv_count + 1;
	newtlvlist->tlvlist = (struct tlv **) malloc((newtlvlist->tlv_count)*sizeof(struct tlv *));
	memcpy(&(newtlvlist->tlvlist[0]),&(list->tlvlist[0]),list->tlv_count*sizeof(struct tlv *));
	newtlvlist->tlvlist[newtlvlist->tlv_count-1]=newtlv;
	free (list->tlvlist); free(list);
  	return newtlvlist;
  }
}

/* Merge two tlvlists into one tlvlist. */

struct tlv_list *
merge_tlvlist(struct tlv_list *list1, struct tlv_list *list2)
{
  struct tlv_list *newtlvlist;
  newtlvlist = (struct tlv_list *) malloc ( sizeof(struct tlv_list) );
  if ( list2 == NULL ) {
	printf ("merge_tlvlist called with NULL list2 !\n"); 
	exit (-2);
  }
  
  if ( list1 == NULL ) {
	/* config_stmt_list can be empty; in that case its value is NULL */
	return list2;
  } 
  newtlvlist->tlv_count = list1->tlv_count+list2->tlv_count;
  newtlvlist->tlvlist = (struct tlv **) malloc ((newtlvlist->tlv_count)*sizeof ( struct tlv *));
  memcpy ( &(newtlvlist->tlvlist[0]), &(list1->tlvlist[0]), 
		list1->tlv_count*sizeof(struct tlv *));  
  memcpy ( &(newtlvlist->tlvlist[list1->tlv_count]), &(list2->tlvlist[0]),
		list2->tlv_count*sizeof(struct tlv *));
  free(list1->tlvlist);free(list1);
  free(list2->tlvlist);free(list2);
/* We don't free the TLVs themselves since they are still referenced by 
 *  newtlvlist->tlvlist 
 */
  return newtlvlist;
}

/* 
 * Used for creating a top-level list corresponding to ClassOfService,
 * BaselinePrivacy, Telephony Settings etc.
 * Creates a "flat" TLV buffer out of a tlvlist, and assembles the 
 * TLV buffer into a new TLV which has the parent's docsis_code. 
 * Returns a TLV list with the new TLV 
 */

struct tlv_list *
assemble_list_in_parent (struct symbol_entry *sym_ptr, struct tlv_list *list) 
{
  struct tlv_list *newtlvlist;
  struct tlv *parent_tlv;

  newtlvlist = (struct tlv_list *) malloc ( sizeof ( struct tlv_list ));
  newtlvlist->tlvlist = (struct tlv **) malloc ( sizeof ( struct tlv *));
  parent_tlv = (struct tlv *) malloc ( sizeof ( struct tlv) );
  
  parent_tlv->docs_code = sym_ptr->docsis_code;
  parent_tlv->tlv_len = flatten_tlvlist(parent_tlv->tlv_value,list);
  newtlvlist->tlv_count=1;
  newtlvlist->tlvlist[0]=parent_tlv;
  return newtlvlist;
}

/* 
 * Creates a buffer filled with the TLV bytes sequentially, as they will be 
 * found in the final configuration file. 
 * This function is called either when creating a "parent" tlv corresponding to 
 * aggregate (e.g. BaselinePrivacy) TLVs, or when creating the final buffer 
 * before writing the output file.
 */

unsigned int 
flatten_tlvlist (unsigned char *buf, struct tlv_list *list )
{
  register unsigned char *cp;
  int i;

  if ( buf == NULL ) { 
	printf( "Error: can't flatten tlvlist in a NULL destination  buffer!\n" );
	exit (-2);
  }
  cp = buf;

  for ( i=0; i< list->tlv_count; i++ ) { 
	*cp = (unsigned char) list->tlvlist[i]->docs_code; cp++;
	*cp = (unsigned char) list->tlvlist[i]->tlv_len; cp++;
	memcpy ( cp, list->tlvlist[i]->tlv_value, list->tlvlist[i]->tlv_len ); 
  	     cp = cp + list->tlvlist[i]->tlv_len;
	free ( list->tlvlist[i] ); /* The TLV won't be referenced anymore */
  }
  /* We leave it to the caller to free the list */
  return (unsigned int) (cp-buf); 
}

int parse_config_file ( char *file ) 
{ 
  FILE *cf;

  if ( (cf = fopen ( file, "r" ))== NULL ) 
  {
	printf ("%s: Can't open config file %s\n",prog_name,file );
	exit(-5);
  }

  yyin = cf ; 
#ifdef DEBUG
  yydebug = 1;
#endif
  return yyparse();
}


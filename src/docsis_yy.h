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

#ifndef BISON_DOCSIS_YY_H
# define BISON_DOCSIS_YY_H

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
/* Line 1281 of /usr/share/bison/yacc.c.  */
#line 119 "y.tab.h"
# define YYSTYPE yystype
#endif

extern YYSTYPE yylval;


#endif /* not BISON_DOCSIS_YY_H */


/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001,2005 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL, office@evvolve.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

/*
 * The IPv6 regular expression below is from SSHGuard:
 *
 * Copyright (c) 2010 Mij <mij@sshguard.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

%{
#include "docsis.h"
#include "docsis_yy.h"

extern int yydebug;
extern symbol_type *global_symtable;

unsigned int line=1; /* On parse errors, we print the line number */

char *str; char strbuf[2048]; unsigned int strsize;/* max string len = 2048; TODO check overflow */
char *tsave;

#define TSAVE(s) tsave=(char*)malloc(yyleng+1);memset(tsave,0,yyleng+1);memcpy(tsave,s,yyleng)
%}
%x STRING
%x COMMENT
%%

([0-9]+,){3}[0-9]+		{ TSAVE(yytext);yylval.strval=tsave; return T_SUBMGT_FILTERS;	  }
([0-9]+\.){3}[0-9]+	 	{ TSAVE(yytext);yylval.strval=tsave; return T_IP;	  }
0[Xx]([0-9A-Fa-f])+		{ TSAVE(yytext);yylval.strval=tsave; return T_HEX_STRING; }
([0-9A-Fa-f][0-9A-Fa-f]:){5}[0-9A-Fa-f][0-9A-Fa-f]\/([0-9A-Fa-f][0-9A-Fa-f]:){5}[0-9A-Fa-f][0-9A-Fa-f]  { TSAVE(yytext);yylval.strval=tsave; return T_ETHERMASK; 			       }
([0-9A-Fa-f][0-9A-Fa-f]:){5}[0-9A-Fa-f][0-9A-Fa-f]  { TSAVE(yytext);yylval.strval=tsave; return T_MAC; 			       }

(\.{1})*([A-Za-z0-9_-]+\.)+[A-Za-z0-9]+	{ TSAVE(yytext);yylval.strval=tsave; return T_LABEL_OID; }
(\.{1})*([A-Za-z0-9_-]+\.)+'[\[A-Za-z0-9@,:_\-\.\]]+'	{ TSAVE(yytext);yylval.strval=tsave; return T_LABEL_OID; }
(\.{1})*([A-Za-z0-9_-]+\.)+((\"{1})*[A-Za-z0-9,:_\-\.]+(\"{1})*)+	{ TSAVE(yytext);yylval.strval=tsave; return T_LABEL_OID; }
([0-9]+:){3}[0-9]+\.[0-9]+	{ TSAVE(yytext);yylval.strval=tsave; return T_TIMETICKS; }
[Mm][Aa][Ii][Nn]        { yylval.strval=yytext;return T_MAIN; }
SnmpWriteControl	{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_SNMPW;   }
SnmpMibObject		{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_SNMPSET; }
ManufacturerCVC		{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_CVC; }
CoSignerCVC		{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_CVC; }
Integer			{ return T_ASNTYPE_INT; 	}
Unsigned32		{ return T_ASNTYPE_UINT; 	}
Short			{ return T_ASNTYPE_SHORT;       }
Char			{ return T_ASNTYPE_CHAR;        }
Gauge			{ return T_ASNTYPE_GAUGE; 	}
Gauge32			{ return T_ASNTYPE_GAUGE; 	}
Counter32 		{ return T_ASNTYPE_COUNTER; 	}
TimeTicks		{ return T_ASNTYPE_TIMETICKS; 	}
IPAddress		{ return T_ASNTYPE_IP; 		}
ObjectID		{ return T_ASNTYPE_OBJID; 	}
String			{ return T_ASNTYPE_STRING; 	}
HexString		{ return T_ASNTYPE_HEXSTR; 	}
DecimalString		{ return T_ASNTYPE_DECSTR; 	}
BitString		{ return T_ASNTYPE_BITSTR; 	}
BigInt			{ return T_ASNTYPE_BIGINT; 	}
UnsignedBigInt		{ return T_ASNTYPE_UBIGINT; 	}
Float			{ return T_ASNTYPE_FLOAT; 	}
Double			{ return T_ASNTYPE_DOUBLE; 	}
GenericTLV 		{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_GENERIC;   }
TlvCode 		{ return T_TLV_CODE; 	}
TlvLength 		{ return T_TLV_LENGTH; 	}
TlvValue		{ return T_TLV_VALUE; 	}
TlvString		{ return T_TLV_STR_VALUE; 	}
TlvStringZero		{ return T_TLV_STRZERO_VALUE; 	}
TlvType			{ return T_TLV_TYPE;		}
[A-Za-z]+[A-Za-z0-9]*	{
				yylval.symptr=find_symbol_by_name(yytext);
		  	  	if(yydebug)
				        fprintf(stderr, "yylex:returning T_IDENTIFIER, yylval.symptr->sym_ident=%s\n",
					yylval.symptr->sym_ident);
			 	return T_IDENTIFIER;
			}

\"		{ BEGIN STRING; str=strbuf; memset(strbuf,0,2048); strsize=0; }
<STRING>\\\"	{
			if (strsize<2048)
			{
				*str++ = '"';
				strsize++;
			} else {
			  fprintf(stderr, "line %d: string too long (max 2048 characters)\n",line); exit(-1);
			}
		}
<STRING>\\ 	{
			if (strsize<2048)
			{
				*str++ = ' ';
				strsize++;
			} else {
			  fprintf(stderr, "line %d: string too long (max 2048 characters)\n",line); exit(-1);
			}
		}
<STRING>\n	{ fprintf(stderr, "line %d: \\n not allowed in string, unmatched \" ?\n",line); exit(-1); }
<STRING>\t	{ fprintf(stderr, "line %d: \\t not allowed in string, unmatched \" ?\n",line); exit(-1); }
<STRING>\"	{ *str=0; BEGIN 0; yylval.strval=strbuf; return T_STRING; }
<STRING>.	{
			if (strsize<2048)
			{
				*str++ = *yytext;
				strsize++;
			} else {
			  fprintf(stderr, "line %d: string too long (max 2048 characters)\n",line); exit(-1);
			}
		}
\/[*]+		{ BEGIN COMMENT; }
<COMMENT>\n	{ line ++; }
<COMMENT>.
<COMMENT>\/[*]+	{ fprintf(stderr, "line %d: comment-in-comment not supported", line) ; }
<COMMENT>[*]+\/	{ BEGIN 0; }

[0-9]+	 { yylval.intval=atoi(yytext);return T_INTEGER; }
-[0-9]+	 { yylval.intval=atoi(yytext);return T_INTEGER; }
[ \t]+	 ;
\r\n	 { line++; }
\n	 { line++; }
\{ 	 { yylval.strval=yytext;return '{'; }
\}	 { yylval.strval=yytext;return '}'; }
;	 { yylval.strval=yytext;return ';'; }
.	 { fprintf(stderr, "Unrecognized char \"%c\" at line %d\n",*yytext,line); exit(-1); }
(::|(([a-fA-F0-9]{1,4}):){7}(([a-fA-F0-9]{1,4}))|(:(:([a-fA-F0-9]{1,4})){1,6})|((([a-fA-F0-9]{1,4}):){1,6}:)|((([a-fA-F0-9]{1,4}):)(:([a-fA-F0-9]{1,4})){1,6})|((([a-fA-F0-9]{1,4}):){2}(:([a-fA-F0-9]{1,4})){1,5})|((([a-fA-F0-9]{1,4}):){3}(:([a-fA-F0-9]{1,4})){1,4})|((([a-fA-F0-9]{1,4}):){4}(:([a-fA-F0-9]{1,4})){1,3})|((([a-fA-F0-9]{1,4}):){5}(:([a-fA-F0-9]{1,4})){1,2}))		{ TSAVE(yytext);yylval.strval=tsave; return T_IP6;	  }
%%

struct symbol_entry
*find_symbol_by_name (char *sym_str)
{
 int i;

 for ( i=0; i<NUM_IDENTIFIERS; i++) {
        if (!strcmp(sym_str, global_symtable[i].sym_ident)) {
                return &global_symtable[i];
        }
 }
 fprintf(stderr, "Unrecognized symbol %s at line %d\n", sym_str, line);
 exit(-1);
}


/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002 Evvolve Media SRL,office@evvolve.com
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

%{ 
#include "docsis.h"
#include "docsis_yy.tab.c"

extern symbol_type *global_symtable;

unsigned int line=1; /* On parse errors, we print the line number */

char *str;char buf[100]; /* max string len = 100; TODO check overflow */

char *tsave;
#define TSAVE(s) tsave=(char*)malloc(yyleng+1);memset(tsave,0,yyleng+1);memcpy(tsave,s,yyleng)
%}
%x STRING
%x COMMENT
%%

[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+	 { TSAVE(yytext);yylval.strval=tsave; return T_IP;	  }
\.([0-9]+\.)*[0-9]+		{ TSAVE(yytext);yylval.strval=tsave; return T_OID; 	  }
0[Xx]([0-9A-Fa-f])+		{ TSAVE(yytext);yylval.strval=tsave; return T_HEX_STRING; }
[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]\/[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]  { TSAVE(yytext);yylval.strval=tsave; return T_ETHERMASK; 			       }
[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]:[0-9A-Fa-f][0-9A-Fa-f]  { TSAVE(yytext);yylval.strval=tsave; return T_MAC; 			       }

([A-Za-z]+\.)+[0-9]+		{ TSAVE(yytext);yylval.strval=tsave; return T_LABEL_OID; }
[Mm][Aa][Ii][Nn]        { yylval.strval=yytext;return T_MAIN; }
SnmpWriteControl	{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_SNMPW;		 }
SnmpMibObject		{ yylval.symptr=find_symbol_by_name(yytext);return T_IDENT_SNMPSET; 	 }
Integer			{ return T_ASNTYPE_INT; 	}
UnsignedInteger		{ return T_ASNTYPE_UINT; 	}
TimeTicks		{ return T_ASNTYPE_TICKS; 	}
IPAddress		{ return T_ASNTYPE_IP; 		}
ObjectID		{ return T_ASNTYPE_OBJID; 	}
String			{ return T_ASNTYPE_STRING; 	}
HexString		{ return T_ASNTYPE_HEXSTR; 	}
DecimalString		{ return T_ASNTYPE_DECSTR; 	}
BigInt			{ return T_ASNTYPE_BIGINT; 	}
UnsignedBigInt		{ return T_ASNTYPE_UBIGINT; 	}
Float			{ return T_ASNTYPE_FLOAT; 	}
Double			{ return T_ASNTYPE_DOUBLE; 	}
[A-Za-z]+	 	{ yylval.symptr=find_symbol_by_name(yytext);
			  	if(yydebug) printf("yylex:returning T_IDENTIFIER, yylval.symptr->sym_ident=%s\n", 
						yylval.symptr->sym_ident); 
			  	return T_IDENTIFIER; }

\"		{ BEGIN STRING; str=buf; memset(buf,0,100); }	
<STRING>\\ 	{ *str++ = ' '; }
<STRING>\;	{ printf("line %d: ';' not allowed in string, unmatched \" ?\n",line); }
<STRING>\n	{ printf("line %d: \\n not allowed in string, unmatched \" ?\n",line);} 
<STRING>\t	{ printf("line %d: \\t not allowed in string, unmatched \" ?\n",line);} 
<STRING>\"	{ *str=0;BEGIN 0; yylval.strval=buf; return T_STRING; }
<STRING>.	{ *str++ = *yytext; }
\/[*]+		{ BEGIN COMMENT; }
<COMMENT>\n	{ line ++; }
<COMMENT>.	
<COMMENT>[*]+\/	{ BEGIN 0; }

[0-9]+	 { yylval.intval=atoi(yytext);return T_INTEGER; }
[ \t]+	 ;
\r\n	 { line++; }
\n	 { line++; }
\{ 	 { yylval.strval=yytext;return '{'; }
\}	 { yylval.strval=yytext;return '}'; }
;	 { yylval.strval=yytext;return ';'; }
.	 { printf("Unrecognized char \"%c\" at line %d\n",*yytext,line); exit(-1); }
%%

struct symbol_entry *find_symbol_by_name(char *sym_str) {
int i;

 for ( i=0; i<NUM_IDENTIFIERS; i++) {
        if (!strcmp(sym_str, global_symtable[i].sym_ident)) {
                return &global_symtable[i];
        }

 }
 printf ("Unrecognized symbol %s at line %d\n", sym_str,line);
 exit(-1);
}          


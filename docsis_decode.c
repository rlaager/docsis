/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
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

#include "docsis.h"
#include "ethermac.h"

extern symbol_type *global_symtable;


int decode_tlvbuf ( unsigned char *buf, unsigned int buflen , unsigned char docs_parent ) 
{
  register unsigned char *cp;
  int i,j;
  int howmany;
  unsigned int helper;
  unsigned short shelper;
  char filename[255];

  memset (filename, 0, 255);
   
  if (buf == NULL ) { printf ("Nothing to decode.\n"); return 0; }
  cp = buf; 
  while ( (unsigned int) (cp - buf) < buflen ) {
  for ( i = 0; i< NUM_IDENTIFIERS; i++ ){ 
	if ( (global_symtable[i].docsis_code == (unsigned char) *cp ) && (global_symtable[i].docsis_parent == docs_parent )) { 
		printf ("code %hhu ident %s len %hhu ", (unsigned char)*cp, global_symtable[i].sym_ident,(unsigned char) *(cp+1));
		switch ( docs_parent ) {
			case 0:
				switch ( (unsigned char) *cp ) { 
					case 0: /* pad */
						printf ("Pad\n");
						break;
						;;
					case 2: /* UpstreamChannelId uchar */
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 3: /* NetworkAccess uchar */
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 4: /* Class Of Service */
						printf ("ClassOfService {\n");
						decode_tlvbuf ( (unsigned char *) (cp+2), (unsigned char) *(cp+1), 4); 
						printf ("}\n");
						break;
						;;
					case 6: /* Cable Modem Message Integrity Check */
						printf ("digest ");
						for (j=0;j<16;j++) 
							printf ("%02x", cp[j+2] );
						printf("\n");
						break;
						;;
					case 7: /* CMTS Message Integrity Check */
						printf ("digest ");
						for (j=0;j<16;j++) 
							printf ("%02x", cp[j+2] );
						printf("\n");
						break;
						;;
					case 9: /* Software Upgrade Filename */
						strncpy ( filename, (char *) cp+2, (unsigned int) cp[1] );
						printf ( "filename %s\n", filename );
						break;
						;;
					case 10:  /* SNMP WriteDisable */
						decode_wd ( cp+2, (unsigned int) cp[1]);
						break;
						;;
					case 11:  /* SNMP Mib Object */
						decode_vbind ( cp+2, (unsigned int) cp[1]);
						break;
						;;
					case 14:  /* CPE MAC Address */
						printf ( "mac %s len %d\n", ether_ntoa(cp+2), (unsigned int) cp[1]);
						break;
						;;
					case 17:  /* Baseline Privacy */
						printf ("\n");
						decode_tlvbuf ( (unsigned char *) (cp+2), (unsigned char) *(cp+1), 17); 
						break;
						;;
					case 18: /* MaxCPEs - uchar */
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 21:  /* Software Upgrade Server IP address */
						printf ( "value %s\n", inet_ntoa(*((struct in_addr *)(cp+2))));
						break;
						;;
					default:  /* all unsigned int top-level TLVs */
						memset(&helper,0,sizeof(unsigned int));
						memcpy(&helper, cp+2, sizeof(unsigned int));
						printf ( "value %u\n", ntohl(helper));
						;;
				}
				break;
				;; /* end case 0 */
			case 4: 
				switch ( ( unsigned char ) *cp )  {
					case 6:  /* MaxBurstUp */
						memset(&shelper,0,sizeof(unsigned short));
						memcpy(&shelper, cp+2, sizeof(unsigned short));
						printf ( "value %hu\n", ntohs(shelper));
						break;
						;;
					case 7: 
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 1: 
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 4: 
						printf ( "value %hhu\n", (unsigned char) *(cp+2));
						break;
						;;
					default: 
						memset(&helper,0,sizeof(unsigned int));
						memcpy(&helper, cp+2, sizeof(unsigned int));
						printf ( "value %u\n", ntohl(helper));
						;;
				}
				break;
				;; /* end case 4  - Class Of Service */
			case 17: 
					/* all BaselinePrivacy values are uint, we don't need a switch() */
					memset(&helper,0,sizeof(unsigned int));
					memcpy(&helper, cp+2, sizeof(unsigned int));
					printf ( "value %u\n", ntohl(helper));
					;;
					break;
			} /* switch */
  	} /* if */
  } /* for */
  cp=(unsigned char*) cp+(((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  howmany++;
  }
 return howmany;
}  

int pretty_decode_buffer ( unsigned char *buf, unsigned int buflen , unsigned char docs_parent ) 
{
  register unsigned char *cp;
  int i,j,matched;
  int howmany;
  unsigned int helper;
  unsigned short shelper;
  char filename[255];

  memset (filename, 0, 255);

  if (buf == NULL ) { printf ("Nothing to decode.\n"); return 0; }
  if ( docs_parent == 0 ) printf( "Main {\n");

  cp = buf; 
  while ( (unsigned int) (cp - buf) < buflen ) {
  matched=0;
  for ( i = 0; i< NUM_IDENTIFIERS; i++ ) { 
	if ( (global_symtable[i].docsis_code == (unsigned char) *cp ) && (global_symtable[i].docsis_parent == docs_parent )) { 
		printf ("%s ", global_symtable[i].sym_ident);
		matched=1;
		switch ( docs_parent ) {
			case 0:
				switch ( (unsigned char) *cp ) { 
					case 0: /* pad */
						printf ("\n");
						break;
						;;
					case 1:  /* Downstream Freq */
						memset(&helper,0,sizeof(unsigned int));
						memcpy(&helper, cp+2, sizeof(unsigned int));
						printf ( "%u;\n", ntohl(helper));
						break;
						;;
					case 2: /* UpstreamChannelId uchar */
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					case 3: /* NetworkAccess uchar */
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					case 4: /* Class Of Service */
						printf(" {\n");
						pretty_decode_buffer ( (unsigned char *) (cp+2), (unsigned char) *(cp+1), 4); 
						printf("}\n");
						break;
						;;
					case 6: /* Cable Modem Message Integrity Check */
						for (j=0;j<16;j++) 
							printf ("%02x", cp[j+2] );
						printf(";\n");
						break;
						;;
					case 7: /* CMTS Message Integrity Check */
						for (j=0;j<16;j++) 
							printf ("%02x", cp[j+2] );
						printf(";\n");
						break;
						;;
					case 9: /* Software Upgrade Filename */
						strncpy ( filename, (char *) cp+2, (unsigned int) cp[1] );
						printf ( "\"%s\";\n", filename );
						break;
						;;
					case 10:  /* SNMP WriteDisable */
						decode_wd ( cp+2, (unsigned int) cp[1]);
						printf(";\n");
						break;
						;;
					case 11:  /* SNMP Mib Object */
						decode_vbind ( cp+2, (unsigned int) cp[1]);
						printf(";\n");
						break;
						;;
					case 14:  /* CPE MAC Address */
						printf ("%s;\n", ether_ntoa(cp+2));
						break;
						;;
					case 17:  /* Baseline Privacy */
						printf ("{\n");
						pretty_decode_buffer ( (unsigned char *) (cp+2), (unsigned char) *(cp+1), 17); 
						printf("}\n");
						break;
						;;
					case 18: /* MaxCPEs - uchar */
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					case 21:  /* Software Upgrade Server IP address */
						printf ( "%s;\n", inet_ntoa(*((struct in_addr *)(cp+2))));
						break;
						;;
					case 255:  /* End-Of-Data Marker */
						printf ("\n");
						break;
						;;
				}
				break;
				;; /* end case 0 -- Main */
			case 4: 	/* ClassOfService */
				switch ( ( unsigned char ) *cp )  {
					case 1:  /* ClassID */
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					case 4: /* PriorityUp */
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					case 6:  /* MaxBurstUp */
						memset(&shelper,0,sizeof(unsigned short));
						memcpy(&shelper, cp+2, sizeof(unsigned short));
						printf ( "%hu;\n", ntohs(shelper));
						break;
						;;
					case 7: 
						printf ( "%hhu;\n", (unsigned char) *(cp+2));
						break;
						;;
					default: 
						memset(&helper,0,sizeof(unsigned int));
						memcpy(&helper, cp+2, sizeof(unsigned int));
						printf ( "%u;\n", ntohl(helper));
						;;
				}
				break;
				;; /* end case 4  - Class Of Service */
			case 17:	
					/* all BaselinePrivacy values are uint, we don't need a switch() */
					memset(&helper,0,sizeof(unsigned int));
					memcpy(&helper, cp+2, sizeof(unsigned int));
					printf ( "%u;\n", ntohl(helper));
					;;
					break;
			}
  	} /* if */
  } /* for */

  if (!matched) {
	printf("GenericUnknownTLV docsis_code %hhu length %hhu value " , (unsigned char)*cp, (unsigned char) *(cp+1));
	if ((unsigned char)*(cp+1)) printf("0x"); else printf("none");
	for (j=0;j<(unsigned char)*(cp+1);j++) printf ("%02x", cp[j+2]);
	printf("\n");
  }     
  cp=(unsigned char*) cp+(((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  howmany++;
  } /* while */
  if ( docs_parent == 0 ) printf( "}\n");
  return howmany;
}  


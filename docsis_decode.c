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
  char filename[255];

  memset (filename, 0, 255);
   
  if (buf == NULL ) { printf ("Nothing to decode.\n"); return 0; }
  cp = buf; 
  while ( (unsigned int) (cp - buf) < buflen ) {
  for ( i = 0; i< NUM_IDENTIFIERS; i++ ){ 
	if ( (global_symtable[i].docsis_code == (unsigned char) *cp ) && (global_symtable[i].docsis_parent == docs_parent )) { 
		printf ("code %hu ident %s len %hu ", (unsigned char)*cp, global_symtable[i].sym_ident,(unsigned char) *(cp+1));
		switch ( docs_parent ) {
			case 0:
				switch ( (unsigned char) *cp ) { 
					case 0: /* pad */
						printf ("Pad\n");
						break;
						;;
					case 2: /* UpstreamChannelId uchar */
						printf ( "value %hu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 3: /* NetworkAccess uchar */
						printf ( "value %hu\n", (unsigned char) *(cp+2));
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
					case 12:  /* CPE MAC Address */
						printf ( "mac %s len %d\n", ether_ntoa(cp+2), (unsigned int) cp[1]);
						break;
						;;
					case 17:  /* Baseline Privacy */
						printf ("\n");
						decode_tlvbuf ( (unsigned char *) (cp+2), (unsigned char) *(cp+1), 17); 
						break;
						;;
					case 18: /* MaxCPEs - uchar */
						printf ( "value %hu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 21:  /* Software Upgrade Server IP address */
						printf ( "value %s\n", inet_ntoa(*((struct in_addr *)(cp+2))));
						break;
						;;
					default:  /* all unsigned int top-level TLVs */
						printf ( "value %u\n", ntohl( * (unsigned int * ) &cp[2]));
						;;
				}
				break;
				;; /* end case 0 */
			case 4: 
				switch ( ( unsigned char ) *cp )  {
					case 6:  /* MaxBurstUp */
						printf ( "value %hu\n", ntohs((unsigned short) *(cp+2)));
						break;
						;;
					case 7: 
						printf ( "value %hu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 1: 
						printf ( "value %hu\n", (unsigned char) *(cp+2));
						break;
						;;
					case 4: 
						printf ( "value %hu\n", (unsigned char) *(cp+2));
						break;
						;;
					default: 
						printf ( "value %u\n", ntohl( * (unsigned int * ) &cp[2]));
						;;
				}
				break;
				;; /* end case 4  - Class Of Service */
			case 17: 
					/* all BaselinePrivacy values are uint, we don't need a switch() */
					printf ( "value %u\n", ntohl( * (unsigned int * ) &cp[2]));
					;;
					break;
			}
  	}
  }
  cp=(unsigned char*) cp+(((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  howmany++;
  }
 return howmany;
}  


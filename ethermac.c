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

#include "ethermac.h"

int ether_aton ( const char *macstr, unsigned char *outbuf ) 
{

int fragval;
const char *ptr; char *p;
char fragptr[3];
unsigned char themac[6]; /* in binary form */
int i=0,rval=0;

ptr=macstr;

p = strchr ( ptr, (int) ':' );

  while (p && i<5) { 
	if ( p-ptr > 2 || p == NULL ) { 
	    printf ("\nInvalid MAC Address %s\n", macstr);
	    exit (-3);
 	}
	memset(fragptr,0,3); memcpy ( fragptr, ptr, p-ptr );
	if ( (rval= sscanf ( fragptr, "%x", &fragval)) == 0 ) { 
		printf("\nInvalid MAC Address %s\n", macstr );
		return 0;
	}
        if ( (unsigned int) fragval > 255 ) {
		printf("\nInvalid MAC Address %s\n", macstr );
		return 0;
	}
		
	themac[i]=(unsigned char) fragval;
	ptr=++p; 
	p = strchr ( ptr, (int) ':' );
	i++; 
  }

  if ( i != 5 || strlen(ptr) > 2 ) { 
    printf ("Invalid MAC Address %s\n", macstr);
    return 0;
  }
  memcpy ( fragptr, ptr, strlen(ptr) );
  if ( (rval=sscanf ( fragptr, "%x", &fragval)) == 0 ) { 
	printf("\nInvalid MAC Address %s\n", macstr );
	return 0;
  }
  if ( (unsigned int) fragval > 255 ) {
	printf("\nInvalid MAC Address %s\n", macstr );
	return 0;
  }
  themac[i] = (unsigned char) fragval;
  memcpy ( outbuf, themac, 6);
  return 6;
}

/* Tranforms the binary-form Ethernet MAC address received as argument into 
 * string-form human readable Ethernet MAC address */
char *ether_ntoa ( const unsigned char *mac)
{ 

  int i;
  static char themac[18]; 
  
  memset (themac, 0, 18);
  for (i=0;i<5;i++) sprintf ( themac+(3*i), "%02x:", mac[i]);
  sprintf ( themac+(3*i), "%02x", mac[i]);
  return themac;
}

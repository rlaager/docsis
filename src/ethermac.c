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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

/*
    change history
	2002-10-10 fixed overflow causing last two bytes of the MAC address to not be converted
		   correctly when the format was "xx:xx:xx:xx:xx:x"
*/

#include "ethermac.h"

int ether_aton ( const char *macstr, unsigned char *outbuf )
{

int fragval;
const char *ptr; char *p;
char fragptr[3];
unsigned char themac[6]; /* MAC address in binary form */
int i=0,rval=0;

ptr=macstr;

p = strchr ( ptr, (int) ':' );

  while (p && i<5) {
	if ( p-ptr > 2 || p == NULL ) {
	    fprintf(stderr, "\nInvalid MAC Address %s\n", macstr);
	    return 0;
 	}

	memset(fragptr,0,3);
	memcpy ( fragptr, ptr, (size_t)(p-ptr) );

	if ( (rval= sscanf ( fragptr, "%x", &fragval)) == 0 ) {
		fprintf(stderr, "\nInvalid MAC Address %s\n", macstr );
		return 0;
	}
        if ( (unsigned int) fragval > 255 ) {
		fprintf(stderr, "\nInvalid MAC Address %s\n", macstr );
		return 0;
	}

	themac[i]=(unsigned char) fragval;
	ptr=++p;
	p = strchr ( ptr, (int) ':' );
	i++;
  }

  if ( i != 5 || strlen(ptr) > 2 ) {
    fprintf(stderr, "Invalid MAC Address %s\n", macstr);
    return 0;
  }

  memset(fragptr,0,3);
  memcpy ( fragptr, ptr, strlen(ptr) );

  if ( (rval=sscanf ( fragptr, "%x", &fragval)) == 0 ) {
	fprintf(stderr, "\nInvalid MAC Address %s\n", macstr );
	return 0;
  }
  if ( (unsigned int) fragval > 255 ) {
	fprintf(stderr, "\nInvalid MAC Address %s\n", macstr );
	return 0;
  }
  themac[i] = (unsigned char) fragval;
  memcpy ( outbuf, themac, 6);
  return 6;
}

/*
 * Tranforms the binary-form Ethernet MAC address received as argument into
 * string-form human readable Ethernet MAC address
 * themac is a static char which WILL be overwritten by subsequent calls
 * to this function
 */
char *ether_ntoa ( const unsigned char *mac)
{

  int i;
  static char themac[18];

  memset (themac, 0, 18);
  for (i=0;i<5;i++) sprintf ( themac+(3*i), "%02x:", mac[i]);
  sprintf ( themac+(3*i), "%02x", mac[i]);
  return themac;
}

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

#include <arpa/inet.h>

/* 
 * Implements POSIX inet_aton which is missing on Solaris.
 * Based on inet_addr. 
 */

int inet_aton(const char *cp, struct in_addr *inp ) { 

  if (!strcmp(cp,"255.255.255.255")) { 
	inp->s_addr = (unsigned int) -1;
	return 1;
  }

  inp->s_addr = inet_addr ( cp );

  if ((int) inp->s_addr ==  -1 ) { 
	return 0;
  }
  
  return 1;
}

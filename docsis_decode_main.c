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
#include "docsis_symtable.h"
#include "docsis_globals.h"
#include "ethermac.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int line=0;

void usage(char *prog_name ) 
{
	printf( "DOCSIS Configuration File decoder, version %s.%s\n", VERSION,PATCHLEVEL); 
	printf ("Copyright (c) 2000 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
	printf( "Usage: \n\t %s <modem_binary_file>\n",prog_name);
	printf ( "\nWhere:\n<modem_cfg_file>\t= name of the binary configuration file\n"); 
	exit ( -10 );
}

int main(int argc,char *argv[] ) 
{

	unsigned char key[65];
	int ifd;
	unsigned char *buffer;
	unsigned int buflen=0;
	int rv=0;
	struct stat st;

	init_snmp("snmpapp");
	init_global_symtable();
   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_NUMERIC_OIDS)) {
        	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_NUMERIC_OIDS);
   	} /* we want OIDs to appear in numeric form */
   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_NUMERIC_ENUM)) {
        	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_NUMERIC_ENUM);
   	} /* we want OIDs to appear in numeric form */
   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_FULL_OID)) {
       		ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_FULL_OID);
   	} /* we want to full numeric OID to be printed, including prefix */
/*   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_QUICK_PRINT)) {
        	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_QUICK_PRINT);
   	} *//* quick print for easier parsing */
   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_DONT_BREAKDOWN_OIDS)) {
        	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_DONT_BREAKDOWN_OIDS);
   	} /* quick print for easier parsing */

	memset (prog_name,0,255);
	strncpy ( prog_name, argv[0], 254);
	if ( argc != 2 ) { 
		usage(prog_name);
	} 

	
	if ( (ifd = open (argv[1], O_RDONLY))==-1 )  {
		printf("Error opening file %s: %s", argv[1],strerror(errno));
		exit (-1);
	}	
	if ((rv=fstat(ifd, &st))) { 
		printf("stat on file %s: %s",argv[1],strerror(errno));
		exit(-1);
	}
	
	buffer = (unsigned char *) malloc (st.st_size*sizeof(unsigned char)+1);
	
	buflen = read(ifd, buffer, st.st_size);

	pretty_decode_buffer ( buffer, buflen, 0);
	return 0;
}

int init_global_symtable(void)
{
  global_symtable = (symbol_type *) malloc(sizeof(symbol_type)*NUM_IDENTIFIERS);  if (global_symtable == NULL) {
        printf ( "Error allocating memory!\n");
        exit (255);
  }
  memcpy ( global_symtable, symtable, sizeof(symbol_type)*NUM_IDENTIFIERS);
  return 1;
}

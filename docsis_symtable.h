
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

#ifndef _DOCSIS_SYMTABLE_H
#define _DOCSIS_SYMTABLE_H

#include "docsis.h"

/* get_nothing is used for non-numeric data */

symbol_type symtable[NUM_IDENTIFIERS] =  {
{"/* Pad */",			0,  0, (get_nothing), 	0, 0 }, 
{"DownstreamFrequency",	1,  0, (get_uint), 	88000000, 860000000 }, 
{"UpstreamChannelId", 	2,  0, (get_uchar), 	0, 255},
{"NetworkAccess",	3,  0, (get_uchar), 	0, 1},
{"CmMic",		6,  0, (get_nothing), 	0, 0},
{"CmtsMic",		7,  0, (get_nothing), 	0, 0},
{"ClassOfService",	4,  0, (get_nothing), 	0, 0}, 
{"ClassID",		1,  4, (get_uchar), 	1, 16},
{"MaxRateDown",		2,  4, (get_uint), 	0, 52000000},
{"MaxRateUp", 		3,  4, (get_uint), 	0, 10000000},
{"PriorityUp", 		4,  4, (get_uchar), 	0, 7},
{"GuaranteedUp",	5,  4, (get_uint), 	0, 10000000},
{"MaxBurstUp",		6,  4, (get_short), 	0, 65535},
{"PrivacyEnable", 	7,  4, (get_uchar), 	0, 1},
{"SwUpgradeFilename",	9,  0, (get_string), 	0, 0},
{"SnmpWriteControl",	10, 0, (get_nothing),   0, 0},
{"SnmpMibObject",	11, 0, (get_nothing),   0, 0}, /* has encode_vbind */
{"CpeMacAddress",	14, 0, (get_mac),   	0, 0}, 
{"BaselinePrivacy",	17, 0, (get_nothing), 	0, 0},
{"AuthTimeout",		1, 17, (get_uint), 	1, 30},
{"ReAuthTimeout", 	2, 17, (get_uint), 	1, 20},
{"AuthGraceTime",	3, 17, (get_uint), 	1, 1800},
{"OperTimeout",		4, 17, (get_uint), 	1, 10},
{"ReKeyTimeout", 	5, 17, (get_uint), 	1, 10},
{"TEKGraceTime", 	6, 17, (get_uint), 	1, 1800},
{"AuthRejectTimeout", 	7, 17, (get_uint), 	1, 600},
{"MaxCPE", 		18, 0, (get_uchar), 	0, 16},
{"SwUpgradeServer",	21, 0, (get_ip),   	0, 0},
{"/* EndOfDataMarker */",	255, 0, (get_nothing),  	0, 0}
};

#endif /* _DOCSIS_SYMTABLE_H */


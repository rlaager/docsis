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

#ifndef _DOCSIS_SYMTABLE_H
#define _DOCSIS_SYMTABLE_H

#include "docsis.h"
#include "docsis_decode.h"

/* 
 * get_nothing is used for non-numeric data, where we use other functions 
 * that would have prototypes which don't match symbol_type
 * This is a list and a tree at the same time. The "parent_id" of each entry is a pointer to the 
 * parent row in this table; ClassId for example has "6" as parent ID which is the index 
 * in the table of ClassOfService
 */
/* id  "identifier" docsis_code parent_id ... */

symbol_type symtable[NUM_IDENTIFIERS] =  { 
{ 0, 	"/* Pad */",		0,  0, (get_nothing), 	(decode_special), 	0, 0  }, 
{ 1, 	"DownstreamFrequency",	1,  0, (get_uint), 	(decode_uint),		88000000, 860000000  }, 
{ 2, 	"UpstreamChannelId", 	2,  0, (get_uchar), 	(decode_uchar), 	0, 255 },
{ 3, 	"NetworkAccess",	3,  0, (get_uchar), 	(decode_uchar),		0, 1 },
{ 4, 	"CmMic",		6,  0, (get_nothing), 	(decode_md5),		0, 0 },
{ 5, 	"CmtsMic",		7,  0, (get_nothing), 	(decode_md5),		0, 0 },
{ 6, 	"ClassOfService",	4,  0, (get_nothing), 	(decode_aggregate),	0, 0 }, 
{ 7, 	"ClassID",		1,  6, (get_uchar), 	(decode_uchar),		1, 16 },
{ 8, 	"MaxRateDown",		2,  6, (get_uint), 	(decode_uint),		0, 52000000 },
{ 9,	"MaxRateUp", 		3,  6, (get_uint), 	(decode_uint),		0, 10000000 },
{ 10,	"PriorityUp", 		4,  6, (get_uchar), 	(decode_uchar),		0, 7 },
{ 11,	"GuaranteedUp",		5,  6, (get_uint), 	(decode_uint),		0, 10000000 },
{ 12,	"MaxBurstUp",		6,  6, (get_ushort), 	(decode_ushort),	0, 65535 },
{ 13,	"PrivacyEnable", 	7,  6, (get_uchar), 	(decode_uchar),		0, 1 },
{ 14,	"SwUpgradeFilename",	9,  0, (get_string), 	(decode_string),	0, 0 },
{ 15,	"SnmpWriteControl",	10, 0, (get_nothing),   (decode_snmp_wd),	0, 0 },
{ 16,	"SnmpMibObject",	11, 0, (get_nothing),   (decode_snmp_object),	0, 0 }, /* has encode_vbind */
{ 17,	"CpeMacAddress",	14, 0, (get_ether),   	(decode_ether),		0, 0 }, 
{ 18,	"BaselinePrivacy",	17, 0, (get_nothing), 	(decode_aggregate),	0, 0 },
{ 19,	"AuthTimeout",		1, 18, (get_uint), 	(decode_uint),		1, 30 },
{ 20,	"ReAuthTimeout", 	2, 18, (get_uint), 	(decode_uint),		1, 20 },
{ 21,	"AuthGraceTime",	3, 18, (get_uint), 	(decode_uint),		1, 1800 },
{ 22,	"OperTimeout",		4, 18, (get_uint), 	(decode_uint),		1, 10 },
{ 23,	"ReKeyTimeout", 	5, 18, (get_uint), 	(decode_uint),		1, 10 },
{ 24,	"TEKGraceTime", 	6, 18, (get_uint), 	(decode_uint),		1, 1800 },
{ 25,	"AuthRejectTimeout", 	7, 18, (get_uint), 	(decode_uint),		1, 600 },
{ 26,	"MaxCPE", 		18, 0, (get_uchar), 	(decode_uchar),		0, 16 },
{ 27,	"SwUpgradeServer",	21, 0, (get_ip),   	(decode_ip),		0, 0 },
/* DOCSIS  1.1-2.0 */
{ 28,	"UsPacketClass",	22, 0, (get_nothing), 	(decode_aggregate),	0, 0 },
{ 29,	"ClassifierRef",	1, 28, (get_uchar), 	(decode_uchar),		1, 255 },
{ 30,	"ClassifierId",		2, 28, (get_ushort), 	(decode_ushort),	1, 65535 },
{ 31,	"ServiceFlowRef",	3, 28, (get_ushort), 	(decode_ushort),	1, 65535 },
{ 32,	"ServiceFlowId",	4, 28, (get_uint), 	(decode_uint),		1, 0xFFFFFFFF }, 
{ 33,	"RulePriority",		5, 28, (get_uchar), 	(decode_uchar),		0, 255 },
{ 34,	"ActivationState",	6, 28, (get_uchar), 	(decode_uchar),		0, 1 },
{ 35,	"DscAction	",	6, 28, (get_uchar), 	(decode_uchar),		0, 2 },
{ 36,   "IpPacketClassifier",   9, 28, (get_nothing),   (decode_aggregate),     0, 0 },      
{ 37,   "IpTos",                1, 36, (get_hexstr),    (decode_hexstr),        3, 3 },
{ 38,   "IpProto",              2, 36, (get_ushort),     (decode_ushort),       0, 257 },    
{ 39,   "IpSrcAddr",            3, 36, (get_ip),        (decode_ip),            0, 0 },      
{ 40,   "IpSrcMask",            4, 36, (get_ip),        (decode_ip),            0, 0 },      
{ 41,   "IpDstAddr",            5, 36, (get_ip),        (decode_ip),            0, 0 },      
{ 42,   "IpDstMask",            6, 36, (get_ip),        (decode_ip),            0, 0 },      
{ 43,   "SrcPortStart",         7, 36, (get_ushort),     (decode_ushort),       0, 65535 },  
{ 44,   "SrcPortEnd",           8, 36, (get_ushort),     (decode_ushort),       0, 65535 },  
{ 45,   "DstPortStart",         9, 36, (get_ushort),     (decode_ushort),       0, 65535 },  
{ 46,   "DstPortEnd",           10,36, (get_ushort), 	(decode_ushort),        0, 65535 },  
{ 47,   "LLCPacketClassifier",	10,28, (get_nothing),	(decode_aggregate),     0, 0 },      
{ 48,   "DstMacAddress",	1, 47, (get_ethermask),	(decode_ethermask),     0, 0 },      
{ 49,   "SrcMacAddress",	2, 47, (get_ether), 	(decode_ether),     	0, 0 },      
{ 50,   "EtherType",		3, 47, (get_hexstr), 	(decode_hexstr),     	3, 3 },      
{ 51,   "IEEE802Classifier",	11,28, (get_nothing), 	(decode_aggregate),    	0, 0 },      
{ 52,   "UserPriority",		1, 51, (get_ushort), 	(decode_ushort),    	0, 0 },      
{ 53,   "VlanID",		2, 51, (get_ushort), 	(decode_ushort),    	0, 0 },      
/* TODO: Vendor Specific support in the IEEE802Classifier */

{ 54,	"DsPacketClass",	23, 0, (get_nothing), 	(decode_aggregate),	0, 0 },
{ 55,	"ClassifierRef",	1, 54, (get_uchar), 	(decode_uchar),		1, 255 },
{ 56,	"ClassifierId",		2, 54, (get_ushort), 	(decode_ushort),	1, 65535 },
{ 57,	"ServiceFlowRef",	3, 54, (get_ushort), 	(decode_ushort),	1, 65535 },
{ 58,	"ServiceFlowId",	4, 54, (get_uint), 	(decode_uint),		1, 0xFFFFFFFF }, 
{ 59,	"RulePriority",		5, 54, (get_uchar), 	(decode_uchar),		0, 255 },
{ 60,	"ActivationState",	6, 54, (get_uchar), 	(decode_uchar),		0, 1 },
{ 61,	"DscAction	",	6, 54, (get_uchar), 	(decode_uchar),		0, 2 },
{ 62,   "IpPacketClassifier",   9, 54, (get_nothing),   (decode_aggregate),     0, 0 },      
{ 63,   "IpTos",                1, 62, (get_uint),      (decode_uint),          0, 16777215 },
{ 64,   "IpProto",              2, 62, (get_ushort),     (decode_ushort),        0, 257 },    
{ 65,   "IpSrcAddr",            3, 62, (get_ip),        (decode_ip),            0, 0 },      
{ 66,   "IpSrcMask",            4, 62, (get_ip),        (decode_ip),            0, 0 },      
{ 67,   "IpDstAddr",            5, 62, (get_ip),        (decode_ip),            0, 0 },      
{ 68,   "IpDstMask",            6, 62, (get_ip),        (decode_ip),            0, 0 },      
{ 69,   "SrcPortStart",         7, 62, (get_ushort),	(decode_ushort),       0, 65535 },  
{ 70,   "SrcPortEnd",           8, 62, (get_ushort),	(decode_ushort),       0, 65535 },  
{ 71,   "DstPortStart",         9, 62, (get_ushort),	(decode_ushort),       0, 65535 },  
{ 72,   "DstPortEnd",           10,62, (get_ushort),	(decode_ushort),        0, 65535 },  
{ 73,   "LLCPacketClassifier",	10,54, (get_nothing),	(decode_aggregate),     0, 0 },      
{ 74,   "DstMacAddress",	1, 73, (get_ethermask),	(decode_ethermask),     0, 0 },      
{ 75,   "SrcMacAddress",	2, 73, (get_ether), 	(decode_ether),     	0, 0 },      
{ 76,   "EtherType",		3, 73, (get_hexstr), 	(decode_hexstr),     	0, 0 },      
{ 77,   "IEEE802Classifier",	11,54, (get_nothing), 	(decode_aggregate),    	0, 0 },      
{ 78,   "UserPriority",		1, 77, (get_ushort), 	(decode_ushort),    	0, 0 },      
{ 79,   "VlanID",		2, 77, (get_ushort), 	(decode_ushort),    	0, 0 },      
/* Upstream Service Flow */
{ 80,   "UsServiceFlow",	24, 0, (get_nothing), 	(decode_aggregate),    	0, 0 },      
{ 81,   "ServiceFlowRef",	1, 80, (get_ushort), 	(decode_ushort),    	1, 65535 },      
{ 81,   "ServiceFlowId",	2, 80, (get_uint), 	(decode_uint),    	1, 0xFFFFFFFF },  
{ 82,   "ServiceClassName",	4, 80, (get_string), 	(decode_string),    	2, 16 },      
{ 83,   "QosParamSetType",	6, 80, (get_uchar), 	(decode_uchar),    	0, 255 },      
{ 84,   "TrafficPriority",	7, 80, (get_uchar), 	(decode_uchar),    	0, 7 },      
{ 85,   "MaxRateSustained",	8, 80, (get_uint), 	(decode_uint),    	0, 0 },
{ 86,   "MaxTrafficBurst",	9, 80, (get_uint), 	(decode_uint),    	0, 0 },
{ 87,   "MinReservedRate",	10,80, (get_uint), 	(decode_uint),    	0, 0 },
{ 88,   "MinResPacketSize",	11,80, (get_ushort), 	(decode_ushort),    	0, 65535 },
{ 89,   "ActQosParamsTimeout",	12,80, (get_ushort), 	(decode_ushort),    	0, 65535 },
{ 89,   "AdmQosParamsTimeout",	13,80, (get_ushort), 	(decode_ushort),    	0, 65535 },
/* Upstream Service Flow Specific params */
{ 90,   "MaxConcatenatedBurst",	14,80, (get_ushort), 	(decode_ushort),    	0, 65535 },
{ 91,   "SchedulingType",	15,80, (get_uchar), 	(decode_uchar),    	0, 6 },
{ 92,   "RequestOrTxPolicy",	16,80, (get_hexstr), 	(decode_hexstr),    	4, 4 },
{ 93,   "NominalPollInterval",	17,80, (get_uint), 	(decode_uint),    	0, 0 },
{ 94,   "ToleratedPollJitter",	18,80, (get_uint), 	(decode_uint),    	0, 0 },
{ 95,   "UnsolicitedGrantSize",	19,80, (get_ushort), 	(decode_ushort),    	0, 65535 },
{ 96,   "NominalGrantInterval",	20,80, (get_uint), 	(decode_uint),    	0, 0},
{ 97,   "ToleratedGrantJitter",	21,80, (get_uint), 	(decode_uint),    	0, 0 },
{ 98,   "GrantsPerInterval",	22,80, (get_uchar), 	(decode_uchar),    	0, 127 },
{ 99,   "IpTosOverwrite",	23,80, (get_hexstr), 	(decode_hexstr),    	2, 2 },
/* Downstream Service Flow */
{100,   "DsServiceFlow",	25,  0,(get_nothing), 	(decode_aggregate),    	0, 0 },      
{101,   "ServiceFlowRef",	1, 100, (get_ushort), 	(decode_ushort),    	1, 65535 },      
{102,   "ServiceFlowId",	2, 100, (get_uint), 	(decode_uint),    	1, 0xFFFFFFFF },  
{103,   "ServiceClassName",	4, 100, (get_string), 	(decode_string),    	2, 16 },      
{104,   "QosParamSetType",	6, 100, (get_uchar), 	(decode_uchar),    	0, 255 },      
{105,   "TrafficPriority",	7, 100, (get_uchar), 	(decode_uchar),    	0, 7 },      
{106,   "MaxRateSustained",	8, 100, (get_uint), 	(decode_uint),    	0, 0xFFFFFFFF },
{107,   "MaxTrafficBurst",	9, 100, (get_uint), 	(decode_uint),    	0, 0xFFFFFFFF },
{108,   "MinReservedRate",	10,100, (get_uint), 	(decode_uint),    	0, 0xFFFFFFFF },
{109,   "MinResPacketSize",	11,100, (get_ushort), 	(decode_ushort),    	0, 65535 },
{110,   "ActQosParamsTimeout",	12,100, (get_ushort), 	(decode_ushort),    	0, 65535 },
{111,   "AdmQosParamsTimeout",	13,100, (get_ushort), 	(decode_ushort),    	0, 65535 },
/* Downstream Service Flow Specific Params */
{112,   "MaxDsLatency",		14,100, (get_uint), 	(decode_uint),    	0, 0 },
{113,   "PHS",			26,  0, (get_nothing), 	(decode_aggregate),    	0, 0 },
{114,   "ClassifierRef",	1, 113, (get_uchar), 	(decode_uchar),    	1, 255 },
{115,   "ClassifierId",		2, 113, (get_ushort), 	(decode_ushort),    	1, 65535 },
{116,   "ServiceFlowRef",	3, 113, (get_ushort), 	(decode_ushort),    	1, 65535 },
{117,   "ServiceFlowId",	4, 113, (get_uint), 	(decode_uint),    	1, 0xFFFFFFFF},
{118,   "MaxClassifiers",	28, 0,  (get_ushort), 	(decode_ushort),    	0, 0 },
{119,   "GlobalPrivacyEnable",	29, 0,  (get_uchar), 	(decode_uchar),    	0, 0 },
{999,	"/* EndOfDataMkr */",	255,0,  (get_nothing), 	(decode_special),	0, 0 }
};

#endif /* _DOCSIS_SYMTABLE_H */


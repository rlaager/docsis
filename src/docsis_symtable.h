/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL,office@evvolve.com
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
	2004-07-29 fix bug # 970994 ... SubMgmtControl had wrong docsis code
	2003-01-10 added three new TLV types for BPI+ (courtesy of Adrian Kwong)

*/


#ifndef _DOCSIS_SYMTABLE_H


#include "docsis_common.h"
#include "docsis_decode.h"
#include "docsis_encode.h"

/*
 * encode_nothing is used for non-trivial settings, where the encoding functions
 * must have prototypes that don't match symbol_type (i.e. generic TLV, SNMP, etc).
 *
 * This is a list and a tree at the same time. The "parent_id" of each entry is a pointer to the
 * parent row in this table; ClassId for example has "6" as parent ID which is the index
 * in the table of ClassOfService
 */

/*id  "identifier" docsis_code parent_id encode_func decode_func low_limit high_limit */

symbol_type symtable[NUM_IDENTIFIERS] =  {
{ 0,      "/* Pad */",                         0,      0,      (encode_nothing),        (decode_special),        0,           0             },
{ 1,      "DownstreamFrequency",               1,      0,      (encode_uint),           (decode_uint),           88000000,    1008000000    }, /* MULPIv3.0-I24 Annex C.1.1.1 */
{ 2,      "UpstreamChannelId",                 2,      0,      (encode_uchar),          (decode_uchar),          0,           255           }, /* MULPIv3.0-I24 Annex C.1.1.2 */
{ 3,      "NetworkAccess",                     3,      0,      (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.1.3 */

/* Class of Service */
{ 6,      "ClassOfService",                    4,      0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* MULPIv3.0-I24 Annex C.1.1.4 */
{ 7,      "ClassID",                           1,      6,      (encode_uchar),          (decode_uchar),          1,           16            }, /* MULPIv3.0-I24 Annex C.1.1.4.1 */
{ 8,      "MaxRateDown",                       2,      6,      (encode_uint),           (decode_uint),           0,           1334880000    }, /* MULPIv3.0-I24 Annex C.1.1.4.2 */
{ 9,      "MaxRateUp",                         3,      6,      (encode_uint),           (decode_uint),           0,           183900000     }, /* MULPIv3.0-I24 Annex C.1.1.4.3 */
{ 10,     "PriorityUp",                        4,      6,      (encode_uchar),          (decode_uchar),          0,           7             }, /* MULPIv3.0-I24 Annex C.1.1.4.4 */
{ 11,     "GuaranteedUp",                      5,      6,      (encode_uint),           (decode_uint),           0,           183900000     }, /* MULPIv3.0-I24 Annex C.1.1.4.5 */
{ 12,     "MaxBurstUp",                        6,      6,      (encode_ushort),         (decode_ushort),         0,           65535         }, /* MULPIv3.0-I24 Annex C.1.1.4.6 */
{ 13,     "PrivacyEnable",                     7,      6,      (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.1.4.7 */

/* Modem Capabilities Encodings */
{ 144,    "ModemCapabilities",                 5,      0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* MULPIv3.0-I24 Annex C.1.3.1 */
{ 145,    "ConcatenationSupport",              1,      144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.1 */
{ 146,    "ModemDocsisVersion",                2,      144,    (encode_uchar),          (decode_uchar),          0,           3             }, /* MULPIv3.0-I24 Annex C.1.3.1.2 */
{ 147,    "FragmentationSupport",              3,      144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.3 */
{ 148,    "PHSSupport",                        4,      144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.4 */
{ 149,    "IGMPSupport",                       5,      144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.5 */
{ 150,    "BaselinePrivacySupport",            6,      144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.6 */
{ 151,    "DownstreamSAIDSupport",             7,      144,    (encode_uchar),          (decode_uchar),          0,           255           }, /* MULPIv3.0-I24 Annex C.1.3.1.7 */
{ 152,    "UpstreamSIDSupport",                8,      144,    (encode_uchar),          (decode_uchar),          0,           255           }, /* MULPIv3.0-I24 Annex C.1.3.1.8 */
{ 153,    "DCCSupport",                        12,     144,    (encode_uchar),          (decode_uchar),          0,           1             }, /* MULPIv3.0-I24 Annex C.1.3.1.12 */

{ 4,      "CmMic",                             6,      0,      (encode_nothing),        (decode_md5),            0,           0             }, /* MULPIv3.0-I24 Annex C.1.1.5 */
{ 5,      "CmtsMic",                           7,      0,      (encode_nothing),        (decode_md5),            0,           0             }, /* MULPIv3.0-I24 Annex C.1.1.6 */

{ 14,     "SwUpgradeFilename",                 9,      0,      (encode_string),         (decode_string),         0,           0             },
{ 15,     "SnmpWriteControl",                  10,     0,      (encode_nothing),        (decode_snmp_wd),        0,           0             },
{ 16,     "SnmpMibObject",                     11,     0,      (encode_nothing),        (decode_snmp_object),    0,           0             },
{ 17,     "CpeMacAddress",                     14,     0,      (encode_ether),          (decode_ether),          0,           0             },

/* BPI+ */

{ 18,     "BaselinePrivacy",                   17,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 17 */
{ 19,     "AuthTimeout",                       1,      18,     (encode_uint),           (decode_uint),           1,           30            },
{ 20,     "ReAuthTimeout",                     2,      18,     (encode_uint),           (decode_uint),           1,           30            },
{ 21,     "AuthGraceTime",                     3,      18,     (encode_uint),           (decode_uint),           1,           6047999       },
{ 22,     "OperTimeout",                       4,      18,     (encode_uint),           (decode_uint),           1,           10            },
{ 23,     "ReKeyTimeout",                      5,      18,     (encode_uint),           (decode_uint),           1,           10            },
{ 24,     "TEKGraceTime",                      6,      18,     (encode_uint),           (decode_uint),           1,           302399        },
{ 25,     "AuthRejectTimeout",                 7,      18,     (encode_uint),           (decode_uint),           1,           600           },
{ 127,    "SAMapWaitTimeout",                  8,      18,     (encode_uint),           (decode_uint),           1,           10            },
{ 128,    "SAMapMaxRetries",                   9,      18,     (encode_uint),           (decode_uint),           0,           10            },

{ 26,     "MaxCPE",                            18,     0,      (encode_uchar),          (decode_uchar),          1,           254           },
{ 168,    "TftpTimestamp",                     19,     0,      (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 169,    "TftpModemAddress",                  20,     0,      (encode_ip),             (decode_ip),             0,           0             },
{ 27,     "SwUpgradeServer",                   21,     0,      (encode_ip),             (decode_ip),             0,           0             },

/* DOCSIS 1.1-2.0 */

{ 28,     "UsPacketClass",                     22,     0,      (encode_nothing),        (decode_aggregate),      0,           0             },
{ 29,     "ClassifierRef",                     1,      28,     (encode_uchar),          (decode_uchar),          1,           255           }   ,
{ 30,     "ClassifierId",                      2,      28,     (encode_ushort),         (decode_ushort),         1,           65535         },
{ 31,     "ServiceFlowRef",                    3,      28,     (encode_ushort),         (decode_ushort),         1,           65535         },
{ 32,     "ServiceFlowId",                     4,      28,     (encode_uint),           (decode_uint),           1,           0xFFFFFFFF    },
{ 33,     "RulePriority",                      5,      28,     (encode_uchar),          (decode_uchar),          0,           255           },
{ 34,     "ActivationState",                   6,      28,     (encode_uchar),          (decode_uchar),          0,           1             },
{ 35,     "DscAction",                         7,      28,     (encode_uchar),          (decode_uchar),          0,           2             },
{ 36,     "IpPacketClassifier",                9,      28,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 37,     "IpTos",                             1,      36,     (encode_hexstr),         (decode_hexstr),         3,           3             },
{ 38,     "IpProto",                           2,      36,     (encode_ushort),         (decode_ushort),         0,           257           },
{ 39,     "IpSrcAddr",                         3,      36,     (encode_ip),             (decode_ip),             0,           0             },
{ 40,     "IpSrcMask",                         4,      36,     (encode_ip),             (decode_ip),             0,           0             },
{ 41,     "IpDstAddr",                         5,      36,     (encode_ip),             (decode_ip),             0,           0             },
{ 42,     "IpDstMask",                         6,      36,     (encode_ip),             (decode_ip),             0,           0             },
{ 43,     "SrcPortStart",                      7,      36,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 44,     "SrcPortEnd",                        8,      36,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 45,     "DstPortStart",                      9,      36,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 46,     "DstPortEnd",                        10,     36,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 47,     "LLCPacketClassifier",               10,     28,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 48,     "DstMacAddress",                     1,      47,     (encode_ethermask),      (decode_ethermask),      0,           0             },
{ 49,     "SrcMacAddress",                     2,      47,     (encode_ether),          (decode_ether),          0,           0             },
{ 50,     "EtherType",                         3,      47,     (encode_hexstr),         (decode_hexstr),         3,           3             },
{ 51,     "IEEE802Classifier",                 11,     28,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 52,     "UserPriority",                      1,      51,     (encode_ushort),         (decode_ushort),         0,           0             },
{ 53,     "VlanID",                            2,      51,     (encode_ushort),         (decode_ushort),         0,           0             },
{ 211,    "PcIPv6PacketClassification",        12,     28,     (encode_nothing),        (decode_aggregate),      0,           0             }, /* 22.12 */
{ 212,    "PcIPv6TrafficClassRangeAndMask",    1,      211,    (encode_hexstr),         (decode_hexstr),         0,           0             }, /* 22.12.1 */
{ 213,    "PcIPv6FlowLabel",                   2,      211,    (encode_hexstr),         (decode_hexstr),         4,           4             }, /* 22.12.2 */
{ 214,    "PcIPv6NextHeaderType",              3,      211,    (encode_ushort),         (decode_ushort),         0,           257           }, /* 22.12.3 */
{ 215,    "PcIPv6SourceAddress",               4,      211,    (encode_ip6),            (decode_ip6),            0,           0             }, /* 22.12.4 */
{ 216,    "PcIPv6SourcePrefixLength",          5,      211,    (encode_uchar),          (decode_uchar),          0,           128           }, /* 22.12.5 */
{ 217,    "PcIPv6DestAddress",                 6,      211,    (encode_ip6),            (decode_ip6),            0,           0             }, /* 22.12.6 */
{ 218,    "PcIPv6DestPrefixLength",            7,      211,    (encode_uchar),          (decode_uchar),          0,           128           }, /* 22.12.7 */
{ 219,    "PcCMIMEncoding",                    13,     28,     (encode_hexstr),         (decode_hexstr),         0,           0             }, /* 22.13 */
/* TLV 22 - Upstream Packet Classification */
{ 200,    "VendorSpecific",                    43,     28,     (encode_nothing),        (decode_aggregate),      0,           0             }, /* 22.43 */
{ 201,    "VendorIdentifier",                  8,      200,    (encode_hexstr),         (decode_hexstr),         3,           3             }, /* 22.43.8 */
{ 202,    "L2VPNEncoding",                     5,      200,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 22.43.5 */
{ 203,    "VPNIdentifier",                     1,      202,    (encode_string),         (decode_string),         4,           255           }, /* 22.43.5.1 */

/* TODO: Vendor Specific support in the IEEE802Classifier */

{ 54,     "DsPacketClass",                     23,     0,      (encode_nothing),        (decode_aggregate),      0,           0             },
{ 55,     "ClassifierRef",                     1,      54,     (encode_uchar),          (decode_uchar),          1,           255           },
{ 56,     "ClassifierId",                      2,      54,     (encode_ushort),         (decode_ushort),         1,           65535         },
{ 57,     "ServiceFlowRef",                    3,      54,     (encode_ushort),         (decode_ushort),         1,           65535         },
{ 58,     "ServiceFlowId",                     4,      54,     (encode_uint),           (decode_uint),           1,           0xFFFFFFFF    },
{ 59,     "RulePriority",                      5,      54,     (encode_uchar),          (decode_uchar),          0,           255           },
{ 60,     "ActivationState",                   6,      54,     (encode_uchar),          (decode_uchar),          0,           1             },
{ 61,     "DscAction",                         7,      54,     (encode_uchar),          (decode_uchar),          0,           2             },
{ 62,     "IpPacketClassifier",                9,      54,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 63,     "IpTos",                             1,      62,     (encode_hexstr),         (decode_hexstr),         3,           3             },
{ 64,     "IpProto",                           2,      62,     (encode_ushort),         (decode_ushort),         0,           257           },
{ 65,     "IpSrcAddr",                         3,      62,     (encode_ip),             (decode_ip),             0,           0             },
{ 66,     "IpSrcMask",                         4,      62,     (encode_ip),             (decode_ip),             0,           0             },
{ 67,     "IpDstAddr",                         5,      62,     (encode_ip),             (decode_ip),             0,           0             },
{ 68,     "IpDstMask",                         6,      62,     (encode_ip),             (decode_ip),             0,           0             },
{ 69,     "SrcPortStart",                      7,      62,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 70,     "SrcPortEnd",                        8,      62,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 71,     "DstPortStart",                      9,      62,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 72,     "DstPortEnd",                        10,     62,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 73,     "LLCPacketClassifier",               10,     54,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 74,     "DstMacAddress",                     1,      73,     (encode_ethermask),      (decode_ethermask),      0,           0             },
{ 75,     "SrcMacAddress",                     2,      73,     (encode_ether),          (decode_ether),          0,           0             },
{ 76,     "EtherType",                         3,      73,     (encode_hexstr),         (decode_hexstr),         0,           255           },
{ 77,     "IEEE802Classifier",                 11,     54,     (encode_nothing),        (decode_aggregate),      0,           0             },
{ 78,     "UserPriority",                      1,      77,     (encode_ushort),         (decode_ushort),         0,           0             },
{ 79,     "VlanID",                            2,      77,     (encode_ushort),         (decode_ushort),         0,           0             },
{ 220,    "PcIPv6PacketClassification",        12,     54,     (encode_nothing),        (decode_aggregate),      0,           0             }, /* 23.12 */
{ 221,    "PcIPv6TrafficClassRangeAndMask",    1,      220,    (encode_hexstr),         (decode_hexstr),         0,           0             }, /* 23.12.1 */
{ 222,    "PcIPv6FlowLabel",                   2,      220,    (encode_hexstr),         (decode_hexstr),         4,           4             }, /* 23.12.2 */
{ 223,    "PcIPv6NextHeaderType",              3,      220,    (encode_ushort),         (decode_ushort),         0,           257           }, /* 23.12.3 */
{ 224,    "PcIPv6SourceAddress",               4,      220,    (encode_ip6),            (decode_ip6),            0,           0             }, /* 23.12.4 */
{ 225,    "PcIPv6SourcePrefixLength",          5,      220,    (encode_uchar),          (decode_uchar),          0,           128           }, /* 23.12.5 */
{ 226,    "PcIPv6DestAddress",                 6,      220,    (encode_ip6),            (decode_ip6),            0,           0             }, /* 23.12.6 */
{ 227,    "PcIPv6DestPrefixLength",            7,      220,    (encode_uchar),          (decode_uchar),          0,           128           }, /* 23.12.7 */
/* TLV 23 - Downstream Packet Classification */
{ 204,    "VendorSpecific",                    43,     54,     (encode_nothing),        (decode_aggregate),      0,           0             }, /* 23.43 */
{ 205,    "VendorIdentifier",                  8,      204,    (encode_hexstr),         (decode_hexstr),         3,           3             }, /* 23.43.8 */
{ 206,    "L2VPNEncoding",                     5,      204,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 23.43.5 */
{ 207,    "VPNIdentifier",                     1,      206,    (encode_string),         (decode_string),         4,           255           }, /* 23.43.5.1 */

/* Upstream Service Flow */

{ 80,     "UsServiceFlow",                     24,     0,      (encode_nothing),        (decode_aggregate),      0,           0             },
{ 81,     "UsServiceFlowRef",                  1,      80,     (encode_ushort),         (decode_ushort),         1,           65535         },
{ 82,     "UsServiceFlowId",                   2,      80,     (encode_uint),           (decode_uint),           1,           0xFFFFFFFF    },
{ 83,     "ServiceClassName",                  4,      80,     (encode_strzero),        (decode_strzero),        2,           16            },
{ 84,     "QosParamSetType",                   6,      80,     (encode_uchar),          (decode_uchar),          0,           255           },
{ 85,     "TrafficPriority",                   7,      80,     (encode_uchar),          (decode_uchar),          0,           7             },
{ 86,     "MaxRateSustained",                  8,      80,     (encode_uint),           (decode_uint),           0,           0             },
{ 87,     "MaxTrafficBurst",                   9,      80,     (encode_uint),           (decode_uint),           0,           0             },
{ 88,     "MinReservedRate",                   10,     80,     (encode_uint),           (decode_uint),           0,           0             },
{ 89,     "MinResPacketSize",                  11,     80,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 90,     "ActQosParamsTimeout",               12,     80,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 91,     "AdmQosParamsTimeout",               13,     80,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 92,     "MaxConcatenatedBurst",              14,     80,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 93,     "SchedulingType",                    15,     80,     (encode_uchar),          (decode_uchar),          0,           6             },
{ 94,     "RequestOrTxPolicy",                 16,     80,     (encode_hexstr),         (decode_hexstr),         4,           4             },
{ 95,     "NominalPollInterval",               17,     80,     (encode_uint),           (decode_uint),           0,           0             },
{ 96,     "ToleratedPollJitter",               18,     80,     (encode_uint),           (decode_uint),           0,           0             },
{ 97,     "UnsolicitedGrantSize",              19,     80,     (encode_ushort),         (decode_ushort),         0,           65535         },
{ 98,     "NominalGrantInterval",              20,     80,     (encode_uint),           (decode_uint),           0,           0             },
{ 99,     "ToleratedGrantJitter",              21,     80,     (encode_uint),           (decode_uint),           0,           0             },
{ 100,    "GrantsPerInterval",                 22,     80,     (encode_uchar),          (decode_uchar),          0,           127           },
{ 101,    "IpTosOverwrite",                    23,     80,     (encode_hexstr),         (decode_hexstr),         2,           2             },
/* TLV 24 Upstream Service Flow */
{ 192,    "VendorSpecific",                    43,     80,     (encode_nothing),        (decode_aggregate),      0,           0             }, /* 24.43 */
{ 194,    "L2VPNEncoding",                     5,      192,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 24.43.5 */
{ 195,    "VPNIdentifier",                     1,      194,    (encode_string),         (decode_string),         4,           255           }, /* 24.43.5.1 */
{ 193,    "VendorIdentifier",                  8,      192,    (encode_hexstr),         (decode_hexstr),         3,           3             }, /* 24.43.8 */

/* Downstream Service Flow */

{ 102,    "DsServiceFlow",                     25,     0,      (encode_nothing),        (decode_aggregate),      0,           0             },
{ 103,    "DsServiceFlowRef",                  1,      102,    (encode_ushort),         (decode_ushort),         1,           65535         },
{ 104,    "DsServiceFlowId",                   2,      102,    (encode_uint),           (decode_uint),           1,           0xFFFFFFFF    },
{ 105,    "ServiceClassName",                  4,      102,    (encode_strzero),        (decode_strzero),        2,           16            },
{ 106,    "QosParamSetType",                   6,      102,    (encode_uchar),          (decode_uchar),          0,           255           },
{ 107,    "TrafficPriority",                   7,      102,    (encode_uchar),          (decode_uchar),          0,           7             },
{ 108,    "MaxRateSustained",                  8,      102,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 109,    "MaxTrafficBurst",                   9,      102,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 110,    "MinReservedRate",                   10,     102,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 111,    "MinResPacketSize",                  11,     102,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 112,    "ActQosParamsTimeout",               12,     102,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 113,    "AdmQosParamsTimeout",               13,     102,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 114,    "MaxDsLatency",                      14,     102,    (encode_uint),           (decode_uint),           0,           0             },
/* TLV 25 - Downstream Service Flow */
{ 196,    "VendorSpecific",                    43,     102,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 25.43 */
{ 197,    "VendorIdentifier",                  8,      196,    (encode_hexstr),         (decode_hexstr),         3,           3             }, /* 25.43.8 */
{ 198,    "L2VPNEncoding",                     5,      196,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 25.43.5 */
{ 199,    "VPNIdentifier",                     1,      198,    (encode_string),         (decode_string),         4,           255           }, /* 25.43.5.1 */

/* Payload Header Suppression - Appendix C.2.2.8 */

{ 115,    "PHS",                               26,     0,      (encode_nothing),        (decode_aggregate)   ,   0,           0             },
{ 116,    "PHSClassifierRef",                  1,      115,    (encode_uchar),          (decode_uchar),          1,           255           },
{ 117,    "PHSClassifierId",                   2,      115,    (encode_ushort),         (decode_ushort),         1,           65535         },
{ 118,    "PHSServiceFlowRef",                 3,      115,    (encode_ushort),         (decode_ushort),         1,           65535         },
{ 119,    "PHSServiceFlowId",                  4,      115,    (encode_uint),           (decode_uint),           1,           0xFFFFFFFF    },
{ 120,    "PHSField",                          7,      115,    (encode_hexstr),         (decode_hexstr),         1,           255           },
{ 121,    "PHSIndex",                          8,      115,    (encode_uchar),          (decode_uchar),          1,           255           },
{ 122,    "PHSMask",                           9,      115,    (encode_hexstr),         (decode_hexstr),         1,           255           },
{ 123,    "PHSSize",                           10,     115,    (encode_uchar),          (decode_uchar),          1,           255           },
{ 124,    "PHSVerify",                         11,     115,    (encode_uchar),          (decode_uchar),          0,           1             },

{ 125,    "MaxClassifiers",                    28,     0,      (encode_ushort),         (decode_ushort),         0,           0             },
{ 126,    "GlobalPrivacyEnable",               29,     0,      (encode_uchar),          (decode_uchar),          0,           0             },

/* ManufacturerCVC */

{ 129,    "MfgCVCData",                        32,     0,      (encode_hexstr),         (decode_hexstr),         0,           255           },
{ 170,    "ManufacturerCVC",                   32,     0,      (encode_nothing),        (decode_hexstr),         0,           255           },
{ 171,    "CoSignerCVCData",                   33,     0,      (encode_hexstr),         (decode_hexstr),         0,           255           },
{ 172,    "CoSignerCVC",                       33,     0,      (encode_nothing),        (decode_hexstr),         0,           255           },

/* SNMPv3 Kickstart */
{ 132,    "SnmpV3Kickstart",                   34,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 34 */
/* TODO: SP-RFI-v2.0 says the SecurityName is UTF8 encoded */
{ 133,    "SnmpV3SecurityName",                1,      132,    (encode_string),         (decode_string),         1,           16            }, /* 34.1 */
{ 134,    "SnmpV3MgrPublicNumber",             2,      132,    (encode_hexstr),         (decode_hexstr),         1,           514           }, /* 34.2 */

{ 154,    "SubMgmtControl",                    35,     0,      (encode_hexstr),         (decode_hexstr),         3,           3             },
{ 155,    "SubMgmtFilters",                    37,     0,      (encode_ushort_list),    (decode_ushort_list),    4,           4             },

/* Snmpv3 Notification Receiver */
{ 135,    "SnmpV3TrapReceiver",                38,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 38 */
{ 136,    "SnmpV3TrapRxIP",                    1,      135,    (encode_ip),             (decode_ip),             0,           0             },
{ 137,    "SnmpV3TrapRxPort",                  2,      135,    (encode_ushort),         (decode_ushort),         0,           0             },
{ 138,    "SnmpV3TrapRxType",                  3,      135,    (encode_ushort),         (decode_ushort),         1,           5             },
{ 139,    "SnmpV3TrapRxTimeout",               4,      135,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 140,    "SnmpV3TrapRxRetries",               5,      135,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 141,    "SnmpV3TrapRxFilterOID",             6,      135,    (encode_oid),            (decode_oid),            1,           5             },
{ 142,    "SnmpV3TrapRxSecurityName",          7,      135,    (encode_string),         (decode_string),         1,           16            },

{ 143,    "DocsisTwoEnable",                   39,     0,      (encode_uchar),          (decode_uchar),          0,           1             },

{ 158,    "DsChannelList",                     41,     0,      (encode_nothing),        (decode_aggregate),      1,           255           },
{ 159,    "SingleDsChannel",                   1,      158,    (encode_nothing),        (decode_aggregate),      1,           255           },
{ 160,    "SingleDsTimeout",                   1,      159,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 161,    "SingleDsFrequency",                 2,      159,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 162,    "DsFreqRange",                       2,      158,    (encode_nothing),        (decode_aggregate),      1,           255           },
{ 163,    "DsFreqRangeTimeout",                1,      162,    (encode_ushort),         (decode_ushort),         0,           65535         },
{ 164,    "DsFreqRangeStart",                  2,      162,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 165,    "DsFreqRangeEnd",                    3,      162,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 166,    "DsFreqRangeStepSize",               4,      162,    (encode_uint),           (decode_uint),           0,           0xFFFFFFFF    },
{ 167,    "DefaultScanTimeout",                3,      158,    (encode_ushort),         (decode_ushort),         0,           65535         },

/* DOCSIS Extension Field - Vendor Specific */
{ 130,    "VendorSpecific",                    43,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 43 */
{ 188,    "L2VPNEncoding",                     5,      130,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 43.5 */
{ 189,    "VPNIdentifier",                     1,      188,    (encode_string),         (decode_string),         4,           255           }, /* 43.5.1 */
{ 190,    "NSIEncapsulation",                  2,      188,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* 43.5.2 */
{ 191,    "NSIEncapsulationSingleQTag",        2,      190,    (encode_ushort),         (decode_ushort),         1,           4095          }, /* 43.5.2.2 */
{ 191,    "NSIEncapsulationDualQTag",          3,      190,    (encode_dual_qtag),      (decode_dual_qtag),      0,           0             }, /* 43.5.2.3 */
{ 131,    "VendorIdentifier",                  8,      130,    (encode_hexstr),         (decode_hexstr),         3,           3             }, /* 43.8 */

{ 208,    "DUTFiltering",                      45,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 45 */
{ 209,    "DUTControl",                        1,      208,    (encode_uchar),          (decode_uchar),          0,           1             }, /* 45.1 */
{ 210,    "DUTCMIM",                           2,      208,    (encode_hexstr),         (decode_hexstr),         0,           0             }, /* 45.2 */

/* Channel Assignment Configuration Settings per Docsis3.1 CM-SP-MULPIv3.1-I01-131029 Annex C C.1.2.15 */
{ 185,    "ChannelAssignmentConfig",           56,     0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* 56 */
{ 186,    "CaTransmit",                        1,      185,    (encode_uchar),          (decode_uchar),          0,           255           },
{ 187,    "CaReceive",                         2,      185,    (encode_uint),           (decode_uint),           88000000,    860000000     },

/* IPv6 */
{ 173,    "SwUpgradeServer6",                  58,     0,      (encode_ip6),            (decode_ip6),            0,           0             },
{ 174,    "SubMgmtControl6",                   63,     0,      (encode_hexstr),         (decode_hexstr),         2,           2             },

/*{ 156, "SnmpMibObject",                    64, 0,    (encode_nothing),     (decode_snmp_object),    1,        2048       },*/

/* eRouter TLVs */
{ 175,    "eRouter",                           202,    0,      (encode_nothing),        (decode_aggregate),      0,           0             }, /* TLV 202 eRouter-I12 */
{ 176,    "InitializationMode",                1,      175,    (encode_uchar),          (decode_uchar),          0,           3             }, /* TLV 202.1 eRouter-I12 Annex B.4.2 */
{ 177,    "TR69ManagementServer",              2,      175,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* TLV 202.2 eRouter-I12 Annex B.4.3 */
{ 178,    "EnableCWMP",                        1,      177,    (encode_uchar),          (decode_uchar),          0,           1             }, /* TLV 202.2.1 eRouter-I12 Annex B.4.3.1 */
{ 179,    "URL",                               2,      177,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.2.2 eRouter-I12 Annex B.4.3.2 */
{ 180,    "Username",                          3,      177,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.2.3 eRouter-I12 Annex B.4.3.3 */
{ 181,    "Password",                          4,      177,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.2.4 eRouter-I12 Annex B.4.3.4 */
{ 182,    "ConnectionRequestUsername",         5,      177,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.2.5 eRouter-I12 Annex B.4.3.5 */
{ 183,    "ConnectionRequestPassword",         6,      177,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.2.6 eRouter-I12 Annex B.4.3.6 */
{ 184,    "ACSOverride",                       7,      177,    (encode_uchar),          (decode_uchar),          0,           1             }, /* TLV 202.2.7 eRouter-I12 Annex B.4.3.7 */
{ 227,    "InitializationModeOverride",        3,      175,    (encode_uchar),          (decode_uchar),          0,           1             }, /* TLV 202.3 eRouter-I12 Annex B.4.4 */
{ 228,    "SNMPv1v2cCoexistenceConfig",        53,     175,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* TLV 202.53 eRouter-I12 Annex B.4.5 */
{ 229,    "SNMPv1v2cCommunityName",            1,      228,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.53.1 eRouter-I12 Annex B.4.5.1 */
{ 230,    "SNMPv1v2cTransportAddressAccess",   2,      228,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* TLV 202.53.2 eRouter-I12 Annex B.4.5.2 */
{ 231,    "SNMPv1v2cAccessViewType",           3,      228,    (encode_uchar),          (decode_uchar),          1,           2             }, /* TLV 202.53.3 eRouter-I12 Annex B.4.5.3 */
{ 232,    "SNMPv1v2cAccessViewName",           4,      228,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.53.4 eRouter-I12 Annex B.4.5.4 */
{ 233,    "SNMPv3AccessViewConfiguration",     54,     175,    (encode_nothing),        (decode_aggregate),      0,           0             }, /* TLV 202.54 eRouter-I12 Annex B.4.6 */
{ 234,    "SNMPv3AccessViewName",              1,      233,    (encode_string),         (decode_string),         0,           0             }, /* TLV 202.54.1 eRouter-I12 Annex B.4.6.1 */
{ 235,    "SNMPv3AccessViewSubtree",           2,      233,    (encode_oid),            (decode_oid),            0,           0             }, /* TLV 202.54.2 eRouter-I12 Annex B.4.6.2 */
{ 236,    "SNMPv3AccessViewMask",              3,      233,    (encode_hexstr),         (decode_hexstr),         0,           0             }, /* TLV 202.54.3 eRouter-I12 Annex B.4.6.3 */
{ 237,    "SNMPv3AccessViewType",              4,      233,    (encode_uchar),          (decode_uchar),          1,           2             }, /* TLV 202.54.4 eRouter-I12 Annex B.4.6.4 */
{ 238,    "RATransmissionInterval",            10,     175,    (encode_ushort),         (decode_ushort),         3,           1800          }, /* TLV 202.10 eRouter-I12 Annex B.4.10 */

/* A little more organized -> Start with 239 */

/* Generic TLV ... we only use the limits, code and length don't matter ...*/
{ 998,    "GenericTLV",                        0, 0,           (encode_nothing),        (decode_special),        0,           0             },
/* PacketCable MTA Configuration File Delimiter  */
{ 157,    "MtaConfigDelimiter",                254,    0,      (encode_uchar),          (decode_uchar),          1,           255           },
{ 999,    "/*EndOfDataMkr*/",                  255,    0,      (encode_nothing),        (decode_special),        0,           0             }
};

#endif /* _DOCSIS_SYMTABLE_H */

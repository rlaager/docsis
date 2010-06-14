#!/bin/sh -e
# vim: ai ts=4 sts=4 et sw=4

# This script generates an HTML file which contains an overview of all
# configuration settings supported by DOCSIS, as defined in
# ../src/docsis_symtable.h.


if test $# -ne 2; then
echo "Usage: $0 <output_file> <package_version>"
exit 1;
fi


cat > $1 << EOF
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>DOCSIS Configuration Settings</title>
</head>
<body>
<h1>DOCSIS Configuration Settings</h1>
<p>In the table below you can find a list of all configuration settings understood by this program. The name should be self-descriptive; if not you can refer to the Appendix C of the DOCSIS RFI Specification for detailed explanations of what each of these mean.</p>
<p>For String and Hexadecimal Strings, the "Range" is the minimum and maximum length of the string. For "List" types, it represents the number of elements in the list.</p>
<p>This list was generated for docsis version $2.</p>
<table cellpadding="5">
<tr><td><b>Configuration Setting</b></td><td><b>Type</b></td><td><b>Range of values allowed</b></td></tr>
EOF

grep -e "^{" ../src/docsis_symtable.h \
	| grep -v "*" \
	| grep -v "decode_md5" \
	| awk '{print $5 "_" $3 " " $7 " " $8 " " $9}' \
	| sed 's/\"//g' \
	| sed 's/^0,_//g' \
	| sed 's/^[0-9]*,_/\&nbsp\;\&nbsp\;\&nbsp\;\&nbsp\;/g' \
	| sed 's/\decode_//g' \
	| sed 's/\ //g' | tr -s "\(\)" "%%" \
	| sed 's/^/<tr><td><b>/g' \
	| sed 's/,\%/<\/b><\/td><td>/g' \
	| sed 's/%,/<\/td><td>/g' \
	| sed 's/,/-/g' \
	| sed 's/$/<\/td><\/tr>/g'\
	| sed 's/uchar/Unsigned Integer/g' \
	| sed 's/uint/Unsigned Integer/g' \
	| sed 's/ushort_list/Unsigned Integer List - comma separated/g' \
	| sed 's/ushort/Unsigned Integer/g' \
	| sed 's/0xFFFFFFFF/4294967296/g' \
	| sed 's/ip/IP address/g' \
	| sed 's/ether/Ethernet MAC address/g' \
	| sed 's/ethermask/Ethernet MAC address mask/g' \
	| sed 's/hexstr/Hexadecimal characters/g' \
	| sed 's/string/String - double quote delimited/g' \
	| sed 's/strzero/String - double quote delimited/g' \
	| sed 's/aggregate/Aggregate/g' \
	| sed 's/vspecific/Aggregate/g' \
	| sed 's/oid/SNMP Object Identifier - numeric/g' \
	| sed 's/snmp_wd/SNMP Write Control - OID, value/g' \
	| sed 's/snmp_object/SNMP MIB Object- OID, Type, Value/g' \
	| sed 's/0-0/Not Applicable/g' >> $1

echo "</table></body></html>" >> $1

#!/bin/sh

# This file is part of the docsis distribution. 
# Copyright (c) 2003, Evvolve SRL; All Rights Reserved.  

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/net-snmp/lib

failed=0;

version=`grep VERSION ../version.h| awk '{print $3}' | sed 's/\"//g' `
patch=`grep PATCHLEVEL ../version.h| awk '{print $3}' | sed 's/\"//g' `
echo "Starting docsis regression test - version $version.$patch"

if [ ! -d bin/$version.$patch ]; then 
	mkdir bin/$version.$patch; 
fi
if [ ! -d bin/$version.$patch/re ]; then 
	mkdir bin/$version.$patch/re; 
fi

for file in docsis*.cfg;
do
echo "Processing $file..."
prefix=`echo $file | sed 's/\.cfg//g'`
if (! ../docsis -e $file keyfile bin/$version.$patch/$prefix.bin >/dev/null 2>/dev/null); then 
	echo "FAILED - could not encode file $file"
	failed=1;
fi

if (! diff bin/$version.$patch/$prefix.bin bin/reference/$prefix.bin); then
	echo "FAILED - binary file encoded by version $version.$patch differs from reference"
	failed=1; 
fi

# now decode and re-encode; 

if (! ../docsis -d bin/$version.$patch/$prefix.bin > bin/$version.$patch/re/$prefix.cfg 2>/dev/null); then
	echo "FAILED - could not decode binary file $file encoded with version $version.$patch"
	failed=1;
fi

if (! ../docsis -e bin/$version.$patch/re/$prefix.cfg keyfile bin/$version.$patch/re/$prefix.bin >/dev/null 2>/dev/null); then 
	echo "FAILED - could not self-re-encode file $file"
	failed=1;
fi

if (! diff bin/$version.$patch/$prefix.bin bin/$version.$patch/re/$prefix.bin); then
	echo "FAILED - Cyclic encoding of file $file yields different results"
	failed=1; 
fi

done;

for file in packetcable*.cfg;
do
echo "Processing $file..."
prefix=`echo $file | sed 's/\.cfg//g'`
if (! ../docsis -p $file bin/$version.$patch/$prefix.bin >/dev/null 2>/dev/null); then 
	echo "FAILED - could not encode file $file"
	failed=1;
fi

if (! diff bin/$version.$patch/$prefix.bin bin/reference/$prefix.bin); then
	echo "FAILED - binary file encoded by version $version.$patch differs from reference"
	failed=1; 
fi

# now decode and re-encode; 

if (! ../docsis -d bin/$version.$patch/$prefix.bin > bin/$version.$patch/re/$prefix.cfg 2>/dev/null); then
	echo "FAILED - could not decode binary file $file encoded with version $version.$patch"
	failed=1;
fi

if (! ../docsis -p bin/$version.$patch/re/$prefix.cfg bin/$version.$patch/re/$prefix.bin >/dev/null 2>/dev/null); then 
	echo "FAILED - could not self-re-encode file $file"
	failed=1;
fi

if (! diff bin/$version.$patch/$prefix.bin bin/$version.$patch/re/$prefix.bin); then
	echo "FAILED - Cyclic encoding of file $file yields different results"
	failed=1; 
fi

done;

if [ $failed -gt 0 ]; then 
echo "docsis regression test failed" 
else
echo "docsis regression test successful"
fi

echo "rm -r ../examples/bin/$version.$patch"
rm -r ../examples/bin/$version.$patch


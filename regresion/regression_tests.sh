#!/bin/bash

FILES=*.regression
DOCSIS=../src/docsis
KEYFILE=../src/keyfile

for f in $FILES
do
    $DOCSIS -d $f > temporary
    $DOCSIS -e temporary $KEYFILE temporary.bin
    MD5SUM_1=$(md5sum $f)
    MD5SUM_1_array=($MD5SUM_1)
    MD5SUM_2=$(md5sum temporary.bin)
    MD5SUM_2_array=($MD5SUM_2)
    [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ] && echo "$f test FAILED." >> TestResults.txt
done

rm temporary
rm temporary.bin

if [ -f TestResults.txt ]; then
    cat TestResults.txt
    echo ">>> Some tests FAILES <<<"
    rm TestResults.txt
else
    echo ">>> All tests PASSED <<<"
fi

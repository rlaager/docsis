#!/bin/bash

FILES=*.txt
DOCSIS=../src/docsis
KEYFILE=key

for f in $FILES
do
  TEST=$(echo $f | cut -f1 -d.)
  $DOCSIS -e $TEST.txt $KEYFILE $TEST.cm.new
  if [ ! -f $TEST.cm.new ]; then
    echo "Test $TEST failed to create CM file on first pass.";
    exit -1;
  fi
  MD5SUM_1=$(md5sum $TEST.cm)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$(md5sum $TEST.cm.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CM file on first pass.";
    exit -1;
  fi
  $DOCSIS -d $TEST.cm.new > $TEST.conf.new
  if [ ! -f $TEST.conf.new ]; then
    echo "Test $TEST failed to create a conf file on second pass.";
    exit -1;
  fi
  MD5SUM_1=$(md5sum $TEST.conf)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$(md5sum $TEST.conf.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CONF file on second pass.";
    exit -1;
  fi
  $DOCSIS -e $TEST.conf.new $KEYFILE $TEST.cm.new
  if [ ! -f $TEST.cm.new ]; then
    echo "Test $TEST failed to create CM file on third pass.";
    exit -1;
  fi
  MD5SUM_1=$(md5sum $TEST.cm)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$(md5sum $TEST.cm.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CM file on third pass.";
    exit -1;
  fi
  rm $TEST*.new
done

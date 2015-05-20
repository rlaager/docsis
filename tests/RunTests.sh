#!/usr/bin/env bash

if [ "$1" == '--help' ]; then
  echo "";
  echo "  Usage: RunTests.sh [OPTIONS]";
  echo "     or: Runtests.sh --help";
  echo "";
  echo "  OPTIONS:";
  echo "    --allow-failed=TRUE    Failed tests will be ignored and execution will";
  echo "                           continue. Usefull in code coverage testing. Default";
  echo "                           value: FALSE (execution will stop when a test failed.)";
  echo "";
  exit -1;
fi

if [ "$1" == '--allow-failed=TRUE' ]; then
  ALLOW_FAILED=TRUE;
else
  ALLOW_FAILED=FALSE;
fi

MD5=$(which md5sum)

if [ ! $MD5 ]; then
  MD5='md5 -q';
else
  MD5='md5sum';
fi

FILES=*.txt
DOCSIS=../src/docsis
KEYFILE=key

ALL_TESTS=`ls -l *.txt | wc -l`;

for f in $FILES
do
  let i++;
  echo "Progress: $i / $ALL_TESTS";
  TEST=$(echo $f | cut -f1 -d.)
  $DOCSIS -e $TEST.txt $KEYFILE $TEST.cm.new
  if [ ! -f $TEST.cm.new ]; then
    echo "Test $TEST failed to create CM file on first pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  MD5SUM_1=$($MD5 $TEST.cm)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$($MD5 $TEST.cm.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CM file on first pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  $DOCSIS -d $TEST.cm.new > $TEST.conf.new
  if [ ! -f $TEST.conf.new ]; then
    echo "Test $TEST failed to create a conf file on second pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  MD5SUM_1=$($MD5 $TEST.conf)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$($MD5 $TEST.conf.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CONF file on second pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  $DOCSIS -e $TEST.conf.new $KEYFILE $TEST.cm.new
  if [ ! -f $TEST.cm.new ]; then
    echo "Test $TEST failed to create CM file on third pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  MD5SUM_1=$($MD5 $TEST.cm)
  MD5SUM_1_array=($MD5SUM_1)
  MD5SUM_2=$($MD5 $TEST.cm.new)
  MD5SUM_2_array=($MD5SUM_2)
  if [ "${MD5SUM_1_array[0]}" != "${MD5SUM_2_array[0]}" ]; then
    echo "Test $TEST created a wrong CM file on third pass.";
    if [ $ALLOW_FAILED = FALSE ]; then
      exit -1;
    fi
  fi
  rm $TEST*.new
done

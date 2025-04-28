#!/bin/sh

BIN=../build/bin

test -d tests && cd tests
test -d in || {
    echo "ERROR not in directory: tests"
    exit 1
}

config_gen() {
    FILE1=in/example-$NAME.ini 
    FILE2=out/example-$NAME.ini
    set -x
    $BIN/pod_racer -d $FILE1 > out/config-declare.h
    $BIN/pod_racer -D $FILE1 > out/config-initialize.h
    $BIN/pod_racer -R $FILE1 > out/config-reader.cpp
}

ini_check() {
    FILE1=in/example-$NAME.ini 
    FILE2=out/example-$NAME.ini
    set -x
    $BIN/pod_racer -i $FILE1 > $FILE2
    sh ini-normalize.sh $FILE1 > $TMP1
    sh ini-normalize.sh $FILE2 > $TMP2
    diff -ub $TMP1 $TMP2
}

(NAME=small config_gen)
(NAME=big   config_gen)

TMP1=/tmp/_file1
TMP2=/tmp/_file2

(NAME=small ini_check)
(NAME=big   ini_check)




#!/bin/sh

FILE1=in/example1.ini
FILE2=out/example2.ini

TMP1=/tmp/_file1
TMP2=/tmp/_file2

BIN=../build/bin

test -d tests && cd tests
test -d in || {
    echo "ERROR not in directory: tests"
    exit 1
}

set -x
$BIN/pod_racer -d $FILE1 > out/config-declare.h
$BIN/pod_racer -D $FILE1 > out/config-initialize.h
$BIN/pod_racer -R $FILE1 > out/config-reader.cpp

$BIN/pod_racer -i $FILE1 > $FILE2
sh ini-normalize.sh $FILE1 > $TMP1
sh ini-normalize.sh $FILE2 > $TMP2
diff -ub $TMP1 $TMP2



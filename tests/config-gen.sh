
#!/bin/sh

BIN=../build/bin

test -d tests && cd tests
test -d in || {
    echo "ERROR not in directory: tests"
    exit 1
}

set -x
$BIN/pod_racer -d in/example1.ini > out/config-declare.h
$BIN/pod_racer -D in/example1.ini > out/config-initialize.h
$BIN/pod_racer -R in/example1.ini > out/config-reader.cpp



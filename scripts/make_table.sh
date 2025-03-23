#!/bin/sh

N=${1-${N-256}}

echo $N | awk '
{
    n = $1
    t[0] = 0
    for (i = 0; i < n; i++) {
        t[i] = i
    }
    for (i1 = 0; i1 < n; i1++) {
        i2 = int(rand() * n)
        v1 = t[i1]
        v2 = t[i2]
        t[i1] = v2
        t[i2] = v1
    }
    for (i = 0; i < n; i++) {
        printf("%u,", t[i]) | "fmt"
    } 
}'

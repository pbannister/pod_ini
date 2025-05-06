#!/bin/sh
##
## SECURITY:     This file is UNCLASSIFIED.
##
## LEGAL NOTICE: This file is proprietary to Technovative Applications.
##               All rights reserved as described in LegalNotice.txt.
##

STAMP_H=${STAMP_H-${1-version.h}}
N_BUILD=${N_BUILD-100}

echo "#define VERSION_BUILD $N_BUILD" >> $STAMP_H

N_BUILD=$( awk '/VERSION_BUILD/ { print 1 + $3 ; exit }' $STAMP_H )
S_STAMP=$(date +%Y.%m.%d)
X_STAMP=$(date +%Y%m%d)
S_BRANCH=$( git branch --show-current )
S_COMMIT=$( git rev-parse --short HEAD )
S_RELEASE="$( date +%Y.%m ).$N_BUILD"

cat > $STAMP_H <<XXXX
#pragma once
#define VERSION_BRANCH "$S_BRANCH"
#define VERSION_BUILD_S "$N_BUILD"
#define VERSION_COMMIT "$S_COMMIT"
#define VERSION_RELEASE "$S_RELEASE"
#define VERSION_STAMP "$S_STAMP"

#define VERSION_BUILD $N_BUILD
XXXX

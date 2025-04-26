#
#
#

all : # default target
.PSEUDO: all clean test

build : ; mkdir $@
build/Makefile : build ; cd build && cmake ..

all : build/Makefile ; cd build && make -j $( nproc )
clean : ; rm -rf build

test : ; cd build && make test

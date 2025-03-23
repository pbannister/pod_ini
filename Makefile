#
#
#

all : # default target
.PSEUDO: all clean

build : ; mkdir $@
build/Makefile : build ; cd build && cmake ../sources

all : build/Makefile ; cd build && make -j $( nproc )
clean : ; rm -rf build

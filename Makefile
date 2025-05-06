#
#
#

all : # default target
.PSEUDO: all clean test install uninstall

build : ; mkdir $@
build/Makefile : build ; cd build && cmake ..

all : build/Makefile ; cd build && make -j $( nproc )
clean : ; rm -rf build

install: all ; cd build && sudo make install
uninstall: ; sudo rm -r /usr/local/include/pod /usr/local/lib/pod /usr/local/bin/pod_racer

test : ; cd build && make test

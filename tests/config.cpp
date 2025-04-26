#include "out/config-declare.h"
#include "out/config-initialize.h"

#include <stdio.h>

int main(int, char**) {
    ::printf("sizeof(configuration_pod) = %lu\n", sizeof(configuration_pod));
    return 0;
}

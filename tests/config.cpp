#include "out/config-declare.h"
#include "out/config-initialize.h"

#include <stdio.h>

int main(int, char**) {
    ::printf("sizeof(configuration_pod) = %lu\n", sizeof(configuration_pod));
    ::printf("rule_allowed_sudo_options.validator=%s\n", configuration_pod.s_rule_allowed_sudo_options.v_validator);
    return 0;
}

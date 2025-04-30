#include "out/config-declare.h"
#include "out/config-initialize.h"
#include "pod/pod-valid.h"

#include <stdio.h>

int main(int, char**) {
    ::printf("sizeof(configuration_pod) = %lu\n", sizeof(configuration_pod));
    ::printf("rule_allowed_sudo_options.validator=%s\n", configuration_pod.s_panic.v_when);

    auto v = pod_valid::validator_o(configuration_pod.s_panic.v_when).as_integer().in_range(1, 10).limit_range(2, 6);
    ::printf("in: %s value: %li valid: %u\n", configuration_pod.s_panic.v_when, v.value, v.valid);

    return 0;
}

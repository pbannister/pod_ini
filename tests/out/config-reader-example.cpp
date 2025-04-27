//
//
#include "config-declare.h"
#include "pod/pod-racer.h"

struct configuration_pod_t::context_o : public pod_racer::configuration_context_o {
};

configuration_pod_t::~configuration_pod_t() {
    delete p_context;
}

bool configuration_pod_t::pod_load(const char *filename) {
    p_context = new context_o();
    if (!p_context->pod_load(filename)) {
        return false;
    }

    // TODO inject into POD.

    return true;
}

bool configuration_pod_t::pod_save(const char *) {
    return false;
}

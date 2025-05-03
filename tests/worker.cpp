#include "out/config-declare.h"
#include "out/config-initialize.h"
#include "pod/pod-valid.h"

#include "base/strings.h"
#include "base/timed.h"

#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

using base_strings::string_o;

struct worker_o {
    pthread_t id_thread = 0;
    unsigned i_thread = 0;
    os_clock::timed_t t_sleep = 0;
    unsigned n_iterations = 0;
    void worker_run() {
        id_thread = ::pthread_self();
        ::printf(
            "# worker: %p thread: %lu sleep: %6.2f\n",
            (void*) this, id_thread, t_sleep.get_seconds());
        string_o s1 = "foobar";
        for (unsigned i = 0; i < n_iterations; ++i) {
            string_o s2 = "zot-time";
            string_o s3 = s1;
            ::printf("# [%03u] %3u thread: %lu\n", i_thread, i, id_thread);
            os_clock::timed_sleep(t_sleep);
        }
    }
    static void* worker_start(void* p) {
        auto p_worker = ((worker_o*) p);
        p_worker->worker_run();
        return 0;
    }
};

static int want_verbose = 1;
int is_verbose(int n = 1) {
    return (n <= want_verbose);
}

int main(int, char**) {
    ::printf("sizeof(configuration_pod) = %lu\n", sizeof(configuration_pod));
    ::printf("rule_allowed_sudo_options.validator=%s\n", configuration_pod.s_panic.v_when);

    auto v = pod_valid::validator_o(configuration_pod.s_panic.v_when).as_integer().in_range(1, 10).limit_range(2, 6);
    ::printf("in: %s value: %li valid: %u\n", configuration_pod.s_panic.v_when, v.value, v.valid);

    const unsigned n_workers = 8;
    worker_o workers[n_workers];
    for (unsigned i = 0; i < n_workers; ++i) {
        ::printf("# start: %u\n", i);
        auto p_worker = (workers + i);
        p_worker->i_thread = (100 + i);
        p_worker->n_iterations = (3 + ((i + 4) % 9));
        p_worker->t_sleep = os_clock::timed_ms_t(1000 + (400 * (i % 3)));
        ::pthread_attr_t attr;
        ::pthread_attr_init(&attr);
        auto v = ::pthread_create(&p_worker->id_thread, &attr, worker_o::worker_start, p_worker);
        auto error = ((0 == v) ? 0 : errno);
        ::printf("# thread created: %i error: %u\n", v, error);
        ::pthread_attr_destroy(&attr);
    }

    for (unsigned i = 0; i < n_workers; ++i) {
        void* pv = 0;
        ::pthread_join(workers[i].id_thread, &pv);
        ::printf("# [%03u] done\n", i);
    }

    return 0;
}

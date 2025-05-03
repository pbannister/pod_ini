#pragma once
#include <stdint.h>
#include <time.h>

namespace os_clock {

static constexpr unsigned MS_PER_SECOND = 1000;
static constexpr unsigned US_PER_SECOND = (1000 * MS_PER_SECOND);
static constexpr unsigned NS_PER_SECOND = (1000 * US_PER_SECOND);

struct timed_ss_t {
    int64_t ns = 0;
    timed_ss_t(long n) :
        ns(n * NS_PER_SECOND) {
    }
};
struct timed_ms_t {
    int64_t ns = 0;
    timed_ms_t(long n) :
        ns(n * (NS_PER_SECOND / MS_PER_SECOND)) {
    }
};
struct timed_us_t {
    int64_t ns = 0;
    timed_us_t(long n) :
        ns(n * (NS_PER_SECOND / US_PER_SECOND)) {
    }
};
struct timed_ns_t {
    int64_t ns = 0;
    timed_ns_t(long n) :
        ns(n) {
    }
};

struct timed_t {
    int64_t ns;
    timed_t(timed_ns_t v) :
        ns(v.ns) {
    }
    timed_t(timed_us_t v) :
        ns(v.ns) {
    }
    timed_t(timed_ms_t v) :
        ns(v.ns) {
    }
    timed_t(double v) {
        ns = (NS_PER_SECOND * v);
    }
    int64_t get_ns() const {
        return ns;
    }
    int64_t get_us() const {
        return (ns / (NS_PER_SECOND / US_PER_SECOND));
    }
    int64_t get_ms() const {
        return (ns / (NS_PER_SECOND / MS_PER_SECOND));
    }
    int64_t get_ss() const {
        return (ns / (NS_PER_SECOND));
    }
    double get_seconds() const {
        return (ns * (1.0 / NS_PER_SECOND));
    }
};

bool timed_sleep(timed_t t) {
    timespec ts;
    ts.tv_nsec = (t.ns % NS_PER_SECOND);
    ts.tv_sec = (t.ns / NS_PER_SECOND);
    int v = ::clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, 0);
    return (0 == v);
}

};  // namespace os_clock
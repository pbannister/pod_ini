#pragma once
//
//
//
#include <stdlib.h>
#include <string.h>
#include "base/strings.h"

namespace pod_valid {
using base_strings::string_o;

struct valid_string_o {
    string_o value;
    bool valid = true;
    valid_string_o& limit_size(unsigned n) {
        auto n1 = ::strlen(value);
        if (n1 < n) {
            value.buffer_get()[n1] = 0;
        }
        return *this;
    }
};

struct valid_integer_o {
    long value = 0;
    bool valid = true;
    char* suffix = 0;
    valid_integer_o& in_range(int v1, int v2) {
        valid &= ((v1 <= value) && (value <= v2));
        return *this;
    }
    valid_integer_o& limit_range(int v1, int v2) {
        if (value < v1) {
            value = v1;
        }
        if (v2 < value) {
            value = v2;
        }
        return *this;
    }
    valid_integer_o& is_even() {
        valid &= (0 == (1 & value));
        return *this;
    }
    valid_integer_o& is_odd() {
        valid &= (1 == (1 & value));
        return *this;
    }
    valid_integer_o& make_even() {
        value &= (~1);
        return *this;
    }
    valid_integer_o& make_odd() {
        value |= 1;
        return *this;
    }
    valid_integer_o& no_suffix() {
        valid &= (0 == *suffix);
        return *this;
    }
    valid_integer_o& size_suffix() {
        switch (*suffix) {
        case 0:
            break;
        case 'k':
            value *= 1000;
            break;
        case 'K':
            value *= (1 << 10);
            break;
        case 'm':
            value *= 1000000;
            break;
        case 'M':
            value *= (1 << 20);
            break;
        case 'g':
            value *= 1000000000;
            break;
        case 'G':
            value *= (1 << 30);
            break;
        case 't':
            value *= 1000000000000L;
            break;
        case 'T':
            value *= (1L << 40);
            break;
        default:
            valid = false;
            break;
        }
        static char s_done[2] = {0};
        suffix = s_done;  // consumed
        return *this;
    }
};

struct valid_float_o {
    float value = 0;
    bool valid = true;
    char* suffix = 0;
    valid_float_o& in_range(int v1, int v2) {
        valid &= ((v1 <= value) && (value <= v2));
        return *this;
    }
    valid_float_o& limit_range(int v1, int v2) {
        if (value < v1) {
            value = v1;
        }
        if (v2 < value) {
            value = v2;
        }
        return *this;
    }
};

class validator_o {
    const char* s_value;

public:
    validator_o(const char* s) :
        s_value(s) {
    }
    valid_string_o as_string() {
        valid_string_o v;
        v.value = s_value;
        return v;
    }
    valid_integer_o as_integer(int base = 0) {
        valid_integer_o v;
        v.value = ::strtol(s_value, &v.suffix, base);
        return v;
    }
    valid_float_o as_float() {
        valid_float_o v;
        v.value = ::strtof(s_value, &v.suffix);
        return v;
    }
};

}  // namespace pod_valid

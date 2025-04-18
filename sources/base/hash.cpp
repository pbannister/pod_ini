#include "hash.h"
#include "strings.h"

#include <string.h>
#include <stdlib.h>

static constexpr unsigned N_256 = 256;
static unsigned T256[N_256] = {0};

using namespace base_strings;
using namespace base_hash;

hash256_o::hash256_o() {
    if (0 == T256[0]) {
        // Generate a random permutation of 0..255
        for (unsigned i = 0; i < N_256; ++i) {
            T256[i] = i;
        }
        for (unsigned i1 = 0; i1 < N_256; ++i1) {
            auto i2 = ((N_256 - 1) & ::rand());
            auto v1 = T256[i1];
            auto v2 = T256[i2];
            T256[i1] = v2;
            T256[i2] = v1;
        }
    }
}

hash256_o::~hash256_o() {
    for (unsigned i = 0; i < N_256; ++i) {
        delete pp_table[i];
        pp_table[i] = 0;
    }
}

static inline unsigned hash256(const char* p) {
    unsigned h = 0;  // seed
    for (; *p; ++p) {
        h = (h ^ T256[(h ^ *p) & 0xFF]);
    }
    return (255 & h);
}

bool hash256_o::hash_kv_add(const char* k, const char* v) {
    auto h = hash256(k);
    auto p1 = pp_table[h];
    if (!p1) {
        p1 = new node_o(k, v);
        pp_table[h] = p1;
        return true;
    }
    for (; p1; p1 = p1->p_next) {
        if (0 == ::strcmp(k, p1->key)) {
            p1->value = v;
            return true;
        }
    }
    auto p2 = new node_o(k, v);
    p2->p_next = pp_table[h];
    pp_table[h] = p2;
    return true;
}

const char* hash256_o::hash_kv_get(const char* k) {
    auto h = hash256(k);
    auto p = pp_table[h];
    if (!p) {
        return 0;
    }
    for (; p; p = p->p_next) {
        if (0 == ::strcmp(k, p->key)) {
            return p->value;
        }
    }
    return 0;
}

//
//
//

unsigned hash256_o::node_count() const {
    unsigned n = 0;
    for (unsigned i = 0; i < N_256; ++i) {
        for (auto p = pp_table[i]; p; p = p->p_next) {
            ++n;
        }
    }
    return n;
}

//
//
//

bool hash256_o::as_list(hash_list_o& list) {
    auto n_nodes = node_count();
    auto p_list = new node_p[n_nodes];
    unsigned i_node = 0;
    for (unsigned i = 0; i < N_256; ++i) {
        for (auto p = pp_table[i]; p; p = p->p_next) {
            p_list[i_node++] = p;
        }
    }
    list.n_nodes = n_nodes;
    list.pp_list = p_list;   
    return true;
}

base_hash::hash_list_o::~hash_list_o() {
    delete pp_list;
    pp_list = 0;
    n_nodes = 0;
}

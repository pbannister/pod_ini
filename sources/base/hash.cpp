#include "base/hash.h"
#include "base/strings.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

void hash256_o::as_list(hash_list_o& list) {
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
}

base_hash::hash_list_o::~hash_list_o() {
    delete pp_list;
    pp_list = 0;
    n_nodes = 0;
}

//
//  Sort the hash list.
//

void base_hash::hash_list_o::list_sort() {
    ::qsort(pp_list, n_nodes, sizeof(base_hash::node_p), [](const void* p1, const void* p2) {
        auto n1 = *(base_hash::node_p*) p1;
        auto n2 = *(base_hash::node_p*) p2;
        return ::strcmp(n1->key.buffer_get(), n2->key.buffer_get());
    });
}
//
//
//

static constexpr unsigned N_PARTS_MAX = 20;

struct key_parts_o {
    string_o parts;
    unsigned ac = 0;
    const char* av[N_PARTS_MAX + 1] = {0};
    key_parts_o(const char*);
};

key_parts_o::key_parts_o(const char* s) {
    parts = s;
    auto p1 = parts.buffer_get();
    for (ac = 0;;) {
        av[ac++] = p1;
        if (N_PARTS_MAX <= ac) {
            break;
        }
        p1 = ::strchr(p1, '.');
        if (!p1) {
            break;
        }
        *p1++ = 0;
    }
    av[ac] = 0;
}

//
//  Add list to tree.
//

void base_hash::counted_tree_o::tree_insert_node(node_p p_node) {
    key_parts_o k_node(p_node->key);
    auto p_check = this;
    unsigned i = 0;
    // Walk down the existing tree.
    for (; i < k_node.ac;) {
        const char* k1 = p_check->key.buffer_get();
        const char* k2 = k_node.av[i];
        if (0 == ::strcmp(k1, k2)) {
            ++i;  // consume the key
            if (!p_check->p_right) {
                break;
            }
            p_check = p_check->p_right;
        } else {
            if (!p_check->p_after) {
                p_check->p_after = new counted_tree_o(k_node.av[i++]);
                p_check = p_check->p_after;
                break;
            }
            p_check = p_check->p_after;
        }
    }
    // Must create remainder of branch.
    for (; i < k_node.ac; ++i) {
        p_check->p_right = new counted_tree_o(k_node.av[i]);
        p_check = p_check->p_right;
    }
    // Associate the value.
    p_check->p_node = p_node;
}

void base_hash::counted_tree_o::list_add(hash_list_o& list) {
    for (unsigned i = 0; i < list.n_nodes; ++i) {
        auto p = list.pp_list[i];
        tree_insert_node(p);
    }
}
static const char s_spaces[] =
    "| | | | | | | | | | | | | | | | | | | | | | | | | "  //
    "| | | | | | | | | | | | | | | | | | | | | | | | | "  //
    "| | | | | | | | | | | | | | | | | | | | | | | | | "  //
    "| | | | | | | | | | | | | | | | | | | | | | | | | "  //
    ;

void base_hash::counted_tree_o::tree_print(unsigned i1, unsigned i2) {
    {
        unsigned n_spaces = (sizeof(s_spaces) - 1);
        unsigned i_spaces = (n_spaces - ((i1 * 2) % n_spaces));
        auto p_spaces = (s_spaces + i_spaces);
        auto s_key = p_node->key.buffer_get();
        if (p_node) {
            auto s_value = p_node->value.buffer_get();
            ::printf("%4u: %s %s='%s'\n", i1, p_spaces, s_key, s_value);
        } else {
            ::printf("%4u: %s %s\n", i1, p_spaces, s_key);
        }
    }
    if (p_right) {
        p_right->tree_print(i1, 1 + i2);
    }
    if (p_after) {
        p_after->tree_print(1 + i1, i2);
    }
}

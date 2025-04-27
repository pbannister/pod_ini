#include "base/hash.h"
#include "base/strings.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//
//  Simple hash table using Pearson's hash function.
//

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

// Pearon's hash function.

static inline unsigned hash256(const char* p) {
    unsigned h = 0;  // seed
    for (; *p; ++p) {
        h = (h ^ T256[255 & (h ^ *p)]);
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

//  Count the nodes in the hash table.

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
//  Make a list from a hash table.
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
        return ::strcmp(n1->key, n2->key);
    });
}

//
//  Print the hash list.
//

void base_hash::hash_list_o::list_print() {
    for (unsigned i = 0; i < n_nodes; ++i) {
        auto p = pp_list[i];
        ::printf("[%u] %s = %s\n", i, p->key.buffer_get(), p->value.buffer_get());
    }
}

//
//  Split a '.' separated key into parts.
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
        auto p2 = ::strchr(p1, '.');
        if (!p2) {
            p2 = ::strchr(p1, '!');
            *p2++ = 0;
            av[ac++] = p2;
            break;
        }
        p1 = p2;
        *p1++ = 0;
    }
    av[ac] = 0;
}

//
//  Add items from a hash list into a INI-style sectioned tree.
//

struct base_hash::tree_root_o::value_o {
    node_o* p_node = 0;
    value_o* p_value_next = 0;
};

struct base_hash::tree_root_o::section_o {
    string_o key;
    section_o* p_section_next = 0;
    value_o* p_value_head = 0;
    value_o* p_value_tail = 0;
};

void base_hash::tree_root_o::list_add(hash_list_o& list) {
    string_o section1;
    for (unsigned i = 0; i < list.n_nodes; ++i) {
        auto p_node = list.pp_list[i];
        string_o section2(p_node->key);
        auto p = ::strrchr(section2.buffer_get(), '!');
        *p = 0;
        if (0 != ::strcmp(section1, section2)) {
            section1 = section2;
            auto p_section = new section_o();
            p_section->key = section1;
            if (!p_section_tail) {
                p_section_head = p_section;
                p_section_tail = p_section;
            } else {
                p_section_tail->p_section_next = p_section;
                p_section_tail = p_section;
            }
        }
        auto p_value = new value_o();
        p_value->p_node = p_node;
        if (!p_section_tail->p_value_head) {
            p_section_tail->p_value_head = p_value;
            p_section_tail->p_value_tail = p_value;
        } else {
            p_section_tail->p_value_tail->p_value_next = p_value;
            p_section_tail->p_value_tail = p_value;
        }
    }
}

//
//  Generate an INI file from the tree.
//

void base_hash::tree_root_o::tree_print_ini() {
    auto t_now = ::time(0);
    auto s_now = ::ctime(&t_now);
    ::printf("// Generated date: %s", s_now);
    for (auto p_section = p_section_head; p_section; p_section = p_section->p_section_next) {
        auto s_section = p_section->key.buffer_get();
        ::printf("\n[%s]\n", s_section);
        for (auto p_value = p_section->p_value_head; p_value; p_value = p_value->p_value_next) {
            auto s_key = p_value->p_node->key.buffer_get();
            auto s_value = p_value->p_node->value.buffer_get();
            auto s_k = (::strchr(s_key, '!') + 1);
            ::printf("%s='%s'\n", s_k, s_value);
        }
    }
    ::printf("\n");
}

//
//  Replace one character witn another in a string.
//

static void string_filter(char* s1, char c1, char c2) {
    for (; *s1; ++s1) {
        if (*s1 == c1) {
            *s1 = c2;
        }
    }
}

//
//  Quote characters in an string as needed for C++ source.
//

static void string_requote(string_o& s1, string_o& s2) {
    auto n = ::strlen(s1);
    s2.buffer_get(2 * n);
    auto p1 = s1.buffer_get();
    auto p2 = s2.buffer_get();
    for (;;) {
        auto c1 = *p1++;
        if (!c1) {
            break;
        }
        if ('"' == c1) {
            *p2++ = '\\';
            *p2++ = '"';
        } else if ('\\' == c1) {
            *p2++ = '\\';
            *p2++ = '\\';
        } else {
            *p2++ = c1;
        }
    }
    *p2 = 0;
}

//
//  Generate the structure definition for the POD.
//

void base_hash::tree_root_o::tree_print_pod_struct() {
    auto t_now = ::time(0);
    auto s_now = ::ctime(&t_now);
    ::printf(
        "#pragma once\n"
        "// Generated date: %s"
        "// Do not edit this file.\n"
        "\nextern struct configuration_pod_t {"
        "\n    struct context_o;"
        "\n    struct context_o* p_context;"
        "\n    bool pod_load(const char*);"
        "\n    bool pod_save(const char*);\n",
        s_now);
    for (auto p_section = p_section_head; p_section; p_section = p_section->p_section_next) {
        string_o section_name(p_section->key);
        string_filter(section_name.buffer_get(), '.', '_');
        ::printf("    struct configuration_pod_%s_t {\n", section_name.buffer_get());
        for (auto p_value = p_section->p_value_head; p_value; p_value = p_value->p_value_next) {
            auto key1 = p_value->p_node->key.buffer_get();
            auto key2 = (::strchr(key1, '!') + 1);
            ::printf("        const char* v_%s;\n", key2);
        }
        ::printf("    } s_%s;\n", section_name.buffer_get());
    }
    ::printf("\n} configuration_pod;\n");
}

//
//  Generate the initializer for the POD.
//

void base_hash::tree_root_o::tree_print_pod_initialize() {
    auto t_now = ::time(0);
    auto s_now = ::ctime(&t_now);
    ::printf(
        "#pragma once\n"
        "// Generated date: %s"
        "// Do not edit this file.\n"
        "\nstruct configuration_pod_t configuration_pod = {"
        "\n    0,",
        s_now);
    for (auto p_section = p_section_head; p_section; p_section = p_section->p_section_next) {
        string_o section_name(p_section->key);
        string_filter(section_name.buffer_get(), '.', '_');
        ::printf("    { // %s\n", section_name.buffer_get());
        for (auto p_value = p_section->p_value_head; p_value; p_value = p_value->p_value_next) {
            auto key1 = p_value->p_node->key.buffer_get();
            auto key2 = (::strchr(key1, '!') + 1);
            string_o quoted;
            string_requote(p_value->p_node->value, quoted);
            ::printf("        \"%s\", // %s\n", quoted.buffer_get(), key2);
        }
        ::printf("    },\n");
    }
    ::printf("\n};\n");
}

//
//  Generate a POD reader.
//

void base_hash::tree_root_o::tree_print_pod_reader() {
    auto t_now = ::time(0);
    auto s_now = ::ctime(&t_now);
    ::printf(
        "// Generated date: %s"
        "// Do not edit this file."
        "\n#include \"config-declare.h\""
        "\n#include \"pod/pod-racer.h\""
        "\n"
        "\nstruct configuration_pod_t::context_o : public pod_racer::configuration_context_o {};"
        "\n"
        "\nbool configuration_pod_t::pod_load(const char *) {"
        "\n    return false;"
        "\n}\n"
        "\nbool configuration_pod_t::pod_save(const char *) {"
        "\n    return false;"
        "\n}\n",
        s_now);

    // TODO
}
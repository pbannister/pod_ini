#pragma once
#include "strings.h"

namespace base_hash {

using base_strings::string_o;

struct node_o {
    node_o* p_next = 0;
    string_o key;
    string_o value;
    node_o(const char* k, const char* v) :
        key(k), value(v) {
    }
    ~node_o() {
        delete p_next;
        p_next = 0;
    }
};
typedef node_o* node_p;

//
//
//

struct hash_list_o {
    unsigned n_nodes = 0;
    node_o** pp_list = 0;
    ~hash_list_o();
    void list_sort();
    void list_print();
};

//
//  Small hash table for key-value pairs.
//

class hash256_o {
protected:
    node_p pp_table[256] = {0};

public:
    bool hash_kv_add(const char* k, const char* v);
    const char* hash_kv_get(const char* k);

public:
    unsigned node_count() const;
    void as_list(hash_list_o&);

public:
    hash256_o();
    ~hash256_o();
};

//
//  Tree of nodes created from a sort list.
//  Ordered by key in form for INI/PROPERTIES file.
//

class tree_root_o {
    struct section_o;
    struct value_o;
    section_o* p_section_head = 0;
    section_o* p_section_tail = 0;

public:
    void list_add(hash_list_o&);
    void tree_print_ini();
    void tree_print_pod_struct();
    void tree_print_pod_initialize();
    void tree_print_pod_reader();
};

}  // namespace base_hash

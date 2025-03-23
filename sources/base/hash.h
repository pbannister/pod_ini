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
    bool as_list(hash_list_o&);

public:
    hash256_o();
    ~hash256_o();
};

}  // namespace base_hash

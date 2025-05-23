#pragma once
//
//  Pod Racer - INI/Properties file reader and conversion to POD format. 
//
#include "base/strings.h"
#include "base/hash.h"

namespace pod_racer {

using base_hash::hash256_o;
using base_hash::hash_list_o;
using base_hash::node_p;
using base_strings::string_o;

//
//  Class to hold a hash table of key/value pairs.
//  Methods for verification and conversion to POD format.
//

class pod_hashtable_o {
public:
    hash256_o table;

public:
    void as_list(hash_list_o& list) {
        table.as_list(list);
    }
};

//
//  Class to read INI/PROPERTIES files and convert to POD format.
//

class pod_reader_o {
public:
    typedef hash256_o* hashtable_p;

protected:
    string_o filename;
    string_o key_section;
    hashtable_p p_table = 0;

protected:
    struct buffer_o {
        char* p_buffer = 0;
        unsigned n_room = 0;
        unsigned n_have = 0;
        void buffer_allocate(unsigned n_want);
        bool buffer_load(const char* filename);
        ~buffer_o();
    };

protected:
    buffer_o buffer;

public:
    bool file_read(const char* _name);

protected:
    bool scan_buffer();
    bool scan_line(char*, char*);
    bool scan_section_key(char*, char*);
    bool scan_key_value(char*, char*);

protected:
    void key_normalize(char*, char*);
    bool key_value_store(char*, char*);

public:
    pod_reader_o(pod_hashtable_o& o);
    ~pod_reader_o();
};

//
//
//

struct configuration_context_o {
    pod_racer::pod_hashtable_o table;
    pod_racer::pod_reader_o pod;
    base_hash::tree_root_o tree;
    bool file_read(const char*);
    bool tree_build();
    bool pod_load(const char*);
    configuration_context_o() :
        pod(table) {
    }
};

}  // namespace pod_racer

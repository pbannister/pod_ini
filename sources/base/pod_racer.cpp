#include "pod_racer.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

//
//  Simple file helper class.
//

struct file_o {
    int file = -1;
    int error = 0;
    bool file_open_read(const char* name) {
        file = ::open(name, O_RDONLY);
        if (file < 0) {
            error = errno;
            return false;
        }
        return true;
    }
    bool file_close() {
        auto v = ::close(file);
        if (v) {
            error = errno;
            return false;
        }
        file = -1;
        return true;
    }
    off_t file_size() {
        struct stat st;
        if (::fstat(file, &st)) {
            error = errno;
            return 0;
        }
        return st.st_size;
    }
    // Assumes anything other than a complete read is an error.
    bool file_read(void* p, size_t n) {
        auto v = ::read(file, p, n);
        if (v < 0) {
            error = errno;
            return false;
        }
        return (n == (size_t) v);
    }
    ~file_o() {
        file_close();
    }
};

using namespace pod_racer;

//
//  POD file reader.
//

bool pod_reader_o::file_read(const char* _name) {
    filename = _name;
    file_o file;
    if (!file.file_open_read(filename)) {
        return false;
    }
    unsigned bytes_wanted = file.file_size();
    p_buffer = new char[bytes_wanted + 1];
    bytes_buffer = bytes_wanted;
    if (!file.file_read(p_buffer, bytes_wanted)) {
        return false;
    }
    p_buffer[bytes_buffer] = '\n';  // ensure terminated.
    return scan_buffer();
}

//
//  POD file scanner.
//

bool pod_reader_o::scan_buffer() {
    const auto p_bob = (p_buffer);
    const auto p_eob = (p_buffer + bytes_buffer);
    for (auto p_bol = p_bob; p_bol < p_eob;) {
        auto p_eol = ::strchr(p_bol, '\n');
        *p_eol = 0;
        scan_line(p_bol, p_eol);
        p_bol = (p_eol + 1);
    }
    return false;
}

//
//  POD line scanner.
//

bool pod_reader_o::scan_line(char* p_bol, char* p_eol) {
    for (; ((p_bol < p_eol) && (*p_bol <= ' ')); ++p_bol) {
        // skip leading whitespace
    }
    for (auto p = (p_eol - 1); ((p_bol < p) && (*p <= ' ')); --p) {
        // skip trailing whitespace
        *p = 0;
        p_eol = p;
    }
    auto c1 = (*p_bol);
    switch (c1) {
    case 0:
        // empty line
        break;
    case '#':
        // Comment
        return true;
    case '[':
        scan_section_key(p_bol, p_eol);
        break;
    default:
        scan_key_value(p_bol, p_eol);
        break;
    }
    return false;
}

//
//  POD section key scanner.
//

bool pod_reader_o::scan_section_key(char* p_bol, char* p_eol) {
    // Extract section key between '[' and ']'
    if (']' == *(p_eol - 1)) {
        *p_eol-- = 0;  // drop ']'
    }
    ++p_bol;  // skip '['
    for (; (p_bol < p_eol) && (*p_bol <= ' '); ++p_bol) {
        // skip leading whitespace
    }
    for (; (p_bol < p_eol) && (*p_eol <= ' '); --p_eol) {
        // skip trailing whitespace
    }
    *p_eol = 0;
    // Process section key (p_bol now points to the section key)
    auto p_bok = p_bol;
    auto p_eok = p_eol;
    key_normalize(p_bok, p_eok);
    if (!*p_bol) {
        // Disallow an empty section key.
        key_section = "GLOBAL";
    } else {
        // We now have a normalized section key.
        key_section = p_bol;
    }
    // ::printf("Section: %s\n", key_section.buffer_get());
    return true;
}

//
//  POD key-value scanner.
//

bool pod_reader_o::scan_key_value(char* p_bol, char* p_eol) {
    auto p_eq = ::strchr(p_bol, '=');
    if (!p_eq) {
        // No assignment.
        return false;
    }
    auto p_bok = p_bol;
    auto p_eok = p_eq;
    *p_eq++ = 0;  // split key from value.
    auto p_bov = p_eq;
    auto p_eov = p_eol;
    // Trim whitespace from assignment.
    for (; (p_bok < p_eok) && (*(p_eok - 1) <= ' ');) {
        // drop trailing whitespace on key
        *--p_eok = 0;
    }
    for (; (p_bov < p_eov) && (*p_bov <= ' ');) {
        // drop leading whitespace on value
        *p_bov++ = 0;
    }
    key_normalize(p_bok, p_eok);
    // ::printf("[%s] Key: %s = %s\n", key_section.buffer_get(), p_bok, p_bov);
    return key_value_store(p_bok, p_bov);
}

//
//  POD key normalizer.
//

void pod_racer::pod_reader_o::key_normalize(char* p_bok, char* p_eok) {
    // Allow only characters that are acceptable in a C field name.
    char c2 = '.';
    auto p2 = p_bok;
    for (auto p1 = p_bok; p1 < p_eok; ++p1) {
        auto c1 = *p1;
        if (('.' == c2) && ::isdigit(c1)) {
            c2 = 0;
            *(p2 - 1) = '_';
            *p2++ = c1;
        } else if (::isalnum(c1)) {
            c2 = 0;
            *p2++ = c1;
        } else if (::isspace(c1) || ('_' == c1)) {
            if ('_' == c2) {
                // Only one '_' allowed
            } else {
                c2 = '_';
                *p2++ = c2;
            }
        } else {
            // Anything else is assumed a field-separator.
            if ('.' == c2) {
                // Only one '.' allowed
            } else {
                c2 = '.';
                *p2++ = c2;
            }
        }
    }
    *p2 = 0;
}

//
//  POD key-value store.
//

bool pod_reader_o::key_value_store(char* k, char* v) {
    string_o key = key_section;
    key.strcat(".");
    key.strcat(k);
    return p_table->hash_kv_add(key, v);
}

//
//  POD reader constructor.
//

pod_reader_o::pod_reader_o(pod_hashtable_o& o) :
    p_table(&o.table) {
    key_section = "GLOBAL";
}

//
//  POD reader destructor.
//

pod_reader_o::~pod_reader_o() {
    delete p_buffer;
    p_buffer = 0;
    bytes_buffer = 0;
}

//
//  POD sorted hash list.
//

void pod_racer::pod_hashtable_o::table_sort(hash_list_o& list) {
    if (!table.as_list(list)) {
        return;
    }
    ::qsort(list.pp_list, list.n_nodes, sizeof(base_hash::node_p), [](const void* p1, const void* p2) {
        auto n1 = *(base_hash::node_p*) p1;
        auto n2 = *(base_hash::node_p*) p2;
        return ::strcmp(n1->key.buffer_get(), n2->key.buffer_get());
    });
}

//
//  Print POD hash table (unsorted).
//

void pod_hashtable_o::table_print_unsorted() {
    base_hash::hash_list_o list;
    if (!table.as_list(list)) {
        return;
    }
    for (unsigned i = 0; i < list.n_nodes; ++i) {
        auto p = list.pp_list[i];
        ::printf("[%u] %s = %s\n", i, p->key.buffer_get(), p->value.buffer_get());
    }
}

//
//  Print POD hash table (sorted).
//

void pod_racer::pod_hashtable_o::table_print_sorted() {
    base_hash::hash_list_o list;
    table_sort(list);
    for (unsigned i = 0; i < list.n_nodes; ++i) {
        auto p = list.pp_list[i];
        ::printf("[%u] %s = %s\n", i, p->key.buffer_get(), p->value.buffer_get());
    }
}

//
//
//

void pod_racer::pod_hashtable_o::table_print_pod() {
    base_hash::hash_list_o list;
    table_sort(list);
    // string_o prefix1 = "";
    // for (unsigned i = 0; i < list.n_nodes; ++i) {
    //     auto kv = list.pp_list[i];
    //     string_o name = kv->key;
    //     string_o value = kv->value;
    //     auto p_path = name.buffer_get();
    //     auto p = ::strrchr(p_path, '.');
    //     *p = 0;
    //     auto p_leaf = (1 + p);
    //     //
    //     if (0 != ::strcmp(p_path, prefix1.buffer_get())) {
    //         //
    //     }
    // }
}

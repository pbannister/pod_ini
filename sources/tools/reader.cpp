#include "pod/pod-racer.h"
#include "base/hash.h"

#include "versions/build.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

struct options_o {
    int is_verbose = 0;
    int want_unsorted = 0;
    int want_sorted = 0;
    int want_counts = 0;
    int want_ini = 0;
    int want_pod_struct = 0;
    int want_pod_initializer = 0;
    int want_pod_reader = 0;
};

int is_verbose(int) {
    return 0;
}

options_o g_options;

//
//  Bit of usage information.
//

static void usage(char** av) {
    ::printf(
        "\nUsage: "
        "\n\t%s [options] <file> ..."
        "\n\nWhere options are:"
        "\n\t-v       Verbose output"
        "\n\t-u       Print unsorted hash table"
        "\n\t-s       Print sorted hash table"
        "\n\t-i       Print INI file format"
        "\n\t-d       Print POD struct declaration"
        "\n\t-D       Print POD struct initializer"
        "\n\t-R       Print POD struct reader"
        "\n\nReads POD files and prints the hash table."
        "\n",
        av[0]);
    exit(1);
}

//
//  Fetch options from command line.
//

bool options_get(int ac, char** av) {
    for (;;) {
        int c = ::getopt(ac, av, "vusidDR");
        if (c < 0) {
            break;
        }
        switch (c) {
        case 'v':
            g_options.is_verbose++;
            break;
        case 'u':
            g_options.want_unsorted++;
            break;
        case 's':
            g_options.want_sorted++;
            break;
        case 'i':
            g_options.want_ini++;
            break;
        case 'd':
            g_options.want_pod_struct++;
            break;
        case 'D':
            g_options.want_pod_initializer++;
            break;
        case 'R':
            g_options.want_pod_reader++;
            break;
        default:
            usage(av);
            return false;
        }
    }
    return true;
}

//
//  Main program.
//

int main(int ac, char** av) {
    ::fprintf(stderr, "# version: %s branch: %s commit: %s\n", VERSION_STAMP, VERSION_BRANCH, VERSION_COMMIT);

    if (!options_get(ac, av)) {
        return 1;
    }

    // Read all files into a hash table.
    pod_racer::pod_hashtable_o table;
    for (int i = optind; i < ac; ++i) {
        pod_racer::pod_reader_o pod(table);
        auto ok = pod.file_read(av[i]);
        if (!ok) {
            ::printf("ERROR cannot read file: %s\n", av[i]);
            return 1;
        }
    }

    // Convert the hash table to a list.
    base_hash::hash_list_o list;
    table.as_list(list);
    
    if (g_options.want_unsorted) {
        list.list_print();
    }
    
    // Sort the list.
    list.list_sort();
    
    if (g_options.want_sorted) {
        list.list_print();
    }
    
    // Create a tree from the list.
    base_hash::tree_root_o tree;
    tree.list_add(list);

    if (g_options.want_ini) {
        tree.tree_print_ini();
    }
    if (g_options.want_pod_struct) {
        tree.tree_print_pod_struct();
    }
    if (g_options.want_pod_initializer) {
        tree.tree_print_pod_initialize();
    }
    if (g_options.want_pod_reader) {
        tree.tree_print_pod_reader();
    }
    return 0;
}
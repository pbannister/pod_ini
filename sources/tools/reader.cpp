#include "pod/pod_racer.h"
#include "base/hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

struct options_o {
    int is_verbose = 0;
    int want_unsorted = 0;
    int want_sorted = 0;
    int want_counts = 0;
    int want_pod = 0;
    int want_tree = 0;
};

int is_verbose(int) {
    return 0;
}

options_o g_options;

//
//
//

static void usage(char** av) {
    ::printf(
        "\nUsage: "
        "\n\t%s [options] <file> ..."
        "\n\nWhere options are:"
        "\n\t-v       Verbose output"
        "\n\t-u       Print unsorted hash table"
        "\n\t-s       Print sorted hash table"
        "\n\t-c       Print counts for each key"
        "\n\t-p       Print POD hash table"
        "\n\t-t       Print tree"
        "\n\nReads POD files and prints the hash table."
        "\n",
        av[0]);
    exit(1);
}

//
//
//

bool options_get(int ac, char** av) {
    for (;;) {
        int c = ::getopt(ac, av, "vuscpt");
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
        case 'c':
            g_options.want_counts++;
            break;
        case 'p':
            g_options.want_pod++;
            break;
        case 't':
            g_options.want_tree++;
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
    if (!options_get(ac, av)) {
        return 1;
    }
    pod_racer::pod_hashtable_o table;
    for (int i = 1; i < ac; ++i) {
        pod_racer::pod_reader_o pod(table);
        pod.file_read(av[i]);
    }
    if (g_options.want_unsorted) {
        table.table_print_unsorted();
    }
    if (g_options.want_sorted) {
        table.table_print_sorted();
    }
    if (g_options.want_counts) {
        table.table_print_counts();
    }
    if (g_options.want_pod) {
        table.table_print_pod();
    }
    if (g_options.want_tree) {
        // table.table_print_tree();
    }
    return 0;
}
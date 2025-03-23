#include "base/pod_racer.h"
#include "base/hash.h"

#include <stdlib.h>
#include <stdio.h>

int is_verbose(int) {
    return 0;
}

//
//  Main program.
//

int main(int ac, char** av) {
    pod_racer::pod_hashtable_o table;
    for (int i = 1; i < ac; ++i) {
        pod_racer::pod_reader_o pod(table);
        pod.file_read(av[i]);
    }
    table.table_print_sorted();
    return 0;
}
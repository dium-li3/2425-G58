#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include <sys/resource.h>


int main (int argc, char **argv){
    if (argc != 3){
        printf("não incluiste os ficheiros direito.\n");
        return 1;
    }
    int r = trabalho (argc, argv);
    // Execute work
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    printf("Memory usage: %ld KB\n", r_usage.ru_maxrss);

    return r;
}
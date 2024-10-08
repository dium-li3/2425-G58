#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main (int argc, char **argv){
    if (argc != 3){
        printf("não incluiste os ficheiros direito.\n");
        return 1;
    }
    int r = trabalho (argc, argv);
    
    return r;
}
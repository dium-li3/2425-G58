#include <sys/stat.h>
#include <stdio.h>

#include "utils.h"

/*
    Cria uma diretoria, devolve -1 caso não tenha conseguido,
    provavelmente porque ela já existe.
*/
int make_dir (char *dir){
    int r = mkdir (dir, 0777);
    return r;
}

char *get_output_file(int N){
    char file[22];
    snprintf (file, 22, "command%d_output.txt", N);
    return file;
}

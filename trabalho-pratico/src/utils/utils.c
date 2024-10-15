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

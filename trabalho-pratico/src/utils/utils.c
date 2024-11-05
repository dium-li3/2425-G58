#include <string.h>
//#include <regex.h>
#include "utils.h"

/*
    Valida o campo lista dos CSVs.
*/
int valid_list(char *lista) {
    return (lista[0] == '[' && lista[strlen(lista)-1] == ']');
}
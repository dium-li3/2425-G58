#include <string.h>
#include <stdlib.h>
//#include <regex.h>
#include "utils.h"

/*
    Valida o campo lista dos CSVs.
*/
int valid_list(char *lista) {
    return (lista[0] == '[' && lista[strlen(lista)-1] == ']');
}

/*
    Dados n tokens, liberta a memoria usada por eles
    e pelo pointer que os guarda.
*/
void free_tokens(char **tokens, int n){
    for (int i = 0; i < n; i++){
        free (tokens[i]);
    }
    free (tokens);
}
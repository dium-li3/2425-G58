#ifndef UTILS_H
#define UTILS_H

int valid_list(char *lista);

/*
    Dados n tokens, liberta a memoria usada por eles
    e pelo pointer que os guarda.
*/
void free_tokens(char **tokens, int n);

#endif
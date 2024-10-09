#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

#include "users.h"
#include "user_manager.h"


/*
    Guarda os ids de uma lista guardados 
    numa linha para uma lista de verdade.
*/
GSList *store_list (char *line);

/*
    Guarda os ids de uma lista guardados 
    numa linha para uma lista de verdade.
*/
GSList *parse_list (char *line);

/*
    Separa uma linha nos seus tokens.
*/
char **parse_line (char *line, char **tokens);

/*
    Guarda as linhas lidas numa lista ligada ...
*/
GSList *parse_file (FILE *fp_Users);

/*
    Dado o path onde se encontram os 3 ficheiros .csv,
    devolve um pointer para os 3 file pointers dos ficheiros.
*/
FILE **fopen3Entities (char *path);

/*
    Para de ler dos 3 pointers guardados pelo pointer de pointers de ficheiros.
*/
void fclose3Entities (FILE **fp_entities);

int trabalho (int argc, char **argv);


#endif
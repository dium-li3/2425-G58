#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

#include "users.h"
#include "user_manager.h"

GSList *store_list (char *list);

User store_user_line (char *line);

User_Manager store_Users (FILE *fp_Users, User_Manager user_manager);


void store_entities (FILE **fp_entities, User_Manager user_manager);

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
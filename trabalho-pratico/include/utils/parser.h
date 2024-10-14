#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <glib.h>

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
void *parse_line (FILE *fp, void *(*Func)(char **), ssize_t *nRead);
/*
    Lê uma linha do ficheiro, e devolve os tokens dessa linha.
*/
int parse_1line_querie(FILE *fp, char **info);

/*
    Guarda as linhas lidas numa lista ligada ...
*/
GSList *parse_file (FILE *fp_Users);

/*
    Guarda 1 linha do ficheiro na line dada.
    Devolve caso não tenha lido nada do ficheiro.
*/
ssize_t parse_1line (FILE *fp, char **line);

//Devolva os anos passados de uma string que representa uma data.
short read_date_to_age (char *bd);

/*
    Dado o path onde se encontram os 3 ficheiros .csv,
    devolve um pointer para os 3 file pointers dos ficheiros.
*/
FILE **fopen3Entities (char *path);

/*
    Para de ler dos 3 pointers guardados pelo pointer de pointers de ficheiros.
*/
void fclose3Entities (FILE **fp_entities);

#endif
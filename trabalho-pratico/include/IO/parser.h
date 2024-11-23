#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <glib.h>

typedef struct parser *Parser;

Parser open_parser(char *path);

void close_parser(Parser p);

ssize_t get_nRead (Parser p);

void go_back_1line (Parser p);

/*
    Guarda os ids de uma lista guardados 
    numa linha para uma lista de verdade.
*/
GArray *store_list (char *line);

/*
    Guarda os ids de uma lista guardados 
    numa linha para uma lista de verdade.
*/
GArray *parse_list (char *line);

/*
    Separa uma linha nos seus tokens.
*/
void *parse_line (Parser p, void *(*Func)(char **));
/*
    Lê uma linha do ficheiro, e devolve os tokens dessa linha.
*/
int parse_1line_query(Parser p, char **info);

/*
    Guarda 1 linha do ficheiro na line dada.
    Para verificações, ver o p->nRead.
*/
void parse_1line (Parser p, char **line);


//Devolva os anos passados de uma string que representa uma data.
short read_date_to_age (char *bd);

/*
    Dado o path onde se encontram os 3 ficheiros .csv,
    devolve um pointer para os 3 paths dos ficheiros.
*/
char **path3Entities (char *path);

void free3Entities (char **fp_entities);

#endif
#ifndef ARTISTS_H
#define ARTISTS_H

#include <glib.h>
#include <stdio.h>

#include "output.h"

typedef struct artist *Artist;

// Devolve o id de um artista.
int get_art_id(Artist a);

int *get_art_id_pointer(Artist a);

// Devolve a country de um artista.
const char *get_art_country(Artist a);

int get_disc_duration(Artist a);

const char *get_art_name(Artist a);

//Devolve um array com os ids dos artistas que fazem parte de um grupo.
const GArray *get_group_id_constituent (Artist a);

char get_art_type_from_art(Artist a);

/*
    Escreve num ficheiro de output um resumo
    do artista, útil para a query 1.
*/
void print_art_res(Artist a, Output out);

void print_art_info(Artist a, Output out);

int compare_dur (gconstpointer a, gconstpointer b);

Artist create_artist_from_tokens(char **tokens);

void add_disc_duration(Artist a, int duration);

void add_1_album (Artist a);

/*
    Adiciona a receita inteira que um artista recebe
    por música ouvida ao seu total ganho.
*/
void add_recipe (Artist a);

/*
    Adiciona uma dada percentagem da receita por stream
    a um dado Artista.
*/
void add_percentage_recipe (Artist a, double percentage);

// Dá free do artista
void free_art(Artist a);

#endif
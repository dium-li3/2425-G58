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

// Dada uma string com o tipo do artista, devolve o caracter que o representa.
char get_art_type(char *art_type);

char get_art_type_from_art(Artist a);

void print_art_info(Artist a, char separador, Output out);

int compare_dur (gconstpointer a, gconstpointer b);

Artist create_artist_from_tokens(char **tokens);

void add_disc_duration(Artist a, int duration);

// Dá free do artista
void free_art(Artist a);

#endif
#ifndef ARTIST_H
#define ARTIST_H

#include <glib.h>

#include "artist.h"

typedef struct art_manager *Art_Manager;

typedef struct array_art *Arr_art;

Art_Manager create_art_manager();

void insert_art_by_id(Artist a, Art_Manager art_manager); 

Artist search_art_by_id(int id, Art_Manager art_manager);

void free_art_manager(Art_Manager art_manager);

Arr_art create_arr_art(Art_Manager art_manager, char *country);

void add_arr_art(gpointer *key, gpointer *value, gpointer *art_data);

int cmp_art (gconstpointer a, gconstpointer b);

void fill_arr_art(Art_Manager art_manager, char *country);

void free_arr_art(Arr_art arr_art);

void responde_querie2 (FILE *fp_queries, Art_Manager *art_manager);

#endif
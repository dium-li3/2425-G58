#ifndef ARTIST_MANAGER_H
#define ARTIST_MANAGER_H

#include <glib.h>

#include "artists.h"

typedef struct art_manager *Art_Manager;

typedef struct array_art *Arr_art;

Art_Manager create_art_manager();

void insert_artists_by_id(Artist a, Art_Manager art_manager); 

void order_duration (Art_Manager artist_manager);

/*
    Incrementa a receita total de um dado conjunto de artistas.
    No caso de artistas que são grupos, incrementa-se também a receita total
    dos seus membros por uma fração igual a 1 / nº membros.
*/
void add_recipe_artists (const GArray *artists, Art_Manager am);

void store_Artists (char *art_path, Art_Manager artists_manager);

gboolean all_artists_exist (const GArray *artists, Art_Manager am);

void add_dur_artists (const GArray *music_artists , int duration, Art_Manager am);

void add_1_album_to_artists (const GArray *album_artists, Art_Manager am);

/*
    Dá print do resumo de um artista com um dado id.
*/
void print_art_res_by_id (Art_Manager am, int id, Output out);

void print_N_country_art_info (Art_Manager am, char *country, int N, Output out);

void print_N_art_info (Art_Manager am, int N, Output out);

int length_arr_disc (Art_Manager am);

void free_art_manager(Art_Manager art_manager);


/*
Arr_art create_arr_art(Art_Manager art_manager, char *country);

void add_arr_art(gpointer *key, gpointer *value, gpointer *art_data);

int cmp_art (gconstpointer a, gconstpointer b);

void fill_arr_art(Art_Manager art_manager, char *country);

void free_arr_art(Arr_art arr_art);

void responde_querie2 (FILE *fp_queries, Art_Manager *art_manager);
*/
#endif
#ifndef ARTIST_H
#define ARTIST_H

#include <glib.h>

#include "artist.h"

// typedef struct artist_manager {
//     GHashTable *artist_id;
// } *Artist_Manager;

typedef struct art_manager *Art_Manager;

Art_Manager create_art_manager();

void insert_art_by_id(Artist a, Art_Manager art_manager); 

void store_Art (FILE *fp_Art, Art_Manager art_manager); // sei pra q serve n

Artist search_art_by_id(int id, Art_Manager art_manager);

void free_art_manager(Art_Manager art_manager);

void responde_querie2 (FILE *fp_queries, Art_Manager *art_manager);

#endif
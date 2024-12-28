#ifndef ALBUM_MANAGER_H
#define ALBUM_MANAGER_H

#include <glib.h>

#include "albums.h"
#include "artist_manager.h"

typedef struct album_manager *Album_Manager;

Album_Manager create_album_manager ();

void free_album_manager (Album_Manager am);

void insert_album_by_id (Album al, int id, Album_Manager album_manager);

gboolean album_exists (int id, Album_Manager am);

int store_Album (char *album_path, Album_Manager album_man, Art_Manager art_man);

#endif
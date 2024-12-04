#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <glib.h>

#include "history.h"
#include "user_manager.h"
#include "music_manager.h"
#include "artist_manager.h"

typedef struct history_manager *History_Manager;

History_Manager create_history_manager ();

void free_history_manager (History_Manager am);

void insert_history_by_id (History al, int id, History_Manager history_manager);

History search_history_by_id(int id, History_Manager history_manager);

void store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um);

int **get_matrix(History_Manager hm);

#endif
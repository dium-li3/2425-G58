#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <glib.h>

#include "users.h"
#include "music_manager.h"

typedef struct user_manager *User_Manager;

User_Manager create_user_manager();

User search_user_by_id(int id, User_Manager user_manager);

int search_user_index_by_id(int id, User_Manager um);

void store_Users (char *user_path, User_Manager user_manager, Music_Manager music_manager);

void print_user_res_by_id (User_Manager um, int id, Output out);

void free_user_manager(User_Manager um);

int get_total_users(User_Manager um);

char **get_users_ids (User_Manager um);

#endif
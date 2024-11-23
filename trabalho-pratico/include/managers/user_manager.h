#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <glib.h>

#include "users.h"
#include "music_manager.h"

typedef struct user_manager *User_Manager;

User_Manager create_user_manager();

void store_Users (char *user_path, User_Manager user_manager, Music_Manager music_manager);

void print_user_info_by_id (User_Manager um, int id, char separador, Output out);

void free_user_manager(User_Manager um);

#endif
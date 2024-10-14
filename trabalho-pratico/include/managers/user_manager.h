#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <glib.h>

#include "users.h"

typedef struct user_manager *User_Manager;

User_Manager create_user_manager();

void store_Users (FILE *fp_Users, User_Manager user_manager);

User search_user_by_id(int id, User_Manager user_manager);

void free_user_manager(User_Manager um);

#endif
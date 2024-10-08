#ifndef USER_H
#define USER_H

#include <glib.h>

#include "users.h"


//g_hash_table_get_keys (user_manager->users_by_id) dá-nos logo todos os ids.
typedef struct user_manager {
    GHashTable *users_by_id;
} *User_Manager;

User_Manager create_user_manager();

void insert_user_by_id(User u, User_Manager user_manager);  // o arg table é pointer ou não??

User search_user_by_id(int id, User_Manager user_manager);

void free_user_manager(User_Manager um);

#endif
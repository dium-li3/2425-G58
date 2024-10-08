#ifndef USER_H
#define USER_H

#include <glib.h>

#include "users.h"

typedef struct user_manager {
    GHashTable *users_by_id;
    GSList *users_id;

} *User_Manager;

User_Manager create_user_manager();

void insert_user_by_id(User u, User_Manager user_manager);  // o arg table é pointer ou não??

User search_user_by_id(int *id, GHashTable *table);

void clean_user_manager(User_Manager um);

#endif
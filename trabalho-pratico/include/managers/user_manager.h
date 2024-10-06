#ifndef USER_H
#define USER_H

#include <glib.h>

typedef struct user_manager {
    GHashTable users_by_id;
}User_Manager;


// GHashTable createHash ???

void insert_user_by_id(User *u, GHashTable table);  // o arg table é pointer ou não??

User* search_user_by_id(int id, GHashTable table);

#endif
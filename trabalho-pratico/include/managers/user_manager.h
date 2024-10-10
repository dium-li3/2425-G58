#ifndef USER_H
#define USER_H

#include <glib.h>

#include "users.h"

typedef struct user_manager *User_Manager;

User_Manager create_user_manager();

void insert_user_by_id(User u, User_Manager user_manager);  

int store_User (User_Manager user_manager, GSList *linhas);

void store_Users (FILE *fp_Users, User_Manager user_manager);

User search_user_by_id(int id, User_Manager user_manager);

void free_user_manager(User_Manager um);

void responde_querie1 (FILE *fp_queries, User_Manager um);

#endif
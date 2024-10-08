#include <glib.h>

#include "user_manager.h"

User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new (g_int_hash, g_int_equal); //hash
    um->users_id = NULL; //lligada com os ids
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    g_hash_table_insert (user_manager->users_by_id, &(u->username), u);
    g_slist_prepend (user_manager->users_id, &u->username);
}

User search_user_by_id(int *id, GHashTable *table){
    User u = g_hash_table_lookup(table, id);
    return u;
}

void clean_user_manager(User_Manager um);

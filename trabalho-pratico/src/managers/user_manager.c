#include <glib.h>

#include "user_manager.h"

User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_int_hash, g_int_equal, NULL, free_user); //hash
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    g_hash_table_insert (user_manager->users_by_id, &(u->username), u);
}

User search_user_by_id(int *id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, id);
    return u;
}

// void clean_user_hash(User_Manager user_manager){
//     GList *l = NULL;
//     l = g_hash_table_get_keys (user_manager->users_by_id);
//     GList *temp = NULL;
//     for (temp = l; temp != NULL; temp = temp->next){
//         User x = NULL;
//         x = search_user_by_id(&(x->username), user_manager);
//         g_hash_table_remove (user_manager->users_by_id, x);
//         free_user (x);
//     }
//     g_list_free (l);
// }

void clean_user_manager(User_Manager um){
    g_hash_table_destroy (um->users_by_id);
}

#include <glib.h>
#include <stdio.h>

#include "user_manager.h"
#include "sintatica.h"
#include "parser.h"
#include "users.h"
#include "music_manager.h"
#include "logica.h"
#include "output.h"

//g_hash_table_get_keys (user_manager->users_by_id) dá-nos logo todos os ids.
typedef struct user_manager {
    GHashTable *users_by_id;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_int_hash, g_int_equal, free, (void *)free_user); //hash
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    int *id = get_user_id_pointer(u);
    g_hash_table_insert (user_manager->users_by_id, id, u);
}

void store_Users (char *user_path, User_Manager user_manager, Music_Manager mm){
    Parser p = open_parser (user_path);
    Output out = open_out("resultados/users_errors.csv");
    User user = NULL;
    while (get_nRead (p) != -1){
        user = (User)parse_line (p, (void *)create_user_from_tokens);
        if (user != NULL){
            if (valid_musics(get_liked_musics (user), mm, get_user_age (user)))
                insert_user_by_id (user, user_manager);
            else {
                free_user(user);
                error_output (p, out);
                user = NULL;//pode-se apagar isto né?
        }
        }
        else{
            if (get_nRead (p) != -1){
                error_output (p, out);
            }
        }
    }
    gen_freq_acum (mm);
    close_parser (p);
    close_output (out);
}


User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, &id);
    return u;
}

void free_user_manager(User_Manager um){
    g_hash_table_destroy (um->users_by_id);
    free (um);
}

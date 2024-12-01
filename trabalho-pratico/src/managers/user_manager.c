#include <glib.h>
#include <stdio.h>

#include "user_manager.h"
#include "parser.h"
#include "users.h"
#include "music_manager.h"
#include "output.h"
#include "utils.h"

#define USER_ELEMS 8

//g_hash_table_get_keys (user_manager->users_by_id) dá-nos logo todos os ids.
typedef struct user_manager {
    GHashTable *users_by_id;
    GArray *users_ids;
    int user_number;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void *)free_user); //hash
    um->users_ids = g_array_new(FALSE, TRUE, sizeof(char *));
    um->user_number = 0;
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    int id = get_user_id(u);
    g_hash_table_insert (user_manager->users_by_id, GINT_TO_POINTER(id), u);
}

/*
    Incrementa número de users.
*/

int get_user_number(User_Manager um) {
    return um->user_number;
}

void store_Users (char *user_path, User_Manager user_manager, Music_Manager mm){
    Parser p = open_parser (user_path);
    if(p == NULL) {
        perror("store_Users(31)");
        exit(1);
    }

    Output out = open_out("resultados/users_errors.csv", ';');
    User user = NULL;
    const GArray *liked_musics = NULL;
    int i;
    char **tokens;
    tokens = parse_line(p, USER_ELEMS);
    for (tokens = parse_line(p, USER_ELEMS), i = 0; tokens != NULL; tokens = parse_line(p, USER_ELEMS),i++){
        user = create_user_from_tokens(tokens,i,user_manager->users_ids);
        if (user != NULL){
            liked_musics = get_liked_musics(user);
            user_manager->user_number++;
            if (all_musics_exist(liked_musics, mm)){
                add_like_genres(liked_musics, mm, get_user_age(user));
                insert_user_by_id (user, user_manager);
            }
            else {
                free_user(user);
                error_output (p, out);
            }
        }
        else
            error_output (p, out);
        free_tokens(tokens, USER_ELEMS);
    }
    gen_arr_freq_acum (mm);
    close_parser (p);
    close_output (out);
}

User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, GINT_TO_POINTER(id));
    return u;
}

int search_user_index_by_id(int id, User_Manager um){
    User u = search_user_by_id(id,um);
    int index = get_user_index(u);
    return index;
}

/*
    Dá print de um User cujo id é o dado.
*/
void print_user_res_by_id (User_Manager um, int id, Output out){
    User u = search_user_by_id(id, um);
    print_user_res(u, out);
}

void free_user_manager(User_Manager um){
    g_hash_table_destroy (um->users_by_id);
    g_array_free(um->users_ids, TRUE);
    free (um);
}
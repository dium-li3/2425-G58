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
    int total_users;
    char **users_ids;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void *)free_user); //hash
    um->users_ids = NULL;
    um->total_users = 0;
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

int get_total_users(User_Manager um) {
    return um->total_users;
}

// void cast_ids (User_Manager um, GArray *array_ids) {
//     int tam = um->total_users;

//     for (int i = 0;i<tam;i++) {
//         um->users_ids[i] = g_array_index(array_ids,char *,i);
//     }
// }

char **get_users_ids (User_Manager um){
    return um->users_ids; 
}

void store_Users (char *user_path, User_Manager user_manager, Music_Manager mm){
    Parser p = open_parser (user_path);
    if(p == NULL) {
        perror("store_Users(34)");
        exit(1);
    }

    Output out = open_out("resultados/users_errors.csv", ';');
    User user = NULL;
    const GArray *liked_musics = NULL;
    GArray *array_users_ids = g_array_new(FALSE, TRUE, sizeof(char *));
    int i;
    char **tokens;
    char *user_id;

    tokens = parse_line(p, USER_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, USER_ELEMS);
    for (tokens = parse_line(p, USER_ELEMS), i = 0; tokens != NULL; tokens = parse_line(p, USER_ELEMS)){
        user = create_user_from_tokens(tokens, i);
        if (user != NULL){
            user_id = strdup(tokens[0]);
            g_array_insert_val(array_users_ids, i++, user_id); // armazena o conteúdo do token
            liked_musics = get_liked_musics(user);
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
    user_manager->users_ids = (char**) array_users_ids->data;
    user_manager->total_users = array_users_ids->len;
    g_array_free(array_users_ids, FALSE);
    gen_arr_freq_acum (mm);
    close_parser (p);
    close_output (out);
}

User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, GINT_TO_POINTER(id));
    return u;
}

gboolean user_exists (int id, User_Manager um){
    User u = search_user_by_id (id, um);
    return u == NULL ? FALSE : TRUE;
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
    for (int i = 0; i < um->total_users; i++)
        free (um->users_ids[i]);
    free (um->users_ids);
    free (um);
}




int get_user_index_from_id (int user_id, User_Manager um){
    User u = search_user_by_id (user_id, um);
    return get_user_index (u);
}
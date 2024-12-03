#include <glib.h>
#include <stdio.h>

#include "user_manager.h"
#include "parser.h"
#include "users.h"
#include "music_manager.h"
#include "output.h"
#include "utils.h"

#define USER_ELEMS 8

typedef struct user_manager {
    GHashTable *users_by_id;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void *)free_user); //hash
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    int id = get_user_id(u);
    g_hash_table_insert (user_manager->users_by_id, GINT_TO_POINTER(id), u);
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
    char **tokens;
    tokens = parse_line(p, USER_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, USER_ELEMS);
    for (tokens = parse_line(p, USER_ELEMS); tokens != NULL; tokens = parse_line(p, USER_ELEMS)){
        user = create_user_from_tokens(tokens);
        if (user != NULL){
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
    gen_arr_freq_acum (mm);
    close_parser (p);
    close_output (out);
}

User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, GINT_TO_POINTER(id));
    return u;
}

const GArray *get_year_history_from_user_id(int user_id, int year, User_Manager um){
    User u = search_user_by_id (user_id, um);
    const GArray *history_ids = NULL;
    if (u != NULL)
        history_ids = get_year_history (u, year);
    return history_ids;
}

void add_year_history_id_to_user (User_Manager um, int user_id, int year, int history_id){
    User u = search_user_by_id (user_id, um);
    add_year_history_id(u, year, history_id);
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
    free (um);
}
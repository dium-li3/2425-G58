#include <glib.h>
#include <stdlib.h>

#include "history.h"
#include "parser.h"
#include "output.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "user_manager.h"
#include "utils.h"

#define HISTORY_ELEMS 6 

typedef struct history_manager{
    GHashTable *histories_by_id;
    int** matrix;
    int mat_size;
} *History_Manager;


History_Manager create_history_manager (){
    History_Manager hm = malloc (sizeof(struct history_manager));
    
    hm->histories_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void*)free_history);
    hm->matrix = NULL;
    
    return hm;
}


int **get_matrix(History_Manager hm) {
    return hm->matrix;

void insert_history_by_id (History h, int id, History_Manager history_manager){
    g_hash_table_insert(history_manager->histories_by_id, GINT_TO_POINTER(id), h);
}


History search_history_by_id(int id, History_Manager history_manager){
    History al = g_hash_table_lookup(history_manager->histories_by_id, GINT_TO_POINTER(id));
    return al;
}

void fill_matrix(int user_id, int music_id, User_Manager um, Music_Manager mm, History_Manager hm) {
   int row = search_user_index_by_id(user_id,um);
   int column = search_gen_index_by_id(music_id,mm);
   hm->matrix[row][column]++;
}

void store_History (char *history_path, History_Manager hm, Art_Manager am, Music_Manager mm, User_Manager um){
    Parser p = open_parser(history_path);
    if(p == NULL) {
        perror("store_history(39)");
        exit(1);
    }

    Output out = open_out("resultados/history_errors.csv", ';');
    History history = NULL;
    int id, user_id,music_id, year;
    char **tokens = NULL;
    const GArray *artist_ids;

    int row = get_total_users(um);
    int column = get_total_genres(mm);
    hm->mat_size = get_total_users(um);
    hm->matrix = calloc(row , sizeof(int*)); //estava a fazer sizeof(int)...
    for (int i = 0; i < row; i++) {
        hm->matrix[i] = calloc(column, sizeof(int));
    }
  
    tokens = parse_line (p, HISTORY_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, HISTORY_ELEMS);
    for (tokens = parse_line (p, HISTORY_ELEMS); tokens != NULL; tokens = parse_line (p, HISTORY_ELEMS)){
        history = create_history_from_tokens (tokens, &year);
        
        if (history != NULL){
            id = atoi (tokens[0]+1);
            user_id = atoi (tokens[1]+1);
            music_id = atoi (tokens[2]+1);
            insert_history_by_id (history, id, hm);
            fill_matrix(user_id, music_id, um, mm, hm);

            artist_ids = get_music_artists_from_id (get_history_music (history), mm);
            /*
            artist_ids = get_music_artists_copy_from_id (get_history_music (history), mm);
            set_artist_ids (history, artist_ids);
            */
            add_recipe_artists(artist_ids, am);
        }
        else
            error_output (p, out);
        
        free_tokens(tokens, HISTORY_ELEMS);
    }
    close_parser (p);
    close_output (out);
}

void free_history_manager (History_Manager hm){
    int rows = hm->mat_size;
    g_hash_table_destroy (hm->histories_by_id);
    for (int i = 0; i < rows; i++) {
        free(hm->matrix[i]);
    }
    free(hm->matrix);
    free (hm);
}

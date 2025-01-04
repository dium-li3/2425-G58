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
    int mat_size[2];
    char *history_file_path;
} *History_Manager;

History_Manager create_history_manager (){
    History_Manager hm = malloc (sizeof(struct history_manager));
    
    hm->histories_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void*)free_history);
    hm->matrix = NULL;
    hm->history_file_path = NULL;
    
    return hm;
}

int **get_matrix(History_Manager hm) {
    int id, gen, **m = calloc (hm->mat_size[0], sizeof (int *));
    for (id = 0; id < hm->mat_size[0]; id++){
        m[id] = calloc (hm->mat_size[1], sizeof (int));
        for (gen = 0; gen < hm->mat_size[1]; gen++)
            m[id][gen] = hm->matrix[id][gen];
    }
    return m;
}

const char *get_history_path (History_Manager hm){
    return hm->history_file_path;
}

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

    hm->history_file_path = strdup(history_path);

    Output out = open_out("resultados/history_errors.csv", ';');
    History history = NULL;
    int hist_id, user_id, music_id, year, month, day, week, dur, max_week = -1;
    char **tokens = NULL;
    const GArray *artist_ids;

    int row = get_total_users(um);
    int column = get_total_genres(mm);
    long file_pos;
    hm->mat_size[0] = row;
    hm->mat_size[1] = column;
    hm->matrix = calloc(row , sizeof(int*)); //estava a fazer sizeof(int)...
    for (int i = 0; i < row; i++) {
        hm->matrix[i] = calloc(column, sizeof(int));
    }
  
    tokens = parse_line (p, HISTORY_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, HISTORY_ELEMS);
    for (file_pos = get_file_pos(p), tokens = parse_line (p, HISTORY_ELEMS); tokens != NULL; tokens = parse_line (p, HISTORY_ELEMS)){
        history = create_history_from_tokens (tokens, file_pos, &hist_id, &user_id, &music_id, &year, &month, &day, &dur);
        if (history != NULL){   
            insert_history_by_id (history, hist_id, hm);
            fill_matrix(user_id, music_id, um, mm, hm);

            artist_ids = get_music_artists_from_id (music_id, mm);
            add_recipe_artists(artist_ids, am);
            add_year_history_id_to_user (um, user_id, year, hist_id); 

            week = calc_week(day, month, year);
            if(week > max_week) max_week = week;
            add_listening_time_artists(artist_ids, week, dur, am);
        }
        else
            error_output (p, out);
        file_pos = get_file_pos(p);
        free_tokens(tokens, HISTORY_ELEMS);
    }
    set_max_week(am, max_week);
    close_parser (p);
    close_output (out);
}

void free_matrix (History_Manager hm){
    int rows = hm->mat_size[0];
    for (int i = 0; i < rows; i++) {
        free(hm->matrix[i]);
    }
    free(hm->matrix);
}

void free_history_manager (History_Manager hm){
    g_hash_table_destroy (hm->histories_by_id);

    free (hm->history_file_path);
    free (hm);
}

void get_history_info (int history_id, int *listening_time, int *music_id, int *month, int *day, int *hour, History_Manager hm){
    int useless, history_pos;
    History h = search_history_by_id(history_id, hm);
    history_pos = get_history_pos (h);

    Parser hist_file = open_parser (hm->history_file_path);
    set_file_pos (hist_file, history_pos);
    char **tokens = parse_line (hist_file, HISTORY_ELEMS);

    *music_id = atoi (tokens[2] +1);
    *hour = read_timestamp_elements (tokens[3], &useless, month, day);
    *listening_time = calc_duration_s (tokens[4]);

    close_parser (hist_file);
    free_tokens(tokens, HISTORY_ELEMS);
}
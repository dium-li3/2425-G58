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
    GArray *artists_by_week;
} *History_Manager;


void free_artists_by_week(GArray **a){
    g_array_free(*a, TRUE);
}

History_Manager create_history_manager (){
    History_Manager hm = malloc (sizeof(struct history_manager));
    
    hm->histories_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void*)free_history);
    
    hm->artists_by_week = g_array_sized_new(FALSE, TRUE, sizeof(GArray*), 400);
    g_array_set_clear_func(hm->artists_by_week, (GDestroyNotify) free_artists_by_week);
    
    return hm;
}


void free_history_manager (History_Manager hm){
    g_hash_table_destroy (hm->histories_by_id);
    g_array_free(hm->artists_by_week, TRUE);
    free (hm);
}


void insert_history_by_id (History h, int id, History_Manager history_manager){
    g_hash_table_insert(history_manager->histories_by_id, GINT_TO_POINTER(id), h);
}

/*
    Procura o id do artista em questão.
    Se o encontrar, adiciona o tempo. Se não encontrar, insere no array um novo par.
*/
void insert_artwtime(GArray **week_array, int id, int t){
    int i;
    ArtWTime atual = NULL;

    for(i = 0; i < (*week_array)->len; i++) {
        atual = g_array_index(*week_array, ArtWTime, i);
        
        if(get_artwtime_id(atual) == id) {
            add_artwtime_lt(atual, t);
            break;
        }
    }

    if(i == (*week_array)->len) {
        atual = create_artwtime(id, t);
        g_array_insert_val(*week_array, (*week_array)->len, atual);
    }
}


History search_history_by_id(int id, History_Manager history_manager){
    History al = g_hash_table_lookup(history_manager->histories_by_id, GINT_TO_POINTER(id));
    return al;
}


void store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um){
    Parser p = open_parser(history_path);
    if(p == NULL) {
        perror("store_history(39)");
        exit(1);
    }

    Output out = open_out("resultados/history_errors.csv", ';');
    History history = NULL;
    int id, year, week, dur;
    char **tokens = NULL;
    const GArray *artist_ids;
    GArray **week_array = NULL;

    tokens = parse_line (p, HISTORY_ELEMS); //ignorar 1ª linha do ficheiro
    free_tokens(tokens, HISTORY_ELEMS);
    for (tokens = parse_line (p, HISTORY_ELEMS); tokens != NULL; tokens = parse_line (p, HISTORY_ELEMS)){
        history = create_history_from_tokens (tokens, &year);
        
        if (history != NULL){
            id = atoi (tokens[0]+1);
            insert_history_by_id (history, id, history_man);
            artist_ids = get_music_artists_from_id (get_history_music (history), mm);
            /*
            artist_ids = get_music_artists_copy_from_id (get_history_music (history), mm);
            set_artist_ids (history, artist_ids);
            */
            add_recipe_artists(artist_ids, am);


            week = calc_week(get_history_day(history), get_history_month(history), year);
            week_array = &(g_array_index(history_man->artists_by_week, GArray*, week));

            if(*week_array == NULL) { //inicializar o array de uma semana caso este ainda não exista
                *week_array = g_array_new(FALSE, TRUE, sizeof(ArtWTime));
                g_array_set_clear_func(*week_array, (GDestroyNotify) free_artwtime);
            }

            dur = get_history_dur(history);
            for(int i = 0; i < artist_ids->len; i++) {
                insert_artwtime(week_array, g_array_index(artist_ids, int, i), dur);
            }
        }
        else
            error_output (p, out);
        
        free_tokens(tokens, HISTORY_ELEMS);
    }
    close_parser (p);
    close_output (out);
}

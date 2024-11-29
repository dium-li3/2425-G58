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
} *History_Manager;

History_Manager create_history_manager (){
    History_Manager hm = malloc (sizeof(struct history_manager));
    hm->histories_by_id = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, (void*)free_history);
    return hm;
}

void free_history_manager (History_Manager am){
    g_hash_table_destroy (am->histories_by_id);
    free (am);
}

void insert_history_by_id (History al, int id, History_Manager history_manager){
    g_hash_table_insert(history_manager->histories_by_id, GINT_TO_POINTER(id), al);
}

History search_history_by_id(int id, History_Manager history_manager){
    History al = g_hash_table_lookup(history_manager->histories_by_id, GINT_TO_POINTER(id));
    return al;
}

void store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um){
    Parser p = open_parser(history_path);
    if(p == NULL) {
        perror("store_history(35)");
        exit(1);
    }

    Output out = open_out("resultados/history_errors.csv", ';');
    History history = NULL;
    int id, year;
    char **tokens = NULL;
    const GArray *artist_ids;
    tokens = parse_line (p, HISTORY_ELEMS);
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

        }
        else
            error_output (p, out);
        
        free_tokens(tokens, HISTORY_ELEMS);
    }
    close_parser (p);
    close_output (out);
}

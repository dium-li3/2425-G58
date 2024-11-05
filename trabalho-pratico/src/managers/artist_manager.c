#include <glib.h>
#include <stdio.h>

#include "artist_manager.h"
#include "artists.h"
#include "parser.h"
#include "output.h"

typedef struct art_manager
{
    GHashTable *art_by_id;
    GArray *art_by_dur;
} *Art_Manager;

/*
typedef struct array_art
{
    GArray *artists;
    int tamanho;
    char *country;
} *Arr_art;
*/

Art_Manager create_art_manager()
{
    Art_Manager am = malloc(sizeof(struct art_manager));
    am->art_by_id = g_hash_table_new_full(g_int_hash, g_int_equal, free, (void *)free_art);
    am->art_by_dur = g_array_new(FALSE, TRUE, sizeof(Artist));
    return am;
}

void insert_artists_by_id(Artist a, Art_Manager art_manager)
{
    int *id = get_art_id_pointer(a);
    g_hash_table_insert(art_manager->art_by_id, id, a);
}

void insert_artists_by_dur(Artist a, Art_Manager art_manager, int i){
    art_manager->art_by_dur = g_array_insert_val(art_manager->art_by_dur,i , a);
}


Artist search_artist_by_id(int id, Art_Manager art_manager)
{
    Artist a = g_hash_table_lookup(art_manager->art_by_id, &id);
    return a;
}

Artist search_artist_by_dur_country(Art_Manager am, char *country, int i){
    Artist a = g_array_index(am->art_by_dur, Artist, i);
    char *countri = get_art_country (a);
    if (strcmp (countri, country))
        a = NULL;
    free (countri);
    return a;
}

Artist search_artist_by_dur_indice(Art_Manager am, int i){
    Artist a = g_array_index(am->art_by_dur, Artist, i);
    return a;
}

void order_duration (Art_Manager artist_manager){
    g_array_sort(artist_manager->art_by_dur, compare_dur);
}

void store_Artists (char *art_path, Art_Manager artists_manager){
    Parser p = open_parser(art_path);
    Output out = open_out("resultados/artists_errors.csv");
    Artist artist = NULL;
    int i = 0;
    while (get_nRead(p) != -1){
        artist = parse_line (p, (void *)create_artist_from_tokens);
        if (artist != NULL){
            insert_artists_by_id (artist, artists_manager);
            insert_artists_by_dur(artist, artists_manager, i++);
        }
        else{
            if (get_nRead(p) != -1)
                error_output (p, out);
        }
    }
    close_parser (p);
    close_output (out);
}

int length_arr_disc (Art_Manager am){
    return am->art_by_dur->len;
}

void free_art_manager(Art_Manager am)
{
    g_hash_table_destroy(am->art_by_id);
    g_array_free(am->art_by_dur, TRUE);
    free(am);
}
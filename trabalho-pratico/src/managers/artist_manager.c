#include <glib.h>
#include <stdio.h>

#include "artist_manager.h"
#include "artists.h"
#include "parser.h"
#include "output.h"
#include "utils.h"
#include "heaps.h"

#define ARTIST_ELEMS 7

typedef struct art_manager
{
    GHashTable *art_by_id;
    GArray *art_by_dur;
    int max_week;
} *Art_Manager;

Art_Manager create_art_manager()
{
    Art_Manager am = malloc(sizeof(struct art_manager));
    am->art_by_id = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (void *)free_art);
    am->art_by_dur = g_array_new(FALSE, TRUE, sizeof(Artist));
    return am;
}

void insert_artists_by_id(Artist a, Art_Manager art_manager)
{
    int id = get_art_id(a);
    g_hash_table_insert(art_manager->art_by_id, GINT_TO_POINTER(id), a);
}

void insert_artists_by_dur(Artist a, Art_Manager art_manager, int i){
    art_manager->art_by_dur = g_array_insert_val(art_manager->art_by_dur,i , a);
}

Artist search_artist_by_id(int id, Art_Manager art_manager)
{
    Artist a = g_hash_table_lookup(art_manager->art_by_id, GINT_TO_POINTER(id));
    return a;
}

/*
    Verifica se um artista com o dado id existe.
*/
gboolean artist_exists(int id, Art_Manager art_manager)
{
    gboolean r = TRUE;
    if (g_hash_table_lookup(art_manager->art_by_id, GINT_TO_POINTER(id)) == NULL)
        r = FALSE;
    return r;
}

gboolean all_artists_exist (const GArray *artists, Art_Manager am){
    gboolean r = TRUE;
    int len = -1;

    if (artists != NULL) {
        len = artists->len;
        for (int i = 0; i < len && r; i++){
            r = artist_exists (g_array_index(artists, int, i), am);
        }
    }
  
    return r;
}

void add_disc_dur_artists (const GArray *music_artists , int duration, Art_Manager am){
    Artist a = NULL;
    
    if(music_artists != NULL) {
        int len = music_artists->len;
        for (int i = 0; i < len; i++){
            a = search_artist_by_id (g_array_index(music_artists, int, i), am);
            add_disc_duration(a, duration);
        }
    }
}

void add_listening_time_artists(const GArray *artists, int week, int time, Art_Manager am){
    Artist a = NULL;

    for(int i = 0; i < artists->len; i++) {
        a = search_artist_by_id(g_array_index(artists, int, i), am);
        add_list_time(a, week, time);
    }
}

void set_max_week(Art_Manager am, int mw){
    am->max_week = mw;
}

int get_max_week(Art_Manager am){
    return am->max_week;
}

/*
    Percorre o array de artistas do Art_Manager e calcula o top 10 de uma determinada semana.
*/
void calc_top10_week(GArray *artists, int week) {
    int i, count, len = artists->len;
    Heap h = heap_new(TOP, compare_listening_time, NULL, &week);
    Artist a = NULL;

    //encher a heap com os artistas que têm tempo de reprodução nesta semana
    for(i = 0, count = 0; count < TOP && i < len; i++) {
        a = g_array_index(artists, Artist, i);
        if(get_week_listening_time(a, week) > 0) {
            heap_add(h, a);
            count++;
        }
    }

    //percorrer o resto do array, atualizando o top 10 se aparecer algum artista com tempo de reprodução
    for(; i < len; i++) {
        a = g_array_index(artists, Artist, i);
        if(get_week_listening_time(a, week) > 0) heap_swap_fst_elem(h, a);
    }
    
    int size;
    Artist *array = (Artist*) heap_unwrap_array(h, &size);

    for(i = 0; i < size; i++)
        mark_top10(array[i], week);
    
    free(array);
}

void calc_top10s(Art_Manager am) {
    int i, mw = am->max_week; //len = am->art_by_dur->len;
    /*Artist a = NULL;

    //garantir que todos os artistas têm o array de semanas inicializado a 0 até à max week
    for(i = 0; i < len; i++) {
        a = g_array_index(am->art_by_dur, Artist, i);
        set_art_max_week(a, mw);
    }*/

    for(i = 0; i < mw; i++)
        calc_top10_week(am->art_by_dur, i);
}


void acc_freq_top10s(Art_Manager am) {
    int i, len = am->art_by_dur->len;
    Artist a = NULL;

    for(i = 0; i < len; i++) {
        a = g_array_index(am->art_by_dur, Artist, i);
        acc_freq_top10_1art(a);
    }
}

int find_most_freq_top_art(int begin_week, int end_week, Art_Manager am, int *top_count) {
    int i, len = am->art_by_dur->len, ntops_begin, ntops_end, top_count_temp, art_id, id_temp;
    *top_count = 0;
    Artist a = NULL;

    art_id = id_temp = get_art_id(g_array_index(am->art_by_dur, Artist, 0));

    for(i = 0; i < len; i++) {
        a = g_array_index(am->art_by_dur, Artist, i);
        ntops_begin = (begin_week-1 < 0) ? get_week_listening_time(a, 0) : get_week_listening_time(a, begin_week-1);
        ntops_end = get_week_listening_time(a, end_week);
        
        if(ntops_begin != -1) {
            if(ntops_end == -1) ntops_end = get_art_max_top(a);
            top_count_temp = ntops_end - ntops_begin;
            
            if(top_count_temp == *top_count) {
                id_temp = get_art_id(a);
                if(id_temp < art_id) art_id = id_temp;
            }
            else if(top_count_temp > *top_count) {
                art_id = get_art_id(a);
                *top_count = top_count_temp;
            }
        }
    }

    return art_id;
}

void add_1_album_to_artists (const GArray *album_artists, Art_Manager am){
    Artist a = NULL;

    if (album_artists != NULL){
        int len = album_artists->len;
        for (int i = 0; i < len; i++){
            a = search_artist_by_id (g_array_index(album_artists, int, i), am);
            add_1_album (a);
        }
    }
}

/*
    Devolve o artista que está na posição pedida do array ordenado por discografia de artistas.
*/
Artist search_artist_by_dur_indice(Art_Manager am, int i){
    Artist a = g_array_index(am->art_by_dur, Artist, i);
    return a;
}

/*
    Verifica se o artista numa dada posição do array de artistas 
    ordenado por discografia pertence a um certo país.
*/
Artist search_artist_by_dur_country(Art_Manager am, char *country, int i){
    Artist a = g_array_index(am->art_by_dur, Artist, i);
    const char *countri = get_art_country (a);
    if (strcmp (countri, country))
        a = NULL;

    return a;
}

void print_art_res_by_id (Art_Manager am, int id, Output out){
    Artist a = search_artist_by_id(id, am);
    if (a!= NULL)
        print_art_res (a, out);
    else
        output_empty (out);
}

void print_N_art_info (Art_Manager am, int N, Output out){
    Artist a = NULL;
    for (int i = 0; i < N; i++){
        a = search_artist_by_dur_indice(am, i);
        print_art_info(a, out);
    }
}

void print_N_country_art_info (Art_Manager am, char *country, int N, Output out){
    Artist a = NULL;
    int len = am->art_by_dur->len;

    for (int i = 0; i < len && N > 0; i++){
        a = search_artist_by_dur_country(am, country, i);
        if (a != NULL){
            print_art_info(a, out);
            N--;
        }
    }
}

void print_most_freq_top_art(int art_id, int top_count, Art_Manager am, Output out) {
    Artist a = search_artist_by_id(art_id, am);

    print_top_count_art(a, top_count, out);
}

void order_duration (Art_Manager artist_manager){
    g_array_sort(artist_manager->art_by_dur, compare_dur);
}

void add_recipe_artists (const GArray *artists, Art_Manager am){
    Artist a = NULL, constintuent = NULL;
    int id, len, len_group;
    double percentage, group_recipe;
    const GArray *group_members = NULL;
    if (artists != NULL){
        len = artists->len;
        
        for (int i = 0; i < len; i++){
            id = g_array_index (artists, int, i);
            a = search_artist_by_id(id, am);

            if (get_art_type_from_art (a) == 'G'){ //caso particular de acrescentar a recipe aos membros de um grupo
                group_members = get_group_id_constituent (a);
                len_group = group_members->len;
                group_recipe = get_art_recipe_stream (a);
                percentage = (double) group_recipe / group_members->len;

                for (int j = 0; j < len_group; j++){
                    id = g_array_index (group_members, int, j);
                    constintuent = search_artist_by_id(id, am);
 
                    add_percentage_recipe (constintuent, percentage);
                }
            }
            add_recipe (a);//Qualquer artista (individual ou grupo) recebe uma recipe inteira
        }
    }
}

void store_Artists (char *art_path, Art_Manager artists_manager){
    Parser p = open_parser(art_path);
    if(p == NULL) {
        perror("store_Artists(146)");
        exit(1);
    }

    Output out = open_out("resultados/artists_errors.csv", ';');
    Artist artist = NULL;
    int i = 0;
    char **tokens;

    tokens = parse_line (p, ARTIST_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, ARTIST_ELEMS);
    for (tokens = parse_line (p, ARTIST_ELEMS); tokens != NULL ; tokens = parse_line (p, ARTIST_ELEMS)){
        artist = create_artist_from_tokens (tokens);
        if (artist != NULL){
            insert_artists_by_id (artist, artists_manager);
            insert_artists_by_dur(artist, artists_manager, i++);
            }
        else
                error_output (p, out);
        free_tokens(tokens, ARTIST_ELEMS);
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
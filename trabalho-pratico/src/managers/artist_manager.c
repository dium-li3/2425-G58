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

/*
Arr_art create_arr_art(Art_Manager art_manager, char *country)
{
    Arr_art arr_art = malloc(sizeof(struct array_art));

    arr_art->tamanho = 0;
    arr_art->country = country;
    arr_art->artists = g_array_new(FALSE, FALSE, sizeof(Artist));
}

void add_arr_art(gpointer *key, gpointer *value, gpointer *art_data)
{
    Arr_art arr_art = art_data;
    Artist *artist = (Artist *)value;

    if (strcmp(get_art_country (artist), arr_art->country) == 0 || strcmp(NULL, arr_art->country) == 0)
    {
        g_array_insert_val(arr_art->artists, arr_art->tamanho, artist);
        arr_art->tamanho++;
    }
}

int cmp_art(gconstpointer a, gconstpointer b)
{
    Artist artist_a = (Artist)a;
    Artist artist_b = (Artist)b;

    return (artist_b->disc_duration - artist_a->disc_duration);
}

void fill_arr_art(Art_Manager art_manager, Arr_art arr_art)
{
    g_hash_table_foreach(art_manager, &(add_arr_art), &arr_art);
    g_array_sort(arr_art->artists, cmp_art);
}

void free_arr_art(Arr_art arr_art)
{
    for (int i = 0; i < arr_art->tamanho; i++)
        g_free(g_array_index(arr_art->artists, Artist, i));

    g_array_free(arr_art->artists, TRUE);
    free(arr_art->country);
    free(arr_art);
}

void responde_querie2(FILE *fp_queries, Art_Manager am)
{
    ssize_t nBytes;
    size_t n;
    char *line = NULL;

    for (int i = 0; (nBytes = getline(&line, &n, fp_queries)) != -1; i++)
    {
        if (line[0] == '2')
        {
            char *n_str = strtok(line + 2, " ");
            int N = atoi(n_str);

            char *country_str = strtok(NULL, "\n");
            Arr_art arr_art = create_arr_art(am, country_str);
            
            fill_arr_art(am, arr_art);
            for (int j = 0; j < N; i++)
                print_art_info(g_array_index(arr_art->artists, Artist, j));
        }
    }
    free(line);
}
*/
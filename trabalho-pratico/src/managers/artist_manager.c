#include <glib.h>
#include <stdio.h>

#include "artist_manager.h"
#include "artists.h"
#include "parser.h"
#include "output.h"
#include "utils.h"

#define ARTIST_ELEMS 7

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

/*
    Verifica se todos os artistas num array de artistas existem.
*/
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

/*
    Adiciona uma dada duração à discocrafia de todos os artistas de um dado array.
*/
void add_dur_artists (const GArray *music_artists , int duration, Art_Manager am){
    Artist a = NULL;
    
    if(music_artists != NULL) {
        int len = music_artists->len;
        for (int i = 0; i < len; i++){
            a = search_artist_by_id (g_array_index(music_artists, int, i), am);
            add_disc_duration(a, duration);
        }
    }
}

/*
    Incrementa o número de albuns de todos os artistas
    cujos ids estão num array de ids de artistas.
*/
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

/*
    Dá print do resumo de um artista com um dado id.
*/
void print_art_res_by_id (Art_Manager am, int id, Output out){
    Artist a = search_artist_by_id(id, am);
    if (a!= NULL)
        print_art_res (a, out);
    else
        output_empty (out);
}

/*
    Dá print dos primeiros N artistas do array ordenado por discografia de artistas.
*/
void print_N_art_info (Art_Manager am, int N, Output out){
    Artist a = NULL;
    for (int i = 0; i < N; i++){
        a = search_artist_by_dur_indice(am, i);
        print_art_info(a, out);
    }
}

/*
    Dá print dos N primeiros artistas que pertencem a um dado país,
    pela ordem em que aparecem no array de artistas ordenado por discografia.
*/
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


/*
    Ordena um array de artistas por ordem decrescente de discografia.
*/
void order_duration (Art_Manager artist_manager){
    g_array_sort(artist_manager->art_by_dur, compare_dur);
}

/*
    Incrementa a receita total dos artistas cujos ids
    estão no array dado.
*/
void add_recipe_artists (const GArray *artists, Art_Manager am){
    Artist a = NULL, constintuent = NULL;
    int id, len, len_ids;
    double percentage;
    const GArray *ids = NULL;

    if (artists != NULL){
        len = artists->len;
        
        for (int i = 0; i < len; i++){
            id = g_array_index (artists, int, i);
            a = search_artist_by_dur_indice(am, id);

            if (get_art_type_from_art (a) == 'G'){ //caso particular de acrescentar a recipe aos membros de um grupo
                ids = get_group_id_constituent (a);
                len_ids = ids->len;
                percentage = 1 / ids->len; //hmmm dá o número correto???? rever PI lol

                for (int j = 0; j < len_ids; i++){
                    id = g_array_index (artists, int, i);
                    constintuent = search_artist_by_dur_indice(am, id);
 
                    add_percentage_recipe (constintuent, percentage);
                }
            }
            add_recipe (a);//Qualquer artista (individual ou grupo) recebe uma recipe inteira
        }
    }
}

/*
    Guarda num array e numa hashtable todos os artistas
    que estão num ficheiro cujo path é dado.
    No final o array ainda não está ordenado.

    Também dá print de algum artista que esteja incorreto.
*/
void store_Artists (char *art_path, Art_Manager artists_manager){
    Parser p = open_parser(art_path);
    if(p == NULL) {
        perror("store_Artists(146)");
        exit(1);
    }

    Output out = open_out("resultados/artists_errors.csv", ';');
    Artist artist = NULL;
    int i = 0;
    char **tokens = calloc(8, sizeof (char *));
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
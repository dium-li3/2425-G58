#include <glib.h>
#include <stdio.h>
#include "parser.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "output.h"
#include "genre.h"
#include "album_manager.h"
#include "utils.h"

#define MUSIC_ELEMS 8

typedef struct music_manager
{
    GHashTable *musics_by_id;
    GArray *genre_array;
    char** genre_names;
    int genre_number;
} *Music_Manager;


Music_Manager create_music_manager()
{
    Music_Manager mm = malloc(sizeof(struct music_manager));
    mm->musics_by_id = g_hash_table_new_full(g_direct_hash, g_direct_equal, FALSE, (void *)free_music);
    mm->genre_array = g_array_new(FALSE, TRUE, sizeof(Genre));
    g_array_set_clear_func(mm->genre_array, (GDestroyNotify) free_genre);
    return mm;
}

/*
    Devolve o Genre que está na dada posição do array de Genres
    do Music_Manager.
*/
Genre get_genre_by_index(Music_Manager mm,int index)
{
    Genre gen = g_array_index(mm->genre_array, Genre, index);
    return gen;
}

Music search_music_by_id(int id, Music_Manager music_manager)
{
    Music m = g_hash_table_lookup(music_manager->musics_by_id, GINT_TO_POINTER(id));
    return m;
}

//Devolve 1 caso tenha inserido um novo genero de musica.
gboolean insert_gen(Music m, Music_Manager mus_m, int i)
{
    gboolean r = TRUE;
    const char *gen = get_genre(m);
    int len = mus_m->genre_array->len;

    for (int i = 0; i < len && r; i++){
        r = compare_genre_names(get_genre_by_index(mus_m, i), gen);
    }

    if (r)
    {
        Genre gen_real = create_gen(gen);
        g_array_insert_val(mus_m->genre_array, i, gen_real);
    }
    
    return r;
}


void add_like_genre(Music_Manager mm, const char *genre, short age)
{
    gboolean adicionou = FALSE;
    int len = mm->genre_array->len;

    for (int i = 0; i < len && !adicionou; i++){
        if (compare_genre_names(get_genre_by_index(mm, i), genre) == 0)
        {
            increment_like (get_genre_by_index(mm, i), age);
            adicionou = TRUE;
        }
    }
}

void add_like_genres (const GArray *musics, Music_Manager mm, short age){
    Music m = NULL;
    const char *gen = NULL;
    int len = -1;

    if(musics != NULL) {
        len = musics->len;
        for (int i = 0; i < len; i++){
            m = search_music_by_id(g_array_index(musics, int, i), mm);
            gen = get_genre(m);
            add_like_genre(mm, gen, age);
        }
    }
}

void gen_arr_freq_acum(Music_Manager mm)
{
    int len = mm->genre_array->len;

    for (int i = 0; i < len; i++)
        gen_freq_acum (get_genre_by_index(mm, i));
}


/*
    
*/
void update_arr_total_likes(Music_Manager mm,int min_age, int max_age)
{
    int len = mm->genre_array->len;

    for (int i = 0; i < len; i++)
        update_gen_total_likes (get_genre_by_index(mm, i), min_age, max_age);
}

void sort_gen(Music_Manager mm,int min_age, int max_age)
{
    update_arr_total_likes(mm,min_age,max_age);
    g_array_sort(mm->genre_array, cmp_like_gen);
}

void insert_music_by_id(Music m, Music_Manager music_manager)
{
    int id = get_music_id(m);
    g_hash_table_insert(music_manager->musics_by_id, GINT_TO_POINTER(id), m);
}

/*
    Verifica se existe uma música guardada com o dado id.
*/
gboolean music_exists (int id, Music_Manager mm){
    gboolean r = TRUE;
    r = g_hash_table_lookup(mm->musics_by_id, GINT_TO_POINTER(id)) ? TRUE : FALSE;
    return r;
}

/*
    Verifica se as musicas de uma lista de ids de musicas
    pertencem todas às músicas que temos guardadas.
*/
gboolean all_musics_exist (const GArray *musics, Music_Manager mm){
    gboolean r = TRUE;
    int len = -1;

    if (musics != NULL) {
        len = musics->len;

        for (int i = 0; i < len && r; i++)
            r = music_exists (g_array_index(musics, int, i), mm);
    }

    return r;
}


const GArray *get_music_artists_from_id (int id, Music_Manager mm){
    Music m = search_music_by_id (id, mm);
    const GArray *artists_ids = get_music_artists (m);
    return artists_ids; 
}

/*GArray *get_music_artists_copy_from_id (int id, Music_Manager mm){
    Music m = search_music_by_id (id, mm);
    GArray *artists_ids = get_music_artists_copy (m);
    return artists_ids;
}*/


void store_Musics(char *music_path, Music_Manager mm, Art_Manager am, Album_Manager alm){
    Parser p = open_parser(music_path);
    if(p == NULL) {
        perror("store_Musics(145)");
        exit(1);
    }

    Output out = open_out("resultados/musics_errors.csv", ';');
    Music music = NULL;
    int i = 0, album_id;
    const GArray *music_artists = NULL;
    char **tokens;
    tokens = parse_line(p, MUSIC_ELEMS);
    for (tokens = parse_line(p, MUSIC_ELEMS); tokens != NULL; tokens = parse_line(p, MUSIC_ELEMS)){
        music = create_music_from_tokens(tokens);
        //Validação para saber se realmente guarda a entidade ou não
        if (music != NULL){//sintatica
            album_id = get_music_album (music);
            music_artists = get_music_artists(music);
            if (album_exists(album_id, alm) && all_artists_exist(music_artists, am))//logica
            {
                add_dur_artists (music_artists ,get_music_duration(music), am);
                insert_music_by_id(music, mm);
                if (insert_gen(music, mm, i))
                    i++;
            }
            else
            {
                free_music(music);
                error_output(p, out);
            }
        }
        else{
            error_output(p, out);
        }
        free_tokens(tokens, MUSIC_ELEMS);
    }
    close_parser(p);
    close_output(out);
}

void print_all_genres_info (Music_Manager mm, Output out){
    Genre gen = NULL;
    int escreveu = 0;
    int len = mm->genre_array->len;

    for (int i = 0; i < len; i++){
        gen = get_genre_by_index(mm, i);
        escreveu += print_genre_info(gen, out);
    }
    if (!escreveu)
        output_empty(out);
}

void free_music_manager(Music_Manager mm)
{
    g_hash_table_destroy(mm->musics_by_id);
    g_array_free(mm->genre_array, TRUE);
    free(mm);
}
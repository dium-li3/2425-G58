#include <glib.h>
#include <stdio.h>
#include "parser.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "output.h"
#include "genre.h"

typedef struct music_manager
{
    GHashTable *musics_by_id;
    GArray *genre_array;
} *Music_Manager;

//Devolve o número de elementos úteis do array de generos.
int get_gen_arr_len(Music_Manager mm)
{
    return mm->genre_array->len;
}

//Cria e inicializa um Music_Manager base, com tudo vazio.
Music_Manager create_music_manager()
{
    Music_Manager mm = malloc(sizeof(struct music_manager));
    mm->musics_by_id = g_hash_table_new_full(g_int_hash, g_int_equal, free, (void *)free_music);
    mm->genre_array = g_array_new(FALSE, TRUE, sizeof(Genre));
    g_array_set_clear_func(mm->genre_array, (GDestroyNotify) clear_genre);
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
    Music m = g_hash_table_lookup(music_manager->musics_by_id, &id);
    return m;
}

//Devolve 1 caso tenha inserido um novo genero de musica.
gboolean insert_gen(Music m, Music_Manager mus_m, int i)
{
    gboolean r = TRUE;
    char *gen = get_genre(m);
    for (int i = 0; i < mus_m->genre_array->len && r; i++){
        r = compare_genre_names(get_genre_by_index(mus_m, i), gen);
    }

    if (r)
    {
        Genre gen_real = create_gen(gen);
        g_array_insert_val(mus_m->genre_array, i, gen_real);
    }
    free(gen);
    return r;
}


void add_like_genre(Music_Manager mm, char *genre, short age)
{
    gboolean adicionou = FALSE;
    for (int i = 0; i < mm->genre_array->len && !adicionou; i++){
        if (compare_genre_names(get_genre_by_index(mm, i), genre) == 0)
        {
            increment_like (get_genre_by_index(mm, i), age);
            adicionou++;
        }
    }
}

void add_like_genres (GArray *musics, Music_Manager mm, short age){
    Music m = NULL;
    char *gen = NULL;
    for (int i = 0; i < musics->len; i++)
    {
        m = search_music_by_id(g_array_index(musics, int, i), mm);
        gen = get_genre(m);
        add_like_genre(mm, gen, age);
        free (gen);
    }
}

void gen_arr_freq_acum(Music_Manager mm)
{
    for (int i = 0; i < mm->genre_array->len; i++)
        gen_freq_acum (get_genre_by_index(mm, i));
}

void update_arr_total_likes(Music_Manager mm,int min_age, int max_age)
{
    for (int i = 0; i < mm->genre_array->len; i++)
        update_gen_total_likes (get_genre_by_index(mm, i), min_age, max_age);
}

void sort_gen(Music_Manager mm,int min_age, int max_age)
{
    update_arr_total_likes(mm,min_age,max_age);
    g_array_sort(mm->genre_array, cmp_like_gen);
}

void insert_music_by_id(Music m, Music_Manager music_manager)
{
    int *id = get_music_id_pointer(m);
    g_hash_table_insert(music_manager->musics_by_id, id, m);
}

/*
    Verifica se existe uma música guardada com o dado id.
*/
gboolean music_exists (int id, Music_Manager mm){
    gboolean r = TRUE;
    r = g_hash_table_lookup(mm->musics_by_id, &id) ? TRUE : FALSE;
    return r;
}

/*
    Verifica se as musicas de uma lista de ids de musicas
    pertencem todas às músicas que temos guardadas.
*/
gboolean all_musics_exist (GArray *musics, Music_Manager mm){
    gboolean r = TRUE;
    if (musics == NULL) return FALSE;
    
    for (int i = 0; i < musics->len && r; i++)
        r = music_exists (g_array_index(musics, int, i), mm);
    
    return r;
}

/*
    Armazena a informação das músicas dadas por um ficheiro de um dado path
    numa hashtable de Musicas e preenche um array de Genres sem nenhum repetido.

    Também aproveita e adiciona a duração de cada música aos seus artistas
    se as músicas forem válidas e então guardadas.

    Escreve todas as linhas de músicas inválidas num ficheiro.
*/
void store_Musics(char *music_path, Music_Manager mm, Art_Manager am){
    Parser p = open_parser(music_path);
    Output out = open_out("resultados/musics_errors.csv");
    Music music = NULL;
    int i = 0;
    GArray *music_artists = NULL;
    while (get_nRead(p) != -1){
        music = parse_line(p, (void *)create_music_from_tokens);
        if (music != NULL){
            music_artists = get_music_artists(music);
            if (all_artists_exist(music_artists, am))
            {
                add_dur_artists (music_artists ,get_music_duration(music), am);
                insert_music_by_id(music, mm);
                if (insert_gen(music, mm, i))
                    i++;
            }
            else
            {
                free_music(music);
                music = NULL;
                error_output(p, out);
            }
        }
        else{
            if (get_nRead(p) != -1)
                error_output(p, out);
        }
    }
    close_parser(p);
    close_output(out);
}

void print_all_genres_info (Music_Manager mm, Output out){
    Genre gen = NULL;
    int escreveu = 0;
    for (int i = 0; i < mm->genre_array->len; i++){
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
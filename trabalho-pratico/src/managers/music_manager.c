#include <glib.h>
#include <stdio.h>
#include "parser.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "logica.h"
#include "output.h"
#include "genre.h"

typedef struct music_manager
{
    GHashTable *musics_by_id;
    GArray *genre_array;
} *Music_Manager;

int get_gen_arr_len(Music_Manager mm)
{
    return mm->genre_array->len;
}

Music_Manager create_music_manager()
{
    Music_Manager mm = malloc(sizeof(struct music_manager));
    mm->musics_by_id = g_hash_table_new_full(g_int_hash, g_int_equal, free, (void *)free_music);
    mm->genre_array = g_array_new(FALSE, TRUE, sizeof(Genre));
    g_array_set_clear_func(mm->genre_array, (GDestroyNotify) clear_genre);
    return mm;
}

//Devolve 1 caso tenha inserido um novo genero de musica.
int insert_gen(Music m, Music_Manager mus_m, int i)
{
    int r = 1;
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
    int r = 0;
    for (int i = 0; i < mm->genre_array->len && !r; i++){
        if (compare_genre_names(get_genre_by_index(mm, i), genre) == 0)
        {
            increment_like (get_genre_by_index(mm, i), age);
            r++;
        }
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

Music search_music_by_id(int id, Music_Manager music_manager)
{
    Music m = g_hash_table_lookup(music_manager->musics_by_id, &id);
    return m;
}

void store_Musics(char *music_path, Music_Manager mm, Art_Manager am){
    Parser p = open_parser(music_path);
    Output out = open_out("resultados/musics_errors.csv");
    Music music = NULL;
    int i = 0;
    while (get_nRead(p) != -1){
        music = parse_line(p, (void *)create_music_from_tokens);
        if (music != NULL){
            if (valid_artists(get_music_artists(music), get_music_duration(music), am))
            {
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

void free_music_manager(Music_Manager mm)
{
    g_hash_table_destroy(mm->musics_by_id);
    g_array_free(mm->genre_array, TRUE);
    free(mm);
}

Genre get_genre_by_index(Music_Manager mm,int index)
{
    Genre gen = g_array_index(mm->genre_array, Genre, index);
    return gen;
}

// int cmp_like_gen(gconstpointer g1, gconstpointer g2)
// {
//     Genre *ga = (Genre *)g1;
//     Genre *gb = (Genre *)g2;
//     // if ((*aa)->disc_duration > (*bb)->disc_duration)
//     //     return -1;
//     // if ((*aa)->disc_duration < (*bb)->disc_duration)
//     //     return 1;
//     return (*gb)->likes_acum - (*ga)->likes_acum;
// }

// void add_gen_arr(Music m, Music_Manager mus_m)
// {
//     Genre gen = calloc(1, sizeof(struct genre));
//     gen->name = get_genre(m);
//     g_array_append_val(mus_m->genre_array,gen);
//     copy_likes_in_gen(m, mus_m, mus_m->genre_array->len);
// }

// void fill_gen_arr(Music m, Music_Manager mus_m)
// {
//     int r = 0;

//     for (int i = 0; i < mus_m->genre_array->len && !r; i++)
//         if (strcmp(get_music_genre(m), mus_m->g_array_index(mus_m->genre_array, struct genre, i))->name == 0)
//         {
//             copy_likes_in_gen(m,mus_m,i);
//             r++;
//         }

//     if (!r)
//         add_gen_arr(m,mus_m);
// }

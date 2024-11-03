#include <glib.h>
#include <stdio.h>
#include "parser.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "utils.h"
#include "logica.h"

typedef struct genre
{
    char *name;
    int likes[120];
    int likes_acum;
} *Genre;

typedef struct music_manager
{
    GHashTable *musics_by_id;
    GArray *genre_array;
} *Music_Manager;

Music_Manager create_music_manager()
{
    Music_Manager mm = malloc(sizeof(struct music_manager));
    mm->musics_by_id = g_hash_table_new_full(g_int_hash, g_int_equal, free, (void *)free_music);
    mm->genre_array = g_array_new(FALSE, FALSE, sizeof(struct genre));
    return mm;
}

Genre create_gen(char *gen_name)
{
    Genre gen = calloc(1, sizeof(struct genre));
    gen->name = strdup(gen_name);
    return gen;
}

void gen_freq_acum(Music_Manager mm)
{
    for (int i = 0; i < mm->genre_array->len; i++)
        for (int j = 1; j < 120; j++)
            g_array_index(mm->genre_array, Genre, i)->likes[j] += g_array_index(mm->genre_array, Genre, i)->likes[j - 1];
}

int cmp_like_gen(gconstpointer g1, gconstpointer g2)
{
    Genre *ga = (Genre *)g1;
    Genre *gb = (Genre *)g2;
    // if ((*aa)->disc_duration > (*bb)->disc_duration)
    //     return -1;
    // if ((*aa)->disc_duration < (*bb)->disc_duration)
    //     return 1;
    return (*gb)->likes_acum - (*ga)->likes_acum;
}

void sort_gen(Music_Manager mm)
{
    gen_freq_acum(mm);
    g_array_sort(mm->genre_array, cmp_like_gen);
}

void insert_gen(Music m, Music_Manager mus_m, int i)
{
    int r = 0;
    char *gen = get_genre(m);
    for (int i = 0; i < mus_m->genre_array->len && !r; i++)
        if (strcmp(gen, g_array_index(mus_m->genre_array, Genre, i)->name) == 0)
            r++;

    if (r == 0)
    {
        Genre gen_real = create_gen(gen);
        g_array_insert_val(mus_m->genre_array, i, gen_real);
    }
}

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

void insert_music_by_id(Music m, Music_Manager music_manager)
{
    int *id = get_music_id_pointer(m);
    g_hash_table_insert(music_manager->musics_by_id, id, m);
}

void add_like_genre(Music_Manager mm, Genre gen, short age)
{
    
}

void store_Musics(FILE *fp_musics, Music_Manager mm, Art_Manager am)
{
    ssize_t nRead = 0;
    char **line = calloc(1, sizeof(char *));
    FILE *music_errors = fopen("resultados/musics_errors.csv", "w");
    Music music = NULL;
    int i = 0;
    while (nRead != -1)
    {
        music = parse_line(fp_musics, (void *)create_music_from_tokens, &nRead);
        if (music != NULL)
        {
            if (valid_artists(get_music_artists(music), get_music_duration(music), am))
            {
                insert_music_by_id(music, mm);
                insert_gen(music, mm, i++);
            }
            else
            {
                free_music(music);
                music = NULL;
                error_output(fp_musics, music_errors, line, nRead);
            }
        }
        else
        {
            if (nRead != -1)
                error_output(fp_musics, music_errors, line, nRead);
        }
    }
    fclose(music_errors);
    free(line);
}

Music search_music_by_id(int id, Music_Manager music_manager)
{
    Music m = g_hash_table_lookup(music_manager->musics_by_id, &id);
    return m;
}

void free_music_manager(Music_Manager mm)
{
    g_hash_table_destroy(mm->musics_by_id);
    g_array_free(mm->genres, TRUE);
    free(mm);
}
#include <glib.h>
#include <stdio.h>

#include "artist_manager.h"

typedef struct art_manager
{
    GHashTable *art_by_id;
} *Art_Manager;
/*
typedef struct array_art
{
    GArray *artists;
    int tamanho;
    char *country;
} *Arr_art;

Art_Manager create_art_manager()
{
    Art_Manager am = malloc(sizeof(struct art_manager));
    am->art_by_id = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, (void *)free_art);
    return am;
}

void insert_art_by_id(Artist a, Art_Manager art_manager)
{
    int id = get_art_id(a);
    g_hash_table_insert(art_manager->art_by_id, &id, a);
}

User search_art_by_id(int id, Art_Manager art_manager)
{
    Artist a = g_hash_table_lookup(art_manager->art_by_id, &id);
    return a;
}

void free_art_manager(Art_Manager am)
{
    g_hash_table_destroy(am->art_by_id);
    free(am);
}

Arr_art create_arr_art(Art_Manager art_manager, char *country)
{
    Arr_art arr_art = malloc(sizeof(struct array_art));

    arr_art->tamanho = 0;
    arr_art->country = country;
    arr_art->artists = g_array_new(FALSE, FALSE, sizeof(struct artist));
}

void add_arr_art(gpointer *key, gpointer *value, gpointer *art_data)
{
    Arr_art *arr_art = (arr_art *)art_data;
    Artist *artist = (Artist *)value;

    if (strcmp(artist->country, arr_art->country) == 0 || strcmp(NULL, arr_art->country) == 0)
    {
        g_array_insert_val(arr_art->artists, arr_art->tamanho, artist);
        arr_art->tamanho++;
    }
}

int cmp_art(gconstpointer a, gconstpointer b)
{
    Artist artist_a = (Artist *)a;
    Artist artist_b = (Artist *)b;

    return (artist_b->disc_duration - artist_a->disc_duration);
}

void fill_arr_art(Art_Manager art_manager, Arr_art arr_art)
{
    g_hash_table_foreach(art_manager, add_arr_art, &arr_art);
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
}*/
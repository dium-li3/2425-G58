#include <strings.h>
#include <glib.h>
#include "output.h"

#define GEN_LEN 150
typedef struct genre
{
    char *name;
    int likes[GEN_LEN];
    int total_likes;
} *Genre;

Genre create_gen(char *gen_name)
{
    Genre gen = calloc(1, sizeof(struct genre));
    gen->name = strdup(gen_name);
    return gen;
}

void clear_genre(Genre *gen){
    free ((*gen)->name);
    free (*gen);
}

int compare_genre_names(Genre gen, char *genre){
    return strcmp(gen->name, genre);
}

int get_genre_total_likes (Genre gen){
    return gen->total_likes;
}

void increment_like(Genre gen, short age){
    gen->likes[age]++;
}

void gen_freq_acum (Genre gen){
    for (int j = 1; j < GEN_LEN; j++)
            gen->likes[j] += gen->likes[j - 1];
}

void update_gen_total_likes (Genre gen, int min_age, int max_age){
    int tot_likes = 0;
    if (min_age > 0)
        tot_likes = gen->likes[max_age] - gen->likes[min_age-1];
    else
        tot_likes = gen->likes[max_age];
    gen->total_likes = tot_likes;
}

int cmp_like_gen(gconstpointer g1, gconstpointer g2)
{
    Genre *ga = (Genre *)g1;
    Genre *gb = (Genre *)g2;
    int r = (*gb)->total_likes - (*ga)->total_likes;
    if (r == 0)
        r = strcmp ((*ga)->name, (*gb)->name);
    return r;
}

int print_genre_info(Genre gen, Output out)
{
    if (gen->total_likes > 0){
        char *name = strdup (gen->name);
        output_genre (name, gen->total_likes, out);
        free (name);
    }
    return gen->total_likes;
}
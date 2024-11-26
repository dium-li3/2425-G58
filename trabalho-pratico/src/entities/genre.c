#include <strings.h>
#include <glib.h>
#include "output.h"
#include "utils.h"

#define GEN_LEN 150
typedef struct genre
{
    char *name;
    int likes[GEN_LEN];
    int total_likes;
} *Genre;

Genre create_gen(const char *gen_name)
{
    Genre gen = calloc(1, sizeof(struct genre));
    gen->name = strdup(gen_name);
    return gen;
}

void free_genre(Genre *gen){
    free ((*gen)->name);
    free (*gen);
}

gboolean compare_genre_names(Genre gen, const char *genre){
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

int print_genre_info(Genre gen, char separador, Output out)
{
    if (gen->total_likes > 0){
        char **infos = calloc (2, sizeof (char *));
        infos [0] = strdup (gen->name);
        infos [1] = calloc (11, sizeof (char));
        sprintf (infos [1], "%d", gen->total_likes);
        output_geral (infos, 2, separador, out);
        free_tokens (infos, 2);
    }
    return gen->total_likes;
}
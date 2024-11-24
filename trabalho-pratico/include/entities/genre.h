#ifndef GENRE_H
#define GENRE_H

#include <glib.h>
#include "output.h"

typedef struct genre *Genre;

Genre create_gen(const char *gen_name);

void clear_genre(Genre *gen);

int compare_genre_names(Genre gen, const char *genre);

int get_genre_total_likes (Genre gen);

void increment_like(Genre gen, short age);

void gen_freq_acum (Genre gen);

void update_gen_total_likes (Genre gen, int min_age, int max_age);

int cmp_like_gen(gconstpointer g1, gconstpointer g2);

int print_genre_info(Genre gen, Output out);

#endif
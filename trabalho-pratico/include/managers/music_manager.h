#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <stdio.h>
#include "musics.h"
#include "artist_manager.h"
#include "album_manager.h"
#include "output.h"

typedef struct genre *Genre;

typedef struct music_manager *Music_Manager;

int get_gen_arr_len(Music_Manager mm);

Music_Manager create_music_manager();

void add_like_genres (const GArray *musics, Music_Manager mm, short age);

void gen_arr_freq_acum(Music_Manager mm);

int cmp_like_gen (gconstpointer g1, gconstpointer g2);

void sort_gen(Music_Manager mm,int min_age, int max_age);

void insert_music_by_id(Music m, Music_Manager music_manager);

gboolean all_musics_exist (const GArray *musics, Music_Manager mm);

const GArray *get_music_artists_from_id (int id, Music_Manager mm);

GArray *get_music_artists_copy_from_id (int id, Music_Manager mm);

void store_Musics(char *music_path, Music_Manager mm, Art_Manager am, Album_Manager alm);

void free_music_manager(Music_Manager mm);

void update_arr_total_likes(Music_Manager mm,int min_age, int max_age);

void print_all_genres_info(Music_Manager mm, Output out);

#endif

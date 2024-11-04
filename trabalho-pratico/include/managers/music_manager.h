#ifndef _MUSIC_MANAGER_H
#define _MUSIC_MANAGER_H

#include <stdio.h>
#include "musics.h"
#include "artist_manager.h"

typedef struct genre *Genre;

typedef struct music_manager *Music_Manager;

int get_gen_arr_len(Music_Manager mm);

Music_Manager create_music_manager();

Genre create_gen(char* gen_name);

void add_like_genre(Music_Manager mm, char *genre, short age);

void gen_freq_acum(Music_Manager mm);

int cmp_like_gen (gconstpointer g1, gconstpointer g2);

void sort_gen(Music_Manager mm,int min_age, int max_age);

int insert_gen(Music m, Music_Manager mus_m, int i);

void insert_music_by_id(Music m, Music_Manager music_manager);

void store_Musics(char *music_path, Music_Manager mm, Art_Manager am);

Music search_music_by_id(int id, Music_Manager music_manager);

void free_music_manager(Music_Manager mm);

void get_total_likes(Music_Manager mm,int min_age, int max_age);

Genre get_genre_by_index(Music_Manager mm,int index);

int print_genre_info(Genre gen, Output out);

#endif

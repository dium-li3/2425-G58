#ifndef _MUSIC_MANAGER_H
#define _MUSIC_MANAGER_H

#include <stdio.h>
#include "musics.h"
#include "artist_manager.h"

typedef struct genre *Genre;

typedef struct music_manager *Music_Manager;

Music_Manager create_music_manager();

Genre create_gen(char* gen_name);

void gen_freq_acum(Music_Manager mm);

int cmp_like_gen (gconstpointer g1, gconstpointer g2);

void sort_gen(Music_Manager mm);

void insert_gen(Music m, Music_Manager mus_m, int i);

void insert_music_by_id(Music m, Music_Manager music_manager);

void store_Musics(FILE *fp_musics, Music_Manager mm, Art_Manager am);

Music search_music_by_id(int id, Music_Manager music_manager);

void free_music_manager(Music_Manager mm);

#endif
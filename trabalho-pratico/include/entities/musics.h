#ifndef MUSICS_H
#define MUSICS_H

#include <glib.h>

typedef struct music *Music;

Music create_music_from_tokens (char **tokens);

int get_music_id(Music m);

int *get_music_id_pointer(Music m);

char* get_genre(Music m);

int get_music_duration(Music m);

GSList *get_music_artists (Music m);

short get_likes(Music m, int min, int max);

void add_like(Music m, int age);

void likes_added_frequency(Music m);

void free_music(Music m);

#endif
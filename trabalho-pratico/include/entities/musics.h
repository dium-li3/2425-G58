#ifndef MUSICS_H
#define MUSICS_H

#include <glib.h>

typedef struct music *Music;

Music create_music_from_tokens (char **tokens);

int get_music_id(Music m);

int *get_music_id_pointer(Music m);

char* get_genre(Music m);

int get_music_duration(Music m);

GArray *get_music_artists (Music m);

void free_music(Music m);

int valid_duration (char *duration);

#endif
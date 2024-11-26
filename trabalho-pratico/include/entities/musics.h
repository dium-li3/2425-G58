#ifndef MUSICS_H
#define MUSICS_H

#include <glib.h>

typedef struct music *Music;

Music create_music_from_tokens (char **tokens);

int get_music_id(Music m);

int *get_music_id_pointer(Music m);

const char* get_genre(Music m);

int get_music_duration(Music m);

const GArray *get_music_artists (Music m);

GArray *get_music_artists_copy(Music m);

int get_music_album (Music m);

void free_music(Music m);

#endif
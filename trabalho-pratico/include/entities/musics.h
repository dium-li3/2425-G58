#ifndef MUSICS_H
#define MUSICS_H

#include <glib.h>

typedef struct music *Music;

Music create_music(int id, char *t, GSList arts_ids, char *d, int d_s, char g, short year, char *l);

int get_music_id(Music m);

char get_genre(Music m);

int get_duration(Music m);

void free_music(Music m);

#endif
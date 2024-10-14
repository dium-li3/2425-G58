#include <stdlib.h>
#include "musics.h"

typedef struct music {
    int id;
    char *title;
    GSList artists_ids;
    char *duration;
    int duration_s;
    char genre;
    short year;
    char *lyrics;
} *Music;



Music create_music(int id, char *t, GSList arts_ids, char *d, int d_s, char g, short y, char *l) {
    Music m = malloc(sizeof(struct music));
    m->id = id;
    m->title = strdup(t);
    m->artists_ids = arts_ids;
    m->duration = strdup(d);
    m->duration_s = d_s;
    m->genre = g;
    m->year = y;
    m->lyrics = strdup(l);

    return m;
}



int get_music_id(Music m) {
    return (m->id);
}



char get_genre(Music m) {
    return (m->genre);
}



int get_duration(Music m) {
    return (m->duration);
}



void free_music(Music m) {
    free(m->title);
    free(m->duration);
    free(m->lyrics);
    g_slist_free_full(m->artists_ids, free);

    free(m);
}
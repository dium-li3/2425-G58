#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "musics.h"
#include "parser.h"
#include "utils.h"

typedef struct music {
    int id;
    GArray *artists_ids;
    int duration_s;
    char *genre;
    short year;
    int album_id;
} *Music;

Music create_music(int id, GArray *arts_ids, int d_s, int album_id, char *g, short y) {
    Music m = calloc(1, sizeof(struct music));
    m->id = id;
    m->artists_ids = arts_ids;
    m->duration_s = d_s;
    m->genre = strdup(g);
    m->year = y;
    m->album_id = album_id;
    return m;
}

Music create_music_from_tokens (char **tokens) {
    Music m = NULL;

    if (valid_duration(tokens[4]) && valid_list(tokens[2])) {
        int id = atoi(tokens[0]+1);
        GArray *artists_ids = store_list(tokens[2]);
        int duration_s = calc_duration_s(tokens[4]);
        short year = (short)atoi(tokens[6]);
        int album_id = atoi (tokens[3]+2);

        m = create_music(id, artists_ids, duration_s, album_id, tokens[5], year);
    }

    return m;
}

int get_music_id(Music m) {
    return (m->id);
}

const char* get_genre(Music m) {
    return m->genre;
}

int get_music_duration(Music m){
    return (m->duration_s);
}

const GArray *get_music_artists(Music m){
    return m->artists_ids;
}

int get_music_album (Music m){
    return m->album_id;
}

void free_music(Music m) {
    free(m->genre);
    if (m->artists_ids != NULL)
        g_array_free(m->artists_ids, TRUE);

    free(m);
}
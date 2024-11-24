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
} *Music;


Music create_music(int id, GArray *arts_ids, int d_s, char *g, short y) {
    Music m = calloc(1, sizeof(struct music));
    m->id = id;
    m->artists_ids = arts_ids;
    m->duration_s = d_s;
    m->genre = strdup(g);
    m->year = y;
    //m->likes começa toda a 0 por causa do calloc.
    return m;
}


/*
    Calcula a duração em segundos. Só pode ser usada depois de se validar a string.
*/
int calc_duration_s(char *st) {
    int h = 0, m = 0, s = 0;
    sscanf(st, "%d:%d:%d", &h, &m, &s);

    return (h*3600 + m*60 + s);
}


Music create_music_from_tokens (char **tokens) {
    Music m = NULL;

    if (valid_duration(tokens[3]) && valid_list(tokens[2])) {
        int id = atoi(tokens[0]+1);
        GArray *artists_ids = store_list(tokens[2]);
        int duration_s = calc_duration_s(tokens[3]);
        short year = (short)atoi(tokens[5]);

        m = create_music(id, artists_ids, duration_s, tokens[4], year);
    }

    return m;
}


int get_music_id(Music m) {
    return (m->id);
}


int *get_music_id_pointer (Music m){
    int *copy = malloc(sizeof(int));
    *copy = m->id;

    return copy;
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


void free_music(Music m) {
    free(m->genre);
    if (m->artists_ids != NULL)
        g_array_free(m->artists_ids, TRUE);

    free(m);
}


int valid_duration (char *duration){
    int r = 1;
    if (strlen (duration) != 8) r = 0;

    for (int i = 0; r && duration[i] != '\0'; i++){
        if (i == 2 || i == 5) r = duration[i] == ':';
        else if (i == 3 || i == 6) r = isdigit (duration[i]) && duration[i] < '6';
        else r = isdigit (duration[i]);
    }

    return r;
}
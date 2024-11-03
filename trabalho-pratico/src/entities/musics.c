#include <stdlib.h>
#include <stdio.h>
#include "musics.h"
#include "sintatica.h"
#include "parser.h"

typedef struct music {
    int id;
    char *title;
    GSList *artists_ids;
    char *duration;
    int duration_s;
    char *genre;
    short year;
    char *lyrics;
} *Music;


Music create_music(int id, char *t, GSList *arts_ids, char *d, int d_s, char *g, short y, char *l) {
    Music m = calloc(1, sizeof(struct music));
    m->id = id;
    m->title = strdup(t);
    m->artists_ids = arts_ids;
    m->duration = strdup(d);
    m->duration_s = d_s;
    m->genre = strdup(g);
    m->year = y;
    m->lyrics = strdup(l);
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

    if (valid_duration(tokens[3])) {
        int id = atoi(tokens[0]+1);
        GSList *artists_ids = store_list(tokens[2]);
        int duration_s = calc_duration_s(tokens[3]);
        short year = (short)atoi(tokens[5]);

        m = create_music(id, tokens[1], artists_ids, tokens[3], duration_s, tokens[4], year, tokens[6]);
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


char* get_genre(Music m) {
    return (strdup(m->genre));
}


int get_music_duration(Music m){
    return (m->duration_s);
}


GSList *get_music_artists(Music m){
    return m->artists_ids;
}


// short get_likes(Music m, int min, int max){
//     return (m->likes[max] - m->likes[min]);
// }


// void add_like(Music m, int age){
//     m->likes[age]++;
// }


// void likes_added_frequency(Music m){
//     for (int i = 1; i < 120; i++){
//         m->likes[i] += m->likes[i-1];
//     }
// }


void free_music(Music m) {
    free(m->title);
    free(m->duration);
    free(m->genre);
    free(m->lyrics);
    g_slist_free_full(m->artists_ids, free);

    free(m);
}
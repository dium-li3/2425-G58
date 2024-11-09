#include <stdlib.h>
#include <glib.h>
#include <stdio.h>

#include "artists.h"
#include "parser.h"
#include "output.h"

// #include "sintatica.h"
// #include "parser.h"

typedef struct artist{
    int id;
    char *name;
    char *description;
    float recipe_per_stream;
    GArray *id_constituent;
    char *country;
    char type; // individual = I e grupo = G
    int disc_duration;
} *Artist;

Artist create_art(int id, char *name, char *desc, float rps, GArray *idc, char *coun, char type){
    Artist a = malloc(sizeof(struct artist));
    a->id = id;
    a->name = strdup(name);
    a->description = strdup(desc);
    a->recipe_per_stream = rps;
    a->id_constituent = idc;
    a->country = strdup(coun);
    a->type = type;
    a->disc_duration = 0;
    return a;
}

int get_art_id(Artist a){
    return a->id;
}

int *get_art_id_pointer(Artist a){
    int *copy = malloc(sizeof(int));
    *copy = a->id;

    return copy;
}

char *get_art_country(Artist a){
    return strdup(a->country);
}

int get_disc_duration(Artist a){
    return a->disc_duration;
}

char *get_art_name(Artist a){
    return strdup(a->name);
}

char get_art_type_from_art(Artist a){
    return a->type;
}

char get_art_type(char *art_type){
    char c = 'E';
    if (strcmp(art_type, "individual") == 0)
        c = 'I';
    if (strcmp(art_type, "group") == 0)
        c = 'G';
    return c;
}

int compare_dur (gconstpointer a, gconstpointer b){
    Artist *aa = (Artist *) a;
    Artist *bb = (Artist *) b;
    if ((*aa)->disc_duration > (*bb)->disc_duration)
        return -1;
    if ((*aa)->disc_duration < (*bb)->disc_duration)
        return 1;
    return strcmp ((*aa)->name, (*bb)->name);
}

Artist create_artist_from_tokens(char **tokens){
    char art_type = get_art_type(tokens[6]);
    GArray *id_constituent = NULL;
    id_constituent = store_list(tokens[4]);
    Artist a = NULL;
    if (!(art_type == 'I' && id_constituent != NULL) && art_type != 'E')
    { // negação do caso inválido (individual com id e constituent ñ nulo)
        int id = atoi(tokens[0] + 1);
        float recipe_per_stream = strtof(tokens[3], NULL);
        a = create_art(id, tokens[1], tokens[2], recipe_per_stream, id_constituent, tokens[5], art_type);
    }
    else if (id_constituent != NULL)
        g_array_free(id_constituent, TRUE);
    return a;
}

char *get_art_type_str (Artist a){
    char *res = NULL;
    if (a->type == 'I')
        res = strdup ("individual");
    else
        res = strdup ("group");
    return res;
}

char *calc_duration_hms(int segs){
    int h = segs / 3600;
    int t = segs % 3600;
    int m = t / 60;
    int s = t % 60;
    char *hms =calloc (9, sizeof(char));
    sprintf(hms, "%02d:%02d:%02d", h, m, s);
    return hms;
}

void print_art_info(Artist a, Output out){
    char *type = get_art_type_str (a);
    char *disc_duration = calc_duration_hms(a->disc_duration);
    char *name = strdup (a->name);
    char *country = strdup (a->country);
    output_artist (name, type, disc_duration, country, out);
    free (name);
    free (disc_duration);
    free (type);
    free (country);
}

//só serve para test
// void print_dur (Artist a){
//     FILE *fp = fopen ("help.txt", "a+");
//     fprintf (fp, "%d\n", a->disc_duration);
//     fclose (fp);
// }

void add_disc_duration(Artist a, int duration){
    a->disc_duration += duration;
}

void free_art(Artist a){
    free(a->name);
    free(a->description);
    if (a->id_constituent != NULL)
        g_array_free(a->id_constituent, TRUE);
    free(a->country);
    free(a);
}

#include <stdlib.h>
#include <stdio.h>
#include "artists.h"
// #include "sintatica.h"
// #include "parser.h"

typedef struct artist{
    int id;
    char *name;
    char *description;
    float recipe_per_stream;
    int *id_constituent;
    char *country;
    char type; //individual = I e grupo = G
    int disc_duration;
} *Artist;

Artist create_art (int id, char *name, char *desc, float rps, int *idc, char *coun, char type){
    Artist a = malloc(sizeof (struct artist));
    a->id = id;
    a->name = strdup (name);
    a->description =strdup (desc);
    a->recipe_per_stream = rps;
    a->id_constituent = (int *)malloc(10 * sizeof(int)); //é possível uma banda com mais de 10 artistas?
    a->country = strdup (coun);
    a->type = type;
    a->disc_duration = 0;
    return a;
}

int get_art_id(Artist a){
    return a->id;
}

char get_art_type (char *art_type){
    char c = 'E';
    if  (strcmp (art_type, "individual") == 0)
        c = 'I';
    if (strcmp(sub_type, "group") == 0)
        c = 'P';
    return c;
}

void print_art_info (Artist a){
    fprintf(stdout, "%s;%s;%s;%d;%s\n",a->name, a->type, u->disc_duration, a->country);
}

void free_art(Artist a)
{
    free(a->name);
    free(a->description);
    free(a->idc);
    free(a->country);
    free(a);
}

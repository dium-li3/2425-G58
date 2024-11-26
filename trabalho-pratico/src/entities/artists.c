#include <stdlib.h>
#include <glib.h>
#include <stdio.h>

#include "artists.h"
#include "parser.h"
#include "output.h"
#include "utils.h"

// #include "sintatica.h"
// #include "parser.h"

typedef struct artist{
    int id;
    char *name;
    double recipe_per_stream;
    GArray *id_constituent;
    char *country;
    char type; // individual = I e grupo = G
    int disc_duration;
    int n_albums;
    double total_recipe;
} *Artist;

Artist create_art(int id, char *name, double rps, GArray *idc, char *coun, char type){
    Artist a = malloc(sizeof(struct artist));
    a->id = id;
    a->name = strdup(name);
    a->recipe_per_stream = rps;
    a->id_constituent = idc;
    a->country = strdup(coun);
    a->type = type;
    a->disc_duration = 0;
    a->n_albums = 0;
    a->total_recipe = 0;
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

const char *get_art_country(Artist a){
    return (a->country);
}

int get_disc_duration(Artist a){
    return a->disc_duration;
}

const char *get_art_name(Artist a){
    return a->name;
}

double get_art_recipe_stream (Artist a){
    return a->recipe_per_stream;
}

const GArray *get_group_id_constituent (Artist a){
    const GArray *id_constituent = NULL;
    if (a->type == 'G')
        id_constituent = a->id_constituent;
    return id_constituent;
}

char get_art_type_from_art(Artist a){
    return a->type;
}

char get_art_type(char *art_type){
    char c;
    string_to_lower (art_type);
    switch (same_string (art_type, "individual", "group")){
        case (1):
            c = 'I';
            break;
        case (2):
            c = 'G';
            break;
        default:
            c = 'E';
    }
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
    int valid_l = valid_list(tokens[4]);
    if(valid_l) id_constituent = store_list(tokens[4]);
    Artist a = NULL;
    int valid = !(art_type == 'I' && id_constituent != NULL) && art_type != 'E' && valid_l;

    if (valid)
    { // negação do caso inválido (individual com id e constituent ñ nulo)
        int id = atoi(tokens[0] + 1);
        float recipe_per_stream = strtod(tokens[3], NULL);
        a = create_art(id, tokens[1], recipe_per_stream, id_constituent, tokens[5], art_type);
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

/*
    Escreve num ficheiro de output um resumo
    do artista, útil para a query 1.
*/
void print_art_res(Artist a, Output out){
    char **infos = calloc (5, sizeof(char *)); 
    infos[0] = strdup (a->name);
    infos[1] = get_art_type_str (a);
    infos[2] = strdup (a->country);
    infos[3] = malloc (sizeof(char) * 11);
    snprintf(infos[3], 11, "%d", a->n_albums);
    infos[4] = malloc (sizeof(char) * 20);
    snprintf(infos[4], 20, "%.2f", a->total_recipe);
    output_geral (infos, 5, out);
    free_tokens (infos, 5);
}

void print_art_info(Artist a,Output out){
    char **infos = calloc (4, sizeof(char *)); 
    infos[0] = strdup (a->name);
    infos[1] = get_art_type_str (a);
    infos[2] = calc_duration_hms(a->disc_duration);
    infos[3] = strdup (a->country);
    output_geral (infos, 4, out);
    free_tokens (infos, 4);
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

//Incrementa o número de albuns de um dado artista.
void add_1_album (Artist a){
//    if (a->type == 'I')
    a->n_albums++;
}

/*
    Adiciona a receita inteira que um artista recebe
    por música ouvida ao seu total ganho.
*/
void add_recipe (Artist a){
    a->total_recipe += a->recipe_per_stream;
}

/*
    Adiciona a percentagem da receita de um grupo
    a um dado Artista.
*/
void add_percentage_recipe (Artist a, double recipe){
    a->total_recipe += recipe;
}

void free_art(Artist a){
    free(a->name);
    if (a->id_constituent != NULL)
        g_array_free(a->id_constituent, TRUE);
    free(a->country);
    free(a);
}

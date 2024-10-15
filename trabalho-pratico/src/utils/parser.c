#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "parser.h"

/*
    Dados n tokens, liberta a memoria usada por eles
    e pelo pointer que os guarda.
*/
void free_tokens(char **tokens, int n){
    for (int i = 0; i < 8; i++){
        free (tokens[i]);
    }
    free (tokens);
}

/*
    Guarda os ids de uma lista guardados 
    numa linha para uma lista de verdade.
*/
GSList *store_list (char *line){
    char *token = NULL;
    char *svptr = NULL;

    GSList *list = NULL;
    token = strtok_r (line, "\' ,[]", &svptr);

    int *id1 = malloc(sizeof(int));
    *id1 = atoi (token + 1);
    list = g_slist_prepend (list, id1);
    for (;(token = strtok_r (NULL, "\' ,]", &svptr)) != NULL;){
        int *id = malloc(sizeof(int));
        *id = atoi (token + 1);
        list = g_slist_prepend (list, id);
    }
    return list;
}

//short *parse_time (char *line, short *time){}

/*
    Separa uma linha nos seus tokens.
*/
void *parse_line (FILE * fp, void *(*Func)(char **), ssize_t *nRead){
    void *entity = NULL;
    size_t n;
    char *line = NULL;
    char *token = NULL;
    char *svptr = NULL;
    *nRead = getline (&line, &n, fp);
    if (*nRead != -1){
        token = strtok_r (line, ";\"\n", &svptr);
        char **info = calloc (8, sizeof (char *));
        info [0] = strdup (token);
        for (int j = 1; (token = strtok_r (NULL, ";\"\n", &svptr)) != NULL && j < 8; j++){
            info [j] = strdup(token);
        }
        entity = Func (info);
        free_tokens(info, 8);
    }
    free (line);
    return entity;
}

int parse_line_spaces (char *line, char **info){
    int n_token = 0;
    char *token = NULL;
    char *svptr = NULL;
    char *ajudante = strdup (line);
    token = strtok_r (ajudante, " ", &svptr);
    info [n_token++] = strdup (token);
    while ((token = strtok_r (NULL, " \n", &svptr)) != NULL && n_token < 3){
        info [n_token++] = strdup(token);
    }
    free (ajudante);
    return n_token;
}

int parse_3_tokens (char *line, char **info){
    int n_token = 0;
    char *token = NULL;
    char *svptr = NULL;
    char *ajudante = strdup (line);
    while ((token = strtok_r (ajudante, " \"\n", &svptr)) != NULL && n_token < 2)
        info [n_token++] = strdup (token);
    token = strtok_r (NULL, " ", &svptr);
    if (token != NULL)//pode ser vazio
        info [n_token++] = strdup(token);
    return n_token;
}

int parse_1line_querie(FILE *fp, char **info){
    int n_token = 0;
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    if ((nRead = getline (&line, &n, fp)) != -1){
        switch (line[0]){
            case (2):
                n_token = parse_3_tokens (line, info);
                break;
            case (1):
            case (3):
            default:
                n_token = parse_line_spaces (line, info);
        }
    }
    else info = NULL;
    free(line);
    return n_token;
}

/*
    Guarda uma linha do file que lhe dão.
*/
GSList *parse_file (FILE *fp){
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    GSList *lista = NULL;
    //nRead = getline (&line, &n, fp);//ignorar a 1º linha
    while ((nRead = getline (&line, &n, fp)) != -1){
        lista = g_slist_prepend (lista, strdup (line));
    }
    free (line);
    return lista;
}

/*
    Guarda a linha lida na string dada.
    Devolve -1 caso não tenha lido nada.
*/
ssize_t parse_1line (FILE *fp, char **line){
    ssize_t nRead;
    size_t n;
    nRead = getline (line, &n, fp);
    return nRead;
}

//Devolva os anos passados de uma string que representa uma data.
short read_date_to_age (char *bd){
    short age = 0;
    short year_month_day[3];
    char *y_m_d = NULL;
    char *strpt = NULL;

    y_m_d = strtok_r (bd, "/", &strpt);
    year_month_day[0] = (short)atoi (y_m_d);
    for (int i = 1;(y_m_d = strtok_r (NULL, "/:\n", &strpt)) != NULL && i < 3; i++)
        year_month_day[i] = (short)atoi (y_m_d);

    age = 2023 - year_month_day[0];
    if (year_month_day[1] < 9) age++;
    if (year_month_day[1] == 9){
        if (year_month_day[2] <= 9) age++;
    }
    return age;
}

/*
    Dado o path onde se encontram os 3 ficheiros .csv,
    devolve um pointer para os 3 file pointers dos ficheiros.
*/
FILE **fopen3Entities (char *path){
    size_t length_path = strlen(path);
    char *help = malloc (length_path+12);
    FILE **fp_entities = malloc (sizeof (FILE*) * 3);

    if (fp_entities == NULL || help == NULL){
        perror ("no space to save entities pointer :c :");
        return NULL;
    }

    strcpy (help, path);
    strcpy (help + length_path, "users.csv");
    fp_entities[0] = fopen (help, "r");
    strcpy (help + length_path, "musics.csv");
    fp_entities[1] = fopen (help, "r");
    strcpy (help + length_path, "artists.csv");
    fp_entities[2] = fopen (help, "r");
    free (help);

    if (!fp_entities[0] || !fp_entities[1] || !fp_entities[2]){
        perror("ERROR: "); //os dados ñ estão onde deviam!! ou n déste o path certo
        return NULL;
    }

    return fp_entities;
}

void fclose3Entities (FILE **fp_entities){
    fclose (fp_entities[0]);
    fclose (fp_entities[1]);
    fclose (fp_entities[2]);
    free (fp_entities);
}

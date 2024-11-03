#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "parser.h"

typedef struct parser{
    FILE *fp;
    ssize_t nRead;
} *Parser;

Parser open_parser(char *path){
    Parser p = NULL;
    FILE *temp = NULL;

    if ((temp = fopen(path, "r")) != NULL) {
        p = calloc (1, sizeof (struct parser));
        p->fp = temp;
        p->nRead = 0;
    }

    return p;
}

void close_parser(Parser p){
    fclose (p->fp);
    free (p);
}

ssize_t get_nRead (Parser p){
    return p->nRead;
}

void go_back_1line (Parser p){
    fseek (p->fp, -(p->nRead), SEEK_CUR);
}

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

    if (token != NULL){
        int *id1 = malloc(sizeof(int));
        *id1 = atoi (token + 1);
        list = g_slist_prepend (list, id1);
        for (;(token = strtok_r (NULL, "\' ,]", &svptr)) != NULL;){
            int *id = malloc(sizeof(int));
            *id = atoi (token + 1);
            list = g_slist_prepend (list, id);
        }
    }
    return list;
}

//short *parse_time (char *line, short *time){}

/*
    Separa uma linha nos seus tokens.
*/
void *parse_line (Parser p, void *(*Func)(char **)){
    void *entity = NULL;
    size_t n;
    char *line = NULL;
    char *token = NULL;
    char *svptr = NULL;
    p->nRead = getline (&line, &n, p->fp);
    if (p->nRead != -1){
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

//Parse queries vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 
int parse_line_spaces (char *line, char **info){
    int n_token = 0;
    char *token = NULL;
    char *svptr = NULL;
    char *ajudante = strdup (line);
    token = strtok_r (ajudante, " ", &svptr);
    info [n_token++] = strdup (token);
    while ((token = strtok_r (NULL, " \"\n", &svptr)) != NULL && n_token < 3){
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
    token = strtok_r (ajudante, " ", &svptr);
    info [n_token++] = strdup (token);
    token = strtok_r (NULL, " \"", &svptr);
    info [n_token++] = strdup (token);
    token = strtok_r (NULL, "\"", &svptr);
    if (token != NULL)//pode ser vazio
        info [n_token++] = strdup(token);
    free (ajudante);
    return n_token;
}

int parse_1line_querie(Parser p, char **info){
    int n_token = 0;
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    if ((nRead = getline (&line, &n, p->fp)) != -1){
        switch (line[0]){
            case ('2'):
                n_token = parse_3_tokens (line, info);
                break;
            case ('1'):
            case ('3'):
            default:
                n_token = parse_line_spaces (line, info);
        }
    }
    else info = NULL;
    free(line);
    return n_token;
}
//Parse queries ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
    Guarda uma linha do file que lhe dão.
*/
GSList *parse_file (Parser p){
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    GSList *lista = NULL;
    //nRead = getline (&line, &n, fp);//ignorar a 1º linha
    while ((nRead = getline (&line, &n, p->fp)) != -1){
        lista = g_slist_prepend (lista, strdup (line));
    }
    free (line);
    return lista;
}

/*
    Guarda a linha lida na string dada.
    Devolve -1 caso não tenha lido nada.
*/
ssize_t parse_1line (Parser p, char **line){
    ssize_t nRead;
    size_t n;
    nRead = getline (line, &n, p->fp);
    return nRead;
}

/*
    Semelhante à parse_1line.
*/
char* parse_1line_alt(Parser p){
    size_t n;
    char *line = NULL;

    p->nRead = getline (&line, &n, p->fp);

    return line;
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
char **path3Entities (char *path){
    size_t length_path = strlen(path);
    char *base_path = malloc (length_path+13);
    char **path_entities = malloc (sizeof (char*) * 3);

    strcpy (base_path, path);
    strcpy (base_path + length_path, "/users.csv");
    path_entities[0] = strdup (base_path);
    strcpy (base_path + length_path, "/musics.csv");
    path_entities[1] = strdup (base_path);
    strcpy (base_path + length_path, "/artists.csv");
    path_entities[2] = strdup (base_path);
    free (base_path);

    return path_entities;
}

void free3Entities (char **fp_entities){
    free (fp_entities[0]);
    free (fp_entities[1]);
    free (fp_entities[2]);
    free (fp_entities);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "parser.h"
#include "utils.h"

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
    Guarda os ids de uma lista guardados 
    numa linha para um array.
*/
GArray *store_list (char *line){
    char *token = NULL;
    char *svptr = NULL;

    GArray *list = NULL;
    token = strtok_r (line, "\' ,[]", &svptr);

    if (token != NULL){
        int id1 = atoi (token + 1);
        list = g_array_new(FALSE, TRUE, sizeof(int));
        g_array_insert_val (list, 0,id1);
        for (int i = 1;(token = strtok_r (NULL, "\' ,]", &svptr)) != NULL;){
            int id = atoi (token + 1);
            g_array_insert_val (list, i, id);
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

int parse_1line_query(Parser p, char **info){
    int n_token = 0;
    size_t n;
    char *line = NULL;
    if ((p->nRead = getline (&line, &n, p->fp)) != -1){
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
    Guarda a linha lida na string dada (aqui, ** é endereço de string e não array 2d)
    p->nRead indica se a leitura correu bem ou não.
*/
void parse_1line (Parser p, char **line){
    size_t n;
    *line = NULL;
    p->nRead = getline (line, &n, p->fp);
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
    char **path_entities = malloc (sizeof (char*) * 5);

    strcpy (base_path, path);
    strcpy (base_path + length_path, "/users.csv");
    path_entities[0] = strdup (base_path);
    strcpy (base_path + length_path, "/musics.csv");
    path_entities[1] = strdup (base_path);
    strcpy (base_path + length_path, "/artists.csv");
    path_entities[2] = strdup (base_path);
    strcpy (base_path + length_path, "/albums.csv");
    path_entities[3] = strdup (base_path);
    strcpy (base_path + length_path, "/history.csv");
    path_entities[4] = strdup (base_path);
    free (base_path);

    return path_entities;
}

void freeEntityPaths (char **fp_entities){
    free (fp_entities[0]);
    free (fp_entities[1]);
    free (fp_entities[2]);
    free (fp_entities[3]);
    free (fp_entities[4]);
    free (fp_entities);
}

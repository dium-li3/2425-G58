#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "parser.h"
#include "user_manager.h"


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
char **parse_line (char *line, char **info){
    char *token = NULL;
    char *svptr = NULL;
    token = strtok_r (line+1, ";\"", &svptr);
    info [0] = strdup (token);
    for (int j = 1; (token = strtok_r (NULL, ";\"", &svptr)) != NULL && j < 8; j++){
        info [j] = strdup(token);
    }
    return info;
}

/*
    Guarda uma linha do file que lhe dão.
*/
GSList *parse_file (FILE *fp_Users){
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    GSList *lista = NULL;
    nRead = getline (&line, &n, fp_Users);//ignorar a 1º linha
    for (;(nRead = getline (&line, &n, fp_Users)) != -1;){
        lista = g_slist_prepend (lista, strdup (line));
    }
    free (line);
    return lista;
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

int trabalho (int argc, char **argv){
    char *path = argv[1];

    FILE *fp_queries = fopen (argv[2], "r");
    
    if (fp_queries == NULL || path[0] == '\0') {
        //perror("ERROR: "); //o ficheiro com o nome dado não existe ou a diretoria foi mal escrita
        return 2;
    }

    FILE **fp_entities = fopen3Entities (path);
    if (fp_entities == NULL)
        return -1;
    User_Manager user_manager = create_user_manager ();  
    store_Users(fp_entities[0], user_manager);
    responde_querie1 (fp_queries, user_manager);
    free_user_manager (user_manager);
    fclose3Entities (fp_entities);
    return 0;
}
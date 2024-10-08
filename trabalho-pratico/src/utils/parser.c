#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>

#include "parser.h"
#include "users.h"
#include "user_manager.h"
#include "sintatica.h"

//int ABS = 0;

/*
    Passa a informação de uma string de artistas ou liked musics,
    e passa-as para uma lista.
*/
GSList *store_list (char *line){
    char *token = NULL;
    char *svptr = NULL;

    GSList *list = NULL;
    token = strtok_r (line, "S\' ,]", &svptr);

    int music_id = atoi (token);
    list = g_slist_prepend (list, &music_id);
    //list = g_slist_prepend (list, token);
    for (;(token = strtok_r (NULL, "S\' ,]", &svptr)) != NULL;){
        music_id = atoi (token);
        list = g_slist_prepend (list, &music_id);
        //list = g_slist_prepend (list, token);
        //printf("%d\n", *(int *)list->data);
    }
    return list;
}

/*
    Guarda a informação de um user,
    retorna NULL se o user não for sintáticamente válido.
*/
User store_user_line (char *line){
    char *token = NULL;
    char *svptr = NULL;
    char *info[9];
    User u = NULL;
    info [0] = strtok_r (line, ";\"", &svptr);
    for (int j = 1; (token = strtok_r (NULL, ";\"", &svptr)) != NULL && j < 9; j++){
        info [j] = token;
    }
    char sub_type = get_sub_type (info[6]);
    int valid = valid_user_sintatic (info[1], info [4], sub_type);

    int id;
    int age;
    GSList *liked_musics = NULL;
    if (valid){ //store
        id = atoi (info[0]+1);
        age = get_age (info[4]);
        liked_musics = store_list (info[7]);
        u = create_user (id, info[1], info[2], info[3], info[4], age, info[5], sub_type, liked_musics);
    }
    // for (int i = 0; i < 9; i++)
    //     free (info[i]);
    return u;
}

/*
    Atualiza o User_Manager com a informação de todos os users válidos,
    devolve imediatamente o ficheiro de erro caso o utilizador não seja
    válido e nem o guarda.
*/
User_Manager store_Users (FILE *fp_Users, User_Manager user_manager){
    ssize_t nRead;
    size_t n;
    char *line = NULL;
    User user = NULL;
    //char type = fp_entities [6];//'a' = users; 't' = musics; 'n' = artists
    nRead = getline (&line, &n, fp_Users);
    for (int i = 0; (nRead = getline (&line, &n, fp_Users)) != -1; i++){
        user = store_user_line (line);
        if (user != NULL){//adiciona à hash
            insert_user_by_id (user, user_manager);
        }
        else {
            //mandar a linha com infos invalidas para o ficheiro de erro nº i.
        }
    }
    free(line);
    return user_manager;
}

/*
    Guarda os dados dos ficheiros nos managers de cada entidade,
    dentro do manager de managers (futuramente).

    Fáz também imediatamente a validação sintática dos dados
    e os seus ficheiros de erro caso algum dado seja inválido.
*/
void store_entities (FILE **fp_entities, User_Manager user_manager){
    user_manager = store_Users (fp_entities[0], user_manager);
    //store_Musics (fp_entities[1]);
    //store_Artists (fp_entities[2]); 
    }


void responde_querie1 (FILE *fp_queries, User_Manager um){
    ssize_t nBytes;
    size_t n;
    char *line = NULL;
    int i, id;
    
    for (i = 0; (nBytes = getline (&line, &n, fp_queries)) != -1; i++){
        if (line[0] == '1'){
            id = atoi (line + 3);
            User u = search_user_by_id (id, um);
            if (u != NULL) //aka existe nos dados guardados
                print_info (u);
            else fprintf(stdout, "\n");
        }
        else fprintf(stdout, "\n");
    }
    free (line);
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
    store_entities(fp_entities, user_manager);
    responde_querie1 (fp_queries, user_manager);
    free_user_manager (user_manager);
    fclose3Entities (fp_entities);
    return 0;
}
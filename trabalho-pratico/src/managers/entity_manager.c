#include <stdio.h>

#include "entity_manager.h"
#include "user_manager.h"
#include "queries.h"
#include "users.h"
#include "utils.h"

typedef struct entity_manager{
    User_Manager user_M;
} *Entity_Manager;

Entity_Manager create_entity_manager(){
    Entity_Manager e = calloc (1, sizeof(struct entity_manager));
    e->user_M = create_user_manager();
//    e->querie = create_querie();
    return e;
}

void store_Entities (FILE **fp, Entity_Manager entity_M){
    //store_Artists (fp[2], entity_M->artist_M);
    //store_Artists (fp[1], entity_M->artist_M);
    store_Users (fp[0], entity_M->user_M);
}

//isto ficará no IO?

/*
    Dá print do email, nomes, idade e pais do utilizador.
    No futuro isto não será no stdout e sim no ficheiro de outputs assumo eu.
*/void print_user_info (User u, int n_querie){
    char output_file[34];//dá para numeros até 9999
    snprintf (output_file, 34, "resultados/command%d_output.txt", n_querie);
    FILE *fp = fopen (output_file, "w+");
    if (u != NULL){
        char *email = get_user_email(u);
        char *first_name = get_user_first_name(u);
        char *last_name = get_user_last_name(u);
        short age = get_user_age(u);
        char *country = get_user_country(u);
        fprintf(fp, "%s;%s;%s;%d;%s\n",email , first_name, last_name, age, country);
        }
    else 
        fprintf(fp, " ");
}

void answer1 (int id, Entity_Manager em, int n_querie){
    User u = search_user_by_id (id, em->user_M);
    print_user_info (u, n_querie);
}

void answer2 (int N, char *country, Entity_Manager em){

}

void answer3 (int min, int max, Entity_Manager em){

}



void answer_querie (Querie q, Entity_Manager em, int type, int n_querie){
    switch (type){
        case (1):
            int id = get_querie1_info (q);
            answer1 (id, em, n_querie);
            break;
        case (2):
            short *N = calloc (1, sizeof(int));
            char *country = get_querie2_info(q, N);
            answer2 (*N, country, em);
            free (N);
            break;
        case (3):
            short *max = calloc (1, sizeof(int));
            short min = get_querie3_info (q, max);
            answer3 (min, *max, em);
            free (max);
            break;
    }
}

void answer_all_queries (FILE *fp_queries, Entity_Manager em){
    int i;
    Querie q = create_querie();
    for (i = 1; get_querie_type (q) != -1; i++){
        int type = read_querie_line (fp_queries, q);
        answer_querie (q, em, type, i);
    }
    free_querie (q);
}


void free_entity_manager (Entity_Manager e){
    free_user_manager (e->user_M);
    free (e);
}
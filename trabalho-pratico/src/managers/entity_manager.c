#include <stdio.h>
#include <time.h>

#include "entity_manager.h"
#include "user_manager.h"
#include "queries.h"
#include "users.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "artists.h"
#include "musics.h"
#include "output.h"

typedef struct entity_manager{
    Art_Manager artist_M;
    Music_Manager music_M;
    User_Manager user_M;
} *Entity_Manager;

Entity_Manager create_entity_manager(){
    Entity_Manager e = calloc(1, sizeof(struct entity_manager));
    e->user_M = create_user_manager();
    e->music_M = create_music_manager();
    e->artist_M = create_art_manager();
    return e;
}

void store_Entities(char **entity_paths, Entity_Manager entity_M){
    store_Artists(entity_paths[2], entity_M->artist_M);
    store_Musics(entity_paths[1], entity_M->music_M, entity_M->artist_M);
    order_duration (entity_M->artist_M);
    store_Users(entity_paths[0], entity_M->user_M, entity_M->music_M);
}



//funções normaisVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
// isto ficará no IO?

void answer1(int id, Entity_Manager em, Output out){
    User u = search_user_by_id(id, em->user_M);
    print_user_info(u, out);
}

void slow_answer2(int N, char *country, Entity_Manager em, Output out){
    int size = length_arr_disc(em->artist_M);
    Artist a = NULL;
    if (N == 0)
        output_empty (out);
    for (int i = 0; i < size && N > 0; i++){
        a = search_artist_by_dur_country(em->artist_M, country, i);
        if (a != NULL){
            print_art_info(a, out);
            N--;
        }
    }
}

void fast_answer2(int N, Entity_Manager em, Output out){
    Artist a = NULL;
    if (N == 0)
        output_empty (out);
    for (int i = 0; i < N; i++){
        a = search_artist_by_dur_indice(em->artist_M, i);
        print_art_info(a, out);
    }
}

void answer3(int min, int max, Entity_Manager em, Output out){
}

void answer_querie(Querie q, Entity_Manager em, int type, int n_querie){
    char output_file[34];
    snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
    Output out = open_out (output_file);
    switch (type)
    {
    case (1):
        int id = get_querie1_info(q);
        answer1(id, em, out);
        break;
    case (2):
        short *N = calloc(1, sizeof(int));
        char *country = get_querie2_info(q, N);
        if (country == NULL)
            fast_answer2(*N, em, out);
        else
            slow_answer2(*N, country, em, out);
        free(N);
        break;
    case (3):
        short *max = calloc(1, sizeof(int));
        short min = get_querie3_info(q, max);
        answer3(min, *max, em, out);
        free(max);
        break;
    }
    close_output (out);
}

void answer_all_queries(Parser queries, Entity_Manager em){
    int i;
    Querie q = create_querie();
    for (i = 1; get_querie_type(q) != -1; i++)
    {
        int type = read_querie_line(queries, q);
        answer_querie(q, em, type, i);
    }
    free_querie(q);
}

//funções normais ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



//funções para programa-testes VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

void answer1_test(int id, Entity_Manager em, Output out, Query_data qd){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    User u = search_user_by_id(id, em->user_M);
    print_user_info(u, out);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;
    
    add_query_data(qd, elapsed, 1);
}

void slow_answer2_test(int N, char *country, Entity_Manager em, Output out, Query_data qd){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    int size = length_arr_disc(em->artist_M);
    Artist a = NULL;
    if (N == 0)
        output_empty (out);
    for (int i = 0; i < size && N > 0; i++){
        a = search_artist_by_dur_country(em->artist_M, country, i);
        if (a != NULL){
            print_art_info(a, out);
            N--;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    add_query_data(qd, elapsed, 2);
}

void fast_answer2_test(int N, Entity_Manager em, Output out, Query_data qd){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);

    Artist a = NULL;
    if (N == 0)
        output_empty (out);
    for (int i = 0; i < N; i++){
        a = search_artist_by_dur_indice(em->artist_M, i);
        print_art_info(a, out);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    add_query_data(qd, elapsed, 2);
}

void answer_querie_test(Querie q, Entity_Manager em, int type, int n_querie, Query_data qd){
    char output_file[34];
    snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
    Output out = open_out (output_file);
    switch (type)
    {
    case (1):
        int id = get_querie1_info(q);
        answer1_test(id, em, out, qd);
        break;
    case (2):
        short *N = calloc(1, sizeof(int));
        char *country = get_querie2_info(q, N);
        if (country == NULL)
            fast_answer2_test(*N, em, out, qd);
        else
            slow_answer2_test(*N, country, em, out, qd);
        free(N);
        break;
    case (3):
        short *max = calloc(1, sizeof(int));
        short min = get_querie3_info(q, max);
        answer3(min, *max, em, out);
        free(max);
        break;
    }
    close_output (out);
}

void answer_all_queries_test(Parser queries, Entity_Manager em, Query_data qd){
    int i;
    Querie q = create_querie();
    for (i = 1; get_querie_type(q) != -1; i++)
    {
        int type = read_querie_line(queries, q);
        answer_querie_test(q, em, type, i, qd);
    }
    free_querie(q);
}

//funções para programa-testes ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



void free_entity_manager(Entity_Manager e){
    free_user_manager(e->user_M);
    free_music_manager(e->music_M);
    free_art_manager(e->artist_M);
    free(e);
}

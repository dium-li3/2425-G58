#include <stdio.h>

#include "entity_manager.h"
#include "user_manager.h"
#include "queries.h"
#include "users.h"
#include "utils.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "artists.h"
#include "musics.h"

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

void store_Entities(FILE **fp, Entity_Manager entity_M){
    store_Artists(fp[2], entity_M->artist_M);
    store_Musics(fp[1], entity_M->music_M, entity_M->artist_M);
    order_duration (entity_M->artist_M);
    store_Users(fp[0], entity_M->user_M, entity_M->music_M);
}

// isto ficará no IO?

void answer1(int id, Entity_Manager em, int n_querie){
    User u = search_user_by_id(id, em->user_M);
    print_user_info(u, n_querie);
}

void slow_answer2(int N, char *country, Entity_Manager em, FILE *fp){
    int size = length_arr_disc(em->artist_M);
    Artist a = NULL;
    if (N == 0)
        fprintf(fp, "\n");
    for (int i = 0; i < size && N > 0; i++){
        a = search_artist_by_dur_country(em->artist_M, country, i);
        if (a != NULL){
            print_art_info(a, fp);
            N--;
        }
    }
}

void fast_answer2(int N, Entity_Manager em, FILE *fp){
    Artist a = NULL;
    if (N == 0)
        fprintf(fp, "\n");
    for (int i = 0; i < N; i++){
        a = search_artist_by_dur_indice(em->artist_M, i);
        print_art_info(a, fp);
    }
}

void answer3(int min, int max, Entity_Manager em, FILE *fp){
    Genre gen = NULL;
    get_total_likes(em->music_M,min,max);
    sort_gen(em->music_M,min,max);
    int gen_arr_len = get_gen_arr_len(em->music_M);
    int escreveu = 0;
    for (int i = 0; i < gen_arr_len; i++){
        gen = get_genre_by_index(em->music_M, i);
        escreveu += print_genre_info(gen, fp);
    }
    if (!escreveu)
        fprintf(fp, "\n");
}

void answer_querie(Querie q, Entity_Manager em, int type, int n_querie){
    switch (type)
    {
    case (1):
        int id = get_querie1_info(q);
        answer1(id, em, n_querie);
        break;
    case (2):
        short *N = calloc(1, sizeof(int));
        char *country = get_querie2_info(q, N);
        char output_file[34];
        snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
        FILE *fp = fopen(output_file, "w+");
        if (country == NULL)
            fast_answer2(*N, em, fp);
        else
            slow_answer2(*N, country, em, fp);
        fclose (fp);
        free(N);
        break;
    case (3):
        short *max = calloc(1, sizeof(int));
        short min = get_querie3_info(q, max);
        snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
        fp = fopen(output_file, "w+");
        answer3(min, *max, em,fp);
        free(max);
        fclose (fp);
        break;
    }
}

void answer_all_queries(FILE *fp_queries, Entity_Manager em){
    int i;
    Querie q = create_querie();
    for (i = 1; get_querie_type(q) != -1; i++)
    {
        int type = read_querie_line(fp_queries, q);
        answer_querie(q, em, type, i);
    }
    free_querie(q);
}

void free_entity_manager(Entity_Manager e){
    free_user_manager(e->user_M);
    free_music_manager(e->music_M);
    free_art_manager(e->artist_M);
    free(e);
}

#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L
#include <glib.h>


#include "parser.h"
#include "user_manager.h"
#include "users.h"
#include "artist_manager.h"
#include "artists.h"
#include "music_manager.h"
#include "genre.h"
#include "output.h"
#include "queries.h"

/*
    Estrutura para armazenar o nº de execuções de uma dada query (n)
    e o tempo total para essas execuções(time).
    O índice para uma query do tipo q é q-1.
*/
typedef struct query_stats {
    int n[3];
    double time[3];
} *Query_stats;

/*
    O objetivo será criar uma única querie, que tem os seus dados
    atualizados ao ler cada linha.
*/
typedef struct querie{
    short querie;
    Querie1 querie1;
    Querie2 querie2;
    Querie3 querie3;
} *Querie;

typedef struct querie1{
    int id; //username a procurar nos nossos dados.
} *Querie1;

typedef struct querie2{
    short N; //top N artistas
    char *country;//Pode ser NULL
} *Querie2;

typedef struct querie3{
    short min; //idade minima
    short max; //idade maxima
} *Querie3;

Querie create_querie(){
    Querie q = calloc (1, sizeof(struct querie));//define logo int querie a 0.
    q->querie1 = calloc (1, sizeof(struct querie1));
    q->querie2 = calloc (1, sizeof(struct querie2));
    q->querie3 = calloc (1, sizeof(struct querie3));
    return q;
}


void set_querie_invalid(Querie q){
    q->querie = -1;
}

void set_querie1(int id, Querie q){
    q->querie = 1;
    q->querie1->id = id;
}

void set_querie2(short N, char *country, Querie q){
    q->querie = 2;
    q->querie2->N = N;
    if (country != NULL){
        free(q->querie2->country);
        q->querie2->country = strdup (country);
    }
    else{
        free(q->querie2->country);
        q->querie2->country = NULL;
    }
}

void set_querie3(short min, short max, Querie q){
    q->querie = 3;
    q->querie3->min = min;
    q->querie3->max = max;
}

short get_querie_type(Querie q){
    return q->querie;
}

/*
    Devolve o username a procurar pela querie 1,
*/
int get_querie1_info(Querie q){

    return q->querie1->id;
}

/*
    Devolve a country (se a querie 2 a tiver)
    e atualiza o N dado para ter o valor do N
    da querie 2.
*/
char *get_querie2_info(Querie q, short *N){
    *N = q->querie2->N;
    return q->querie2->country;
}

/*
    Devolve o minimo da querie 3, e atualiza
    o max dado para ter o valor do maximo da querie 3.
*/
short get_querie3_info(Querie q, short *max){
    *max = q->querie3->max;
    return q->querie3->min;
}

void store_querie_from_token (Querie q, char **tokens, int n_tokens){
    for (int i = 0; i < 3; i++){
    }
    if (tokens[0] != NULL){
        q->querie = (short) atoi(tokens[0]);//numero da querie.
        switch (q->querie){
            case (1): //lol sem querer dei cast disto para short e fiquei rip durante um tempo até encontrar o erro
                set_querie1 ((int) atoi(tokens[1] + 1), q); //+1 para ignorar logo o U
                break;
            case (2):
                set_querie2 ((short) atoi(tokens[1]), tokens[2], q);
                break;
            case (3):
                set_querie3 ((short) atoi(tokens[1]), (short) atoi(tokens[2]), q);
                break;
            default:
                set_querie_invalid (q);
        }
    }
    else set_querie_invalid(q);//não leu tokens
}

void read_querie_line(Parser pq, Querie q){
    char **tokens = calloc (3, sizeof(char *));//basta 3 espaços por agora
    int n_tokens = parse_1line_querie(pq, tokens);
    store_querie_from_token (q, tokens, n_tokens);
    if (get_querie_type(q) != -1)
        for (int i = 0; i < 3; i++){
            free (tokens[i]);
        }
    free (tokens);
}


void free_querie2 (Querie2 q){
    free (q->country);
    free (q);
}

void free_querie3 (Querie3 q){
    free (q);
}

void free_querie (Querie q){
    free (q->querie1);
    free_querie2 (q->querie2);
    free_querie3 (q->querie3);
    free (q);
}


void answer1(Querie q, User_Manager um, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Querie1 q1 = q->querie1;
    User u = search_user_by_id(q1->id, um);
    print_user_info(u, out);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;
    
    if (qs != NULL) add_query_stats(qs, elapsed, 1);
}

void answer2(Querie q, Art_Manager am, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Querie2 q2 = q->querie2;
    int size = length_arr_disc(am);
    Artist a = NULL;
    int N = q2->N;
    if (N == 0)
        output_empty (out);
    else {
        if (q2->country == NULL){
            for (int i = 0; i < N; i++){
                a = search_artist_by_dur_indice(am, i);
                print_art_info(a, out);
            }
        }
        else {
            for (int i = 0; i < size && N > 0; i++){
                a = search_artist_by_dur_country(am, q2->country, i);
                if (a != NULL){
                    print_art_info(a, out);
                    N--;
                }
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (qs != NULL) add_query_stats(qs, elapsed, 2);
}

void answer3(Querie q, Music_Manager mm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Querie3 q3 = q->querie3;
    Genre gen = NULL;
    sort_gen(mm, q3->min, q3->max);
    int gen_arr_len = get_gen_arr_len(mm);
    int escreveu = 0;
    for (int i = 0; i < gen_arr_len; i++){
        gen = get_genre_by_index(mm, i);
        escreveu += print_genre_info(gen, out);
    }
    if (!escreveu)
        output_empty(out);
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (qs != NULL) add_query_stats(qs, elapsed, 3);
}

Query_stats create_query_stats() {
    Query_stats r = calloc(1, sizeof(struct query_stats));
    return r;
}

/*
    Incrementa automaticamente o nº de execuções.
*/
void add_query_stats(Query_stats qd, double time, int type) {
    qd->n[type-1]++;
    qd->time[type-1] += time;
}

double get_query_stats_time(Query_stats qd, int i) {
    return (qd->time[i]);
}

int get_query_stats_n(Query_stats qd, int i) {
    return (qd->n[i]);
}
#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L
#include <glib.h>
#include <string.h>


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
    O objetivo será criar uma única query, que tem os seus dados
    atualizados ao ler cada linha.
*/
typedef struct query{
    short query;
    Query1 query1;
    Query2 query2;
    Query3 query3;
    char separador; //caracter que separa os membros do output, dependendo de se o tipo da query tem um S
} *Query;

typedef struct query1{
    int id; //username a procurar nos nossos dados.
} *Query1;

typedef struct query2{
    short N; //top N artistas
    char *country;//Pode ser NULL
} *Query2;

typedef struct query3{
    short min; //idade minima
    short max; //idade maxima
} *Query3;

Query create_query(){
    Query q = calloc (1, sizeof(struct query));//define logo int query a 0.
    q->query1 = calloc (1, sizeof(struct query1));
    q->query2 = calloc (1, sizeof(struct query2));
    q->query3 = calloc (1, sizeof(struct query3));
    q->separador = ';';
    return q;
}


void set_query_invalid(Query q){
    q->query = -1;
}

void set_query1(int id, Query q){
    q->query = 1;
    q->query1->id = id;
}

void set_query2(short N, char *country, Query q){
    q->query = 2;
    q->query2->N = N;
    if (country != NULL){
        free(q->query2->country);
        q->query2->country = strdup (country);
    }
    else{
        free(q->query2->country);
        q->query2->country = NULL;
    }
}

void set_query3(short min, short max, Query q){
    q->query = 3;
    q->query3->min = min;
    q->query3->max = max;
}

short get_query_type(Query q){
    return q->query;
}

char get_separador (Query q){
    return q->separador;
}

void store_query_from_token (Query q, char **tokens, int n_tokens){
    for (int i = 0; i < 3; i++){
    }
    if (tokens[0] != NULL){
        
        if (strlen (tokens[0]) > 1)
            q->separador = '=';
        
        q->query = (short) atoi(tokens[0]);//numero da query.
        switch (q->query){
            case (1):
                set_query1 ((int) atoi(tokens[1] + 1), q); //+1 para ignorar logo o U
                break;
            case (2):
                set_query2 ((short) atoi(tokens[1]), tokens[2], q);
                break;
            case (3):
                set_query3 ((short) atoi(tokens[1]), (short) atoi(tokens[2]), q);
                break;
            default:
                set_query_invalid (q);
        }
    }
    else set_query_invalid(q);//não leu tokens
}

void read_query_line(Parser pq, Query q){
    char **tokens = calloc (3, sizeof(char *));//basta 3 espaços por agora
    int n_tokens = parse_1line_query(pq, tokens);
    store_query_from_token (q, tokens, n_tokens);
    if (q->query != -1)
        for (int i = 0; i < 3; i++){
            free (tokens[i]);
        }
    free (tokens);
}


void free_query2 (Query2 q){
    free (q->country);
    free (q);
}

void free_query3 (Query3 q){
    free (q);
}

void free_query (Query q){
    free (q->query1);
    free_query2 (q->query2);
    free_query3 (q->query3);
    free (q);
}


void answer1(Query q, User_Manager um, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query1 q1 = q->query1;
    print_user_info_by_id (um, q1->id, q->separador, out);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;
    
    if (qs != NULL) add_query_stats(qs, elapsed, 1);
}

void answer2(Query q, Art_Manager am, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query2 q2 = q->query2;
    int N = q2->N;
    if (N == 0)
        output_empty (out);
    else {
        if (q2->country == NULL)
            print_N_art_info (am, N, q->separador, out);
        else {
            char *country = NULL;
            country = strdup (q2->country);
            print_N_country_art_info (am, country, N, q->separador, out);
            free (country);
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (qs != NULL) add_query_stats(qs, elapsed, 2);
}

void answer3(Query q, Music_Manager mm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query3 q3 = q->query3;
    sort_gen(mm, q3->min, q3->max);

    print_all_genres_info (mm, q->separador, out);

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
#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L
#include <glib.h>
#include <string.h>


#include "parser.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "history_manager.h"
#include "output.h"
#include "queries.h"
#include "utils.h"
#include "recomendador.h"

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
    Query4 query4;
    Query5 query5;
    Query6 query6;
    char separador; //caracter que separa os membros do output, dependendo de se o tipo da query tem um S
} *Query;

typedef struct query1{
    int id; //id a procurar nos nossos dados.
    char type; // A == artist e U == User
} *Query1;

typedef struct query2{
    short N; //top N artistas
    char *country;//Pode ser NULL
} *Query2;

typedef struct query3{
    short min; //idade minima
    short max; //idade maxima
} *Query3;

typedef struct query4{
    int from_week;
    int to_week;
} *Query4;

typedef struct query5{
    char *user_id;
    int N_results;
} *Query5;

typedef struct query6{
    int user_id;
    int year;
    int N_artists;
} *Query6;

Query create_query(){
    Query q = calloc (1, sizeof(struct query));//define logo int query a 0.
    q->query1 = calloc (1, sizeof(struct query1));
    q->query2 = calloc (1, sizeof(struct query2));
    q->query3 = calloc (1, sizeof(struct query3));
    q->query4 = calloc (1, sizeof(struct query4));
    q->query5 = calloc (1, sizeof(struct query5));
    q->query6 = calloc (1, sizeof(struct query6));

    q->separador = ' ';
    return q;
}


void set_query_invalid(Query q){
    q->query = -1;
}

void set_query1(int id, char entity, Query q){
    q->query = 1;
    q->query1->id = id;
    q->query1->type = entity;
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

void set_query4(int first_week, int last_week, Query q){
    q->query = 4;
    q->query3->min = first_week;
    q->query3->max = last_week;
}

void set_query5(char *user_id, int N, Query q){
    q->query = 5;
    q->query5->N_results = N;
    if (user_id != NULL){
        free (q->query5->user_id);
        q->query5->user_id = strdup (user_id);
    }
    else{
        free (q->query5->user_id);
        q->query5->user_id = NULL;
    }
}

void set_query6(int user_id, int year, int N, Query q){
    q->query = 6;
    q->query6->user_id = user_id;
    q->query6->year = year;
    q->query6->N_artists = N;
}


short get_query_type(Query q){
    return q->query;
}

char get_separador (Query q){
    return q->separador;
}

void store_query_from_token (Query q, char **tokens, int n_tokens){
    int N = 0;
    if (tokens[0] != NULL){
        
        if (strlen (tokens[0]) > 1)
            q->separador = '=';
        else
            q->separador = ';';
        
        q->query = (short) atoi(tokens[0]);//numero da query.
        switch (q->query){
            case (1):
                set_query1 ((int) atoi(tokens[1] + 1), *tokens[1], q); //+1 para ignorar logo o U ou A
                break;
            case (2):
                set_query2 ((short) atoi(tokens[1]), tokens[2], q);
                break;
            case (3):
                set_query3 ((short) atoi(tokens[1]), (short) atoi(tokens[2]), q);
                break;
            case (4):
                set_query4 (0, 0, q);
                break;
            case (5):
                set_query5 (tokens[1], atoi (tokens[2]), q);
                break;
            case (6):
                if (tokens[3] != NULL)
                    N = atoi (tokens[3]);
                else N = 0;
                set_query6 (atoi (tokens[1] + 1), atoi (tokens[2]), N, q);
                break;
            default:
                set_query_invalid (q);
        }
    }
    else set_query_invalid(q);//não leu tokens
}

void read_query_line(Parser pq, Query q){
    char **tokens = calloc (4, sizeof(char *));//basta 3 espaços por agora
    int n_tokens = parse_1line_query(pq, tokens);
    store_query_from_token (q, tokens, n_tokens);
    if (q->query != -1)
        for (int i = 0; i < 4; i++){
            free (tokens[i]);
        }
    free (tokens);
}


void free_query2 (Query2 q){
    free (q->country);
    free (q);
}

void free_query (Query q){
    free (q->query1);
    free_query2 (q->query2);
    free (q->query3);
    free (q->query4);
    free (q->query5);
    free (q->query6);
    free (q);
}


void answer1(Query q, User_Manager um, Art_Manager am, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query1 q1 = q->query1;
    if (q1->type == 'U')
        print_user_res_by_id (um, q1->id, out);
    else
        print_art_res_by_id (am, q1->id, out);
    
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
            print_N_art_info (am, N, out);
        else {
            char *country = NULL;
            country = strdup (q2->country);
            print_N_country_art_info (am, country, N, out);
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

    print_all_genres_info (mm, out);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (qs != NULL) add_query_stats(qs, elapsed, 3);
}

void answer4(Query q, Output out, Query_stats qs){
    output_empty (out);
}

void answer5(Query q, User_Manager um, Music_Manager mm,History_Manager hm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    if(q->query5->N_results == 0 || !user_exists (atoi (q->query5->user_id + 1), um)) {
        output_empty(out);
    } 
    else {
        Query5 q5 = q->query5;
        char *idUtilizadorAlvo = q5->user_id;
        int **matrizClassificacaoMusicas = get_matrix(hm);
        char **idsUtilizadores = get_users_ids(um);
        char **nomesGeneros = get_genre_names(mm);
        int numUtilizadores = get_total_users(um);
        int numGeneros = get_total_genres(mm);
        int numRecomendacoes = q5->N_results;
        
        char **arrAnswer;

        arrAnswer = recomendaUtilizadores(idUtilizadorAlvo, matrizClassificacaoMusicas, idsUtilizadores,nomesGeneros,numUtilizadores,numGeneros,numRecomendacoes);
        
        for (int i = 0;i<numRecomendacoes;i++) {
            output_geral(arrAnswer + i,1,out);
        }
        
        free (arrAnswer);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;

    if (qs != NULL) add_query_stats(qs, elapsed, 3);
}

void answer6(Query q, Output out, Query_stats qs){
    output_empty (out);
}

//Lógica de resposta à query 6



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
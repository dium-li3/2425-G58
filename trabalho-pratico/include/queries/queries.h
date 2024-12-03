#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>

#include "output.h"
#include "parser.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "history_manager.h"
#include "testagem.h"

#define QUERYTYPES 3

typedef struct query_stats *Query_stats;

typedef struct query *Query;

typedef struct query1 *Query1;

typedef struct query2 *Query2;

typedef struct query3 *Query3;

typedef struct query4 *Query4;

typedef struct query5 *Query5;

typedef struct query6 *Query6;

//Cria uma struct query com espaço para as 3 queries atuais, todas já alocadas mas vazias.
Query create_query();

//Atualiza a query para ter informação relevante à query1.
void set_query1(int id, char type, Query q);

//Atualiza a query para ter informação relevante à query2.
void set_query2(short N, char *country, Query q);

//Atualiza a query para ter informação relevante à query3.
void set_query3(short min, short max, Query q);

//Devolve o tipo de query que tem de ser respondida.
short get_query_type(Query q);

//Devolve o caractér a ser usado como separador no output de uma dada query.
char get_separador (Query q);

//Enche a query com a informação dada relevante ao tipo de query que a informação dá.
void store_query_from_token (Query q, char **tokens, int n_tokens);

//Guarda a informação de uma linha de queries numa query.
void read_query_line(Parser pq, Query q);

void free_query (Query q);

void answer1(Query q, User_Manager um, Art_Manager am, Output out, Query_stats qs);

void answer2(Query q, Art_Manager am, Output out, Query_stats qs);

void answer3(Query q, Music_Manager mm, Output out, Query_stats qs);

void answer4(Query q, Output out, Query_stats qs);

void answer5(Query q, Output out, Query_stats qs);

void answer6(Query q, Art_Manager am, Music_Manager mm, User_Manager um, History_Manager hm, Output out, Query_stats qs);

Query_stats create_query_stats();

//Incrementa automaticamente o nº de execuções.
void add_query_stats(Query_stats qs, double time, int type);

double get_query_stats_time(Query_stats qs, int i);

int get_query_stats_n(Query_stats qs, int i);

#endif
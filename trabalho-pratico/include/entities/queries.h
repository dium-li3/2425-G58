#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>

#include "output.h"
#include "parser.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "testagem.h"

#define QUERYTYPES 3

typedef struct query_stats *Query_stats;

typedef struct querie *Querie;

typedef struct querie1 *Querie1;

typedef struct querie2 *Querie2;

typedef struct querie3 *Querie3;

//Cria uma struct querie com espaço para as 3 queries atuais, todas já alocadas mas vazias.
Querie create_querie();

//Atualiza a querie para ter informação relevante à querie1.
void set_querie1(int id, Querie q);

//Atualiza a querie para ter informação relevante à querie2.
void set_querie2(short N, char *country, Querie q);

//Atualiza a querie para ter informação relevante à querie3.
void set_querie3(short min, short max, Querie q);

//Enche a querie com a informação dada relevante ao tipo de querie que a informação dá.
void store_querie_from_token (Querie q, char **tokens, int n_tokens);

//Guarda a informação de uma linha de queries numa querie.
void read_querie_line(Parser pq, Querie q);

void free_querie (Querie q);

void answer1(Querie q, User_Manager um, Output out, Query_stats qs);

void answer2(Querie q, Art_Manager am, Output out, Query_stats qs);

void answer3(Querie q, Music_Manager mm, Output out, Query_stats qs);

Query_stats create_query_stats();

//Incrementa automaticamente o nº de execuções.
void add_query_stats(Query_stats qs, double time, int type);

double get_query_stats_time(Query_stats qs, int i);

int get_query_stats_n(Query_stats qs, int i);

#endif
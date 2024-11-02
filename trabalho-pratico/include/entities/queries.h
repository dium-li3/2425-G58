#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>

#include "parser.h"

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

//Devolve o tipo de querie que tem de ser respondida.
short get_querie_type(Querie q);

/*
    Devolve o username a procurar pela querie 1,
    // só devolve um username válido se a informação útil da querie
    // for realmente da querie 1.
*/
int get_querie1_info(Querie q);

/*
    Devolve a country (se a querie 2 a tiver)
    e atualiza o N dado para ter o valor do N
    da querie 2.
*/
char *get_querie2_info(Querie q, short *N);

/*
    Devolve o minimo da querie 3, e atualiza
    o max dado para ter o valor do maximo da querie 3.
*/
short get_querie3_info(Querie q, short *max);

//Enche a querie com a informação dada relevante ao tipo de querie que a informação dá.
int store_querie_from_token (Querie q, char **tokens, int n_tokens);

//Guarda a informação de uma linha de queries numa querie.
int read_querie_line(Parser pq, Querie q);

void free_querie (Querie q);


#endif
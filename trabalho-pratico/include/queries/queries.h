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

#define QUERYTYPES 6

typedef struct query_stats *Query_stats;

typedef struct query *Query;

typedef struct query1 *Query1;

typedef struct query2 *Query2;

typedef struct query3 *Query3;

typedef struct query4 *Query4;

typedef struct query5 *Query5;

typedef struct query6 *Query6;

/**
 * @brief Cria um gestor de álbuns.
 * 
 * Aloca memória para um gestor de álbuns, assim como inicializa a sua hash table.
 * Destaca-se que, juntamente com a sua criação, estabele-se que a função @b free_album
 * será a responsável por liberar a memória de cada entrada presente na tabela, quando
 * está for eliminada.
 * 
 * @return Gestor de álbuns.
*/

/**
 * @brief Aloca espaço para um struct query.
 * 
 * Aloca espaço na memória para as 6 queries, inicializando-as vazias a princípio.
 * O campo @b separador é inicialmente definido como o carácter ' '.
 * 
 * @return Struct query recém-criado.
*/
Query create_query();

/**
 * @brief Devolve o número (tipo) da query a ser respondida.
 * 
 * Acessa o campo @b query do struct @p q, que representa o número da query (de um
 * a 6) que se pretende responder. O valor no campo acedido é então retornado.
 * 
 * @param q Struct cujo número será devolvido.
 * @return Número da query @p q.
*/
short get_query_type(Query q);

/**
 * @brief Retorna o caractér a ser usado como separador no output de uma dada query.
 * 
 * Acessa o campo @b separador do struct @p q, caracter que separa os membros do output,
 * caso o tipo da query tenha um S. O valor no campo acedido é então retornado.
 * 
 * @param q Struct cujo separador será devolvido.
 * @return Struct query recém-criado.
*/
char get_separador (Query q);

/**
 * @brief Guarda a informação de uma linha de queries numa query.
 * 
 * Recebe uma linha do ficheiro de input e atribui o seu conteúdo a um struct query @p q,
 * definindo o seu separador mediante o comprimento do primeiro token, e extraindo o seu 
 * tipo para que seja possível invocar o inicializador de queries apropriado, o qual passará
 * as restantes informações da linha para um dos substructs de @p q (Query1, Query1, Query2,
 * Query3, Query4, Query5 ou Query6). Os tokens alocados no final da função têm sua memória
 * libertada.
 * 
 * @param pq Estrutura que se mantém a par de um ficheiro aberto.
 * @param q Struct que armazenará as informações da linha.
*/
void read_query_line(Parser pq, Query q);

/**
 * @brief Libera o espaço alocado por uma query.
 * 
 * Dá free da query @p q e de todos os seus substructs, invocando funções de free especias para
 * os campos @b query2 e @b query5, que possui strings nos campos destas e por isso requerem um
 * cuidado especial.
 * 
 * @param q Query a ser libertada.
*/
void free_query (Query q);

void answer1(Query q, User_Manager um, Art_Manager am, Output out, Query_stats qs);

void answer2(Query q, Art_Manager am, Output out, Query_stats qs);

void answer3(Query q, Music_Manager mm, Output out, Query_stats qs);

void answer4(Query q, Output out, Query_stats qs, Art_Manager am);

void answer5(Query q, User_Manager um, Music_Manager mm,History_Manager hm, Output out, Query_stats qs);

void answer6(Query q, Art_Manager am, Music_Manager mm, User_Manager um, History_Manager hm, Output out, Query_stats qs);

Query_stats create_query_stats();

//Incrementa automaticamente o nº de execuções.
void add_query_stats(Query_stats qs, double time, int type);

double get_query_stats_time(Query_stats qs, int i);

int get_query_stats_n(Query_stats qs, int i);

#endif
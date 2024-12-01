#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include "parser.h"
#include "testagem.h"

typedef struct output *Output;

Output open_out (char *path, char separador);

void close_output(Output out);

void output_empty (Output out);

void output_geral (char **infos, int n_infos,Output out);

void error_output (Parser p, Output out);


/**
 * @brief Percorre @p a para ir imprimindo as linhas onde há erros (programa de testes).
 * 
 * @param a Array com o nº das linhas de um determinado ficheiro de resultados onde ocorreram erros.
 */
void print_query_errors(GArray *a);


/**
 * @brief Imprime os tempos das várias etapas do programa.
 * 
 * Percorre @p elapsed para imprimir os tempos das etapas, sendo 0->total, 1->store, 2->answer e 3->free.
 * 
 * @param elapsed Array com os tempos (criado na main_test).
 */
void print_elapsed_times(double elapsed[]);


/**
 * @brief Imprime os tempos de execução das diferentes queries.
 * 
 * Os argumentos são uma "decomposição" da struct QueryStats (problemazinho do import cíclico...).
 * Para cada tipo de query, é imprimido o tempo total gasto nesse tipo e o tempo médio por execução.
 * 
 * @param type Tipo da query.
 * 
 * @param n Número de vezes que a query foi executada.
 * 
 * @param time Tempo total de execução da query (todas as @p n execuções somadas).
 */
void print_query_time(int type, int n, double time);

#endif
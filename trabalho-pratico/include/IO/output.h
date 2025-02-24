/**
 * @file output.h
 * @author Grupo 58
 * @date Out 2024
 * @brief Header file das funções de output
 */
#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include "parser.h"
#include "testagem.h"

typedef struct output *Output;

/**
 * @brief Cria um Output.
 * 
 * Cria um Output com um caminho @p path para um ficheiro e um caractér @p separador
 * que vai separar cada elemento que o output for dar print de uma só vez.
 * 
 * @param path Caminho para um ficheiro.
 * @param separador Caractér que separa o que está a ser escrito numa mesma linha
 * @param interativo Indica se o programa está a ser executado no modo interativo ou não (booleano)
 *
 * @return Estrutura de output 
 */
Output open_out (char *path, char separador, int interativo);

/**
 * @brief Liberta o espaço ocupado pelo output.
 * 
 * Liberta o espaço ocupado pela estrutura do output @p out.
 * 
 * @param out Output
 */
void close_output(Output out);

/**
 * @brief Escreve uma linha vazia.
 * 
 * Escreve uma linha vazia no ficheiro de output.
 * 
 * @param out Output
 */
void output_empty (Output out);

/**
 * @brief Escreve um dado número de elementos de @p infos no ficheiro de output.
 * 
 * Escreve um dado número @p n_infos de elementos de @p infos no ficheiro de output,
 * cada elemento separado por um caractér separador guardado pelo output @p out.
 * 
 * @param infos 'Lista' de elementos
 * @param n_infos Número de elementos
 * @param out Output
 */
void output_geral (char **infos, int n_infos, Output out);

/**
 * @brief Escreve uma linha num ficheiro de erros.
 * 
 * Escreve uma linha de um ficheiro CSV num ficheiro de erros.
 * 
 * @param p Ficheiro CSV
 * @param out Ficheiro de erros
 */
void error_output (Parser p, Output out);

/**
 * @brief Define o booleano @c terminal a verdadeiro.
 * 
 * @param out Ficheiro de output.
 */
void set_terminal_true(Output out);


/**
 * @brief Percorre um array para imprimir as linhas erradas.
 * 
 * Percorre @p a para ir imprimindo as linhas onde há erros (programa de testes).
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
 * @param n Número de vezes que a query foi executada.
 * @param time Tempo total de execução da query (todas as @p n execuções somadas).
 */
void print_query_time(int type, int n, double time);

#endif
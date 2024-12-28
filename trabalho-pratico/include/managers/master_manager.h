#ifndef MASTER_MANAGER_H
#define MASTER_MANAGER_H
#include <stdio.h>
#include "parser.h"
#include "queries.h"


typedef struct master_manager *Master_Manager;


/**
 * @brief Aloca memória para um Master Manager, i.e., para cada um dos gestores que o Master Manager contém.
 * 
 * @return Apontador para a estrutura do Master Manager.
 */
Master_Manager create_master_manager();


/**
 * @brief Armazena a informação das entidades.
 * 
 * Índices de @p entity_paths : 0->utilizadores, 1->músicas e 2->artistas
 * Também ordena o array de artistas por tempo de discografia e calcula todos os top 10.
 * Caso não tenha sido possível abrir algum dos ficheiros, retorna 1.
 * 
 * @param entity_paths Array com os caminhos para os ficheiros CSV.
 * @param master_M Contém os gestores das entidades.
 * 
 * @return Código de sucesso.
 */
int store_Entities (char **entity_paths, Master_Manager master_M);


/**
 * @brief Responde a todas as queries.
 * 
 * @param queries Parser com o caminho para o ficheiro de inputs.
 * @param mm Master Manager (contém todos os gestores).
 * @param qs Contém os tempos de execução e nº total de execuções para cada tipo de query.
 * @param interativo Indica se o programa está a ser executado no modo interativo ou não.
 */
void answer_all_queries(Parser queries, Master_Manager mm, Query_stats qs, int interativo);


/**
 * @brief Liberta a memória alocada para um Master Manager, i.e., liberta todos os gestores.
 * 
 * @param m Contém todos os gestores.
 */
void free_master_manager (Master_Manager m);

#endif
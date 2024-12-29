#ifndef MASTER_MANAGER_H
#define MASTER_MANAGER_H
#include <stdio.h>
#include "parser.h"
#include "queries.h"

typedef struct master_manager *Master_Manager;

/**
 * @brief Aloca memória para um Master Manager.
 * 
 * Inicializa cada um dos gestores do Master Manager.
 * 
 * @return Apontador para a estrutura do Master Manager.
 */
Master_Manager create_master_manager();

/**
 * @brief Armazena a informação das entidades.
 * 
 * Índices de @p entity_paths : 0->utilizadores, 1->músicas e 2->artistas
 * Também ordena o array de artistas por tempo de discografia.
 * 
 * @param entity_paths Array com os caminhos para os ficheiros CSV.
 * @param master_M Contém os gestores das entidades.
 */
void store_Entities (char **entity_paths, Master_Manager master_M);

/**
 * @brief Responde a todas as queries.
 * 
 * Invoca a função @b answer_query para cada linha do ficheiro dos inputs, dessa
 * forma respondendo cada query mediante o seu tipo.
 * 
 * @param queries Parser com o caminho para o ficheiro de inputs.
 * @param mm Master Manager (contém todos os gestores).
 * @param qs Contém os tempos de execução e nº total de execuções para cada tipo de query.
 */
void answer_all_queries(Parser queries, Master_Manager mm, Query_stats qs);

/**
 * @brief Liberta a memória alocada para um Master Manager
 * 
 * "Dá free" do espaço na memória ocupado por todos os gestores de @p m, e do espaço ocupado
 * pelo prório.
 * 
 * @param m Estrutura detentora de todos os gestores.
 */
void free_master_manager (Master_Manager m);

#endif
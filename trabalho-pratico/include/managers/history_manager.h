/**
 * @file history_manager.h
 * @author Grupo 58
 * @date Nov 2024
 * @brief Header file do gestor de históricos
 */
#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <glib.h>

#include "history.h"
#include "user_manager.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "history_manager.h"

typedef struct history_manager *History_Manager;

/**
 * @brief Cria um gestor de históricos.
 * 
 * Aloca memória para um gestor de históricos, assim como inicializa a sua hash table.
 * Destaca-se que, juntamente com a sua criação, estabele-se que a função @b free_history
 * será a responsável por liberar a memória de cada entrada presente na tabela, quando
 * está for eliminada. A matriz que contém o número de reproduções de cada gênero por cada
 * usuário é aqui inicializada como nula.
 * 
 * @return Gestor de históricos.
 */
History_Manager create_history_manager ();

/**
 * @brief Liberta o espaço usado pela matriz guardada no gestor de históricos @p hm .
 * 
 * @param hm Gestor de históricos.
 */
void free_matrix (History_Manager hm);

/**
 * @brief Libera a memória ocupada pelo gestor de históricos.
 * 
 * Dá free do struct e da hash table deste, dado um gestor @p hm. Somado a isso, também
 * acessa o número de linhas da matriz do gestor com a finalidade de liberar o espaço por
 * aquela ocupado.
 * 
 * @param hm Gestor cuja memória será liberada
 */
void free_history_manager (History_Manager hm);


/**
 * @brief Armazena todos os históricos.
 *
 * Primeiramente inicializa a matriz do @p history_manager, definindo o seu nº de linhas e colunas com
 * base no seu total de users e genêros, respectivamente. Em seguida, acessa uma linha do ficheiro CSV e
 * insere o histórico em questão na hash table do @p history_manager, dado o seu @b id, assim como
 * contabiliza essa reprodução na matriz do gestor de históricos. Após isso, soma à receita total de um
 * artista o valor que aquela reprodução representa, e adiciona o id do histórico da linha atual do 
 * ficheiro CSV no array de históricos por ano que o usuário detentor desse usuário possui. Logo após,
 * calcula a semana em que aquele histórico foi concebido e verifica se essa é a semana mais distante do
 * 9/9/2024 onde ocorreram históricos. Finalmente, adiciona uma duração de reprodução a cada integrante de
 * um dado array de artistas.
 * 
 * @param history_path Caminho para o ficheiro CSV dos históricos.
 * @param history_man History Manager que armazenará os históricos.
 * @param am Gestor de artistas.
 * @param mm Gestor de músicas.
 * @param um Gestor de usuários.
 * @param interativo Indica se o programa está no modo interativo ou não.
 *
 * @return Código de sucesso.
 */
int store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um, int interativo);

/**
 * @brief Devolve a matriz do gestor de históricos.
 *
 * Devolve uma cópia da matriz de reproduções do @p hm.
 * 
 * @param hm Gestor detentor da matriz a ser devolvida.
 * @return Matriz do gestor.
 */
int **get_matrix(History_Manager hm);

/**
 * @brief Copia a informação guardada num histórico para outras variaveis.
 * 
 * Procura um histórico no gestor @p hm dado o seu identificador @p history_id a partir da função
 * @b search_history_by_id e copia o valor dos campos desse histórico para outras variáveis.
 * 
 * @param history_id id do histórico cuja informação queremos. 
 * @param listening_time apontador para onde vamos guardar a duração guardado pelo histórico.
 * @param music_id apontador para onde vamos guardar o id da música à qual o histórico faz referência.
 * @param month apontador para onde vamos guardar o mês no qual o histórico aconteceu.
 * @param day apontador para onde vamos guardar o dia no qual o histórico aconteceu.
 * @param hour apontador para onde vamos guardar a hora do dia na qual o histórico aconteceu.
 * @param hm Gestor que contém todos os históricos.
 */
void get_history_info (int history_id, int *listening_time, int *music_id, int *month, int *day, int *hour, History_Manager hm);

#endif
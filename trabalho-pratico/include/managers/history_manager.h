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
 * @brief Devolve a string com o caminho para o ficheiro CSV de históricos guardada pelo
 * @p hm. 
 * 
 * @param hm Gestor de históricos
 * @return const char* 
 */
const char *get_history_path (History_Manager hm);

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
 * @brief Insere um histórico no seu gestor.
 * 
 * Dado um histórico, adiciona-o no gestor de históricos usando o seu identificador único
 * como a chave para a hash table. 
 * 
 * @param hi Histórico a ser adicionado na hash table do gestor.
 * @param id Identificador único do histórico a ser adicionado, usado como chave para a tabela.
 * @param history_manager gestor cuja tabela terá um novo histórico a ela adicionado.
*/
void insert_history_by_id (History hi, int id, History_Manager history_manager);

/**
 * @brief Verifica se um determinado histórico está presente na hash table do seu gestor, devolvendo-o
 * em caso afirmativo.
 * 
 * Dado um @p id e um gestor @p am, devolve o histórico cujo @p id se especifica, caso este esteja presente
 * no gestor, ou @b NULL, caso contrário.
 * 
 * @param id Identificador único do histórico a ser procurado na tabela.
 * @param history_manager Gestor cuja tabela será revistada a procura do @p id.
 * @return Histórico com identificador @p id (ou @b NULL, na ausência dele no gestor).
*/
History search_history_by_id(int id, History_Manager history_manager);

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
 */
void store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um);

/**
 * @brief Devolve a matriz do gestor de históricos.
 *
 * Devolve a matriz de reproduções do @p hm.
 * 
 * @param hm Gestor detentor da matriz a ser devolvida.
 * @return Matriz do gestor.
 */

int **get_matrix(History_Manager hm);

/**
 * @brief Liberta o espaço usado pela matriz guardada no gestor de históricos @p hm .
 * 
 * @param hm Gestor de históricos.
 */
void free_matrix (History_Manager hm);

/**
 * @brief Copia a informação de um histórico para outras variaveis.
 * 
 * Lê as informações de um histórico abrindo o ficheiro CSV de históricos e atualizando a sua posição
 * para a posição do histórico que queremos, e copia o valor dos campos desse histórico para outras variáveis.
 * 
 * @param history_pos posição do histórico no ficheiro, cuja informação queremos. 
 * @param listening_time apontador para onde vamos guardar a duração guardado pelo histórico.
 * @param music_id apontador para onde vamos guardar o id da música à qual o histórico faz referência.
 * @param month apontador para onde vamos guardar o mês no qual o histórico aconteceu.
 * @param day apontador para onde vamos guardar o dia no qual o histórico aconteceu.
 * @param hour apontador para onde vamos guardar a hora do dia na qual o histórico aconteceu.
 * @param hm Gestor que contém o caminho para o ficheiro de históricos.
*/
void get_history_info (int history_pos, int *listening_time, int *music_id, int *month, int *day, int *hour, History_Manager hm);

#endif
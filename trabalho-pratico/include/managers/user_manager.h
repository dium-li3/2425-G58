#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H

#include <glib.h>

#include "users.h"
#include "music_manager.h"

typedef struct user_manager *User_Manager;

/**
 * @brief Cria um gestor de usuários.
 *
 * Inicializa um gestor de usuários, alocando memória para a hash table
 * por ele contida.
 * 
 * @return Apontador para o gestor criado.
 */
User_Manager create_user_manager();

/**
 * @brief Verifica se um determinado usuário está presente na hash table do gestor.
 * 
 * Invoca a @b search_user_by_id, com os parâmetros que esta necessita, e verifica se o retorno da chamada
 * é nulo (neste caso simbolizando que o @p id dado não é chave de nenhuma entrada da tabela @p um).
 * 
 * @param id Identificador único do usuário a ser procurada na tabela.
 * @param um Gestor cuja tabela será revistada a procura do @p id.
 * @return Devolve @p TRUE se a tabela contiver o @p id; @b FALSE, caso contrário.
*/
gboolean user_exists (int id, User_Manager um);

/**
 * @brief Devolve o índice do usuário na matriz do gestor de históricos.
 * 
 * Dado um id @p id, busca na hash table do gestor de usuários o user com esse identificador, devolvendo o
 * seu índice na matriz do @b history_manager.
 * 
 * @param id Identificador único do usuário cujo índice será devolvido.
 * @param um Gestor onde o usuário será buscado.
 * 
 * @return Índice do usuário na matriz do @b history_manager.
*/
int search_user_index_by_id(int id, User_Manager um);

/**
 * @brief Guarda os usuários e contabiliza os likes de cada gênero.
 *
 * Armazena cada usuário na hash table do seu gestor, enquanto contabiliza
 * no array de gêneros do music_manager o número de curtidas que aquele 
 * usuário deu em cada gênero, com base na lista de músicas por ele curtidas.
 * Caso não seja possível abrir o ficheiro, retorna 1.
 * 
 * @param user_path path do ficheiro dos usuários.
 * @param user_manager gestor cuja hash table receberá os usuários.
 * @param music_manager gestor proprietário do array de gêneros.
 * @param interativo Indica se o programa está no modo interativo ou não.
 * 
 * @return Código de sucesso.
 */
int store_Users (char *user_path, User_Manager user_manager, Music_Manager music_manager, int interativo);

/**
 * @brief Devolve os ids de históricos relacionados a um utilizador num dado ano.
 * 
 * Retorna um array com todos os históricos relacionados ao user com id @p user_id, 
 * pertencentes ao ano @p year.
 * 
 * @param user_id Id do utilizador ao qual queremos os ids de históricos.
 * @param year Ano no qual os eventos relacionados a esses históricos aconteceram.
 * @param um Gestor que contém o user cujo id @p user_id é o que procuramos.
 * @return Array de ids de históricos referente a um dado ano e a um dado utilizador.
*/
const GArray *get_year_history_from_user_id(int user_id, int year, User_Manager um);

/**
 * @brief Adiciona o id de um histórico à informação do utilizador com um dado id.
 * 
 * Procura o utilizador segundo o id que recebeu, e insere no
 * array de ids de históricos por ano do utilizador o id do histórico que recebeu,
 * também tendo em consideração o ano do histórico.
 * 
 * @param user_id id do utilizador ao qual adicionar o id do histórico.
 * @param year ano do histórico.
 * @param history_id id do histórico, valor que vai ser guardado no utilizador.
*/
void add_year_history_id_to_user (User_Manager um, int user_id, int year, int history_id);

/**
 * @brief Recebe o id de um usuário e imprime as suas informações.
 *
 * Busca o usuário na hash table do gestor de usuários e imprime os dados daquele,
 * separados por um caractér guardado no Output. Caso o utilizador não exista, imprime apenas \n.
 * 
 * @param um Gestor que fornece a hash table de usuários.
 * @param id id do usuário a ser imprimido.
 * @param out Struct usado pelo output.
 */
void print_user_res_by_id (User_Manager um, int id, Output out);

/**
 * @brief Liberta o espaço usado pelo array de ids guardado pelo gestor de utilizadores @p um .
 * 
 * @param um Gestor de utilizadores.
 */
void free_users_ids (User_Manager um);

/**
 * @brief Libera a memória ocupada por um gestor de usuários.
 *
 * "Dá free" separadamente de cada um dos campos do gestor de usuários.
 * 
 * @param um Gestor cujos dados devem ser liberados.
 */
void free_user_manager(User_Manager um);

/**
 * @brief Devolve o número total de usuários.
 *
 * Acessa o campo @b total_users do gestor @p um, e devolve o valor nele presente.
 * 
 * @param um Gestor com o campo @b total_users.
 * @return Número total de usuários.
 */
int get_total_users(User_Manager um);

/**
 * @brief Devolve o id de todos os usuários.
 *
 * Retorna uma cópia do array com os identificadores de todos os usuários. 
 * 
 * @param um Gestor portador do array a devolver.
 * @return Array com todos os identificadores dos usuários.
 */
char **get_users_ids (User_Manager um);

#endif
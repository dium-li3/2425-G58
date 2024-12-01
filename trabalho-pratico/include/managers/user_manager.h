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
 * @brief Guarda os usuários e contabiliza os likes de cada gênero.
 *
 * Armazena cada usuário na hash table do seu gestor, enquanto contabiliza
 * no array de gêneros do music_manager o número de curtidas que aquele 
 * usuário deu em cada gênero, com base na lista de músicas por ele curtidas.
 * 
 * @param user_path path do ficheiro dos usuários.
 * @param user_manager gestor cuja hash table receberá os usuários.
 * @param music_manager gestor proprietário do array de gêneros.
 */

void store_Users (char *user_path, User_Manager user_manager, Music_Manager music_manager);

/**
 * @brief Recebe o id de um usuário e expõe as suas informações.
 *
 * Busca o usuário na hash table do gestor de usuários e expõe os dados daquele,
 * separados por um caractér guardado no Output.
 * 
 * @param um Gestor que fornece a hash table de usuários.
 * @param id id do usuário a ser expostos.
 * @param out Struct usado pelo output.
 */

void print_user_res_by_id (User_Manager um, int id, Output out);


/**
 * @brief Libera a memória ocupada por um gestor de usuários.
 *
 * "Dá free" separadamente de cada um dos campos do gestor de usuários.
 * 
 * @param um Gestor cujos dados devem ser liberados.
 */

void free_user_manager(User_Manager um);

#endif
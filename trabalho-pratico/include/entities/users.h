#ifndef USERS_H
#define USERS_H

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "output.h"
#include "users.h"

typedef struct user *User;

/**
 * @brief Inicializa um usuário, baseado nos tokens recebidos.
 *
 * Recebe tokens que representam um usuário e, caso esse usuário seja sintaticamente
 * válido, o armazena na memória.
 * 
 * @param tokens Array dos campos de informação do CSV.
 * @param index Índice do utilizador no array do seu gestor.
 * @return Apontador para o novo usuário e NULL caso este seja inválido.
 */

User create_user_from_tokens (char **tokens, int index);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param u Usuário cujo id será devolvido.
 * @return Id do usuário recebido.
 */

int get_user_id (User u);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param u Usuário cuja idade será devolvida.
 * @return Idade do usuário recebido.
 */

short get_user_age (User u);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param u Usuário cuja lista de músicas curtidas será devolvida.
 * @return GArray que contém as músicas curtidas por um usuário.
 */

const GArray *get_liked_musics(User u);

int get_user_index(User u);

/**
 * @brief Expoẽ as informações de um usuário.
 *
 * Revela as informações de um usuário, separadas por
 * um caractér guardado no @p out.
 * 
 * @param u Usuário que terá suas informações expostas.
 */

void print_user_res(User u, Output out);

/**
 * @brief Libera a memória ocupada por um registo de usuário.
 *
 * "Dá free" separadamente de cada um dos campos do usuário.
 * 
 * @param u Usuário cujos dados devem ser liberados.
 */

void free_user (User u);

#endif
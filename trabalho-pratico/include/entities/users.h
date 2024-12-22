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
 * @brief Função que devolve um array de ids de históricos de um dado ano de um utilizador @p u .
 * 
 * Tendo em consideração que os anos são contados de trás para a frente
 * e então a posição 0 do array de anos equivale ao ano 2024,
 * devolve o array de ids de históricos na posição (2024 - @p year )
 * do array de anos.
 * 
 * @param u Utilizador associado aos históricos cujos ids procuramos.
 * @param year Ano ao qual os históricos fazem referencia.
 * 
 * @result Array de ids de históricos referente a um dado ano e a um dado utilizador.
*/
const GArray *get_year_history(User u, int year);

/**
 * @brief Guarda o id de um histórico no user ao qual ele (o histórico) faz referência.
 * 
 * Adiciona o id de um histórico no array de históricos por ano de 
 * um User com base no ano em que o histórico aconteceu.
 * 
 * @param u Utilizador ao qual se vai adicionar o id do histórico.
 * @param year ano em que o histórico aconteceu, dá-nos o índice do histórico no array de anos.
 * @param history_id id do histórico a guardar no utilizador.
*/

void add_year_history_id(User u, int year, int history_id);

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
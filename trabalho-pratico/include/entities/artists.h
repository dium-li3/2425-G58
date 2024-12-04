#ifndef ARTISTS_H
#define ARTISTS_H

#include <glib.h>
#include <stdio.h>

#include "output.h"

typedef struct artist *Artist;

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param a Artista cujo id será devolvido.
 * @return Id do artista.
 */

int get_art_id(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param a Artista cujo país será devolvido.
 * @return País do artista .
 */

const char *get_art_country(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param a Artista cujo tempo de discografia será devolvido.
 * @return Tempo de discografia o país do artista recebido.
 */

int get_disc_duration(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param a Artista cujo nome será devolvido.
 * @return Devolve o nome do artista recebido.
 */

const char *get_art_name(Artist a);

/**
 * @brief Dada uma string com o tipo do artista, devolve o caracter que o representa.
 *
 * A princípio, o artista tem o tipo @b 'E' (que representa "error"). O seu campo @b type
 * será então comparado com as strings @b "individual" e @b "group", adquirindo o valor
 * @b 'I' ou @b 'G' consoante se verifique uma das respetivas igualdades.
 * 
 * @param a Artista cujo tipo será devolvido.
 * @return Devolve o tipo do artista recebido.
 */

char get_art_type_from_art(Artist a);


double get_art_recipe_stream (Artist a);

//Devolve um array com os ids dos artistas que fazem parte de um grupo.
const GArray *get_group_id_constituent (Artist a);

/**
 * @brief Função que "printa" um artista.
 *
 * Gera um array de strings onde serão armazenadas as informações do artista. Ademais, recebe
 * o tipo de caracter (correspondente ao parâmetro @p separador ) que a query exije que separe o output.
 * 
 * @param a Artista cujo país será devolvido.
 * @param separador Carácter que vai separar os outputs de uma mesma linha.
 * @param out Struct usado pelo output.
 */

/*
    Escreve num ficheiro de output um resumo
    do artista, útil para a query 1.
*/
void print_art_res(Artist a, Output out);

void print_art_info(Artist a, Output out);

/**
 * @brief Função usada de base para ordenar o array de artistas do @b artist_manager
 *
 * É um dos parâmetros que a função da glib @b g_array_sort() precisa para ordenar
 * o array de artistas do @b artist_manager segundo o tempo de discografia de cada um.
 * Quando a função for invocada, os parâmetros @p a e @p b serão os dois artistas a serem
 * comparados.
 * 
 * A função retorna menor que zero se @p b for menor que @p a, zero se for igual,
 * maior que zero se @p b for maior que @p a. É uma versão modificada do tipo
 * qsort()-style, para que o array fique ordenado em ordem decrescente, facilitando
 * a sua exposição posteriormente.
 * 
 * @param a um g_pointer para um artista constante.
 * @param b outro artista que será comparado com @p a.
 * @return Resultado da comparação.
 */

int compare_dur (gconstpointer a, gconstpointer b);

/**
 * @brief Cria um artista se todas as suas informações forem válidas.
 *
 * Faz a validação sintática do artista, verificando se a lista dos
 * artistas constituintes daquele id começa e termina com os caracteres
 * @b "[" e @b "]", respectivamente. Somado a isso, também faz a validação lógica
 * do artista, conferindo se o campo @b id_constituent de um artista 
 * individual não possui elementos.
 * 
 * Satisfeitas essas checagens, a função cria um artista.
 * 
 * @param tokens Strings segmentadas que contêm os dados necessários para criar
 * um artista.
 * @return Apontador para o artista criado.
 */

Artist create_artist_from_tokens(char **tokens);

/**
 * @brief Adiciona uma duração ao tempo de discografia de um artista individual.
 *
 * Este função será posteriormente usado pela função @b add_dur_artists, cujo
 * propósito é parecido, pois adiciona uma duração a todos os artistas que
 * produziram uma determinada música.
 * 
 * @param a Artista cuja discografia será incrementada.
 * @param duration Valor a incrementar na discografia.
 */

void add_disc_duration(Artist a, int duration);


/**
 * @brief Adiciona um tempo de reprodução a um artista.
 * 
 * Adiciona @p t na posição @p week do array de semanas de @p a .
 * 
 * @param a Artista cujo tempo de reprodução deve ser aumentado.
 * @param week Semana em que a reprodução ocorreu.
 * @param t Duração da reprodução em segundos.
 */
void add_list_time(Artist a, int week, int t);


/**
 * @brief Aumenta em 1 o número de albuns que um artista tem.
 *
 * @param a Artista que vai ter o seu número de albuns incrementado.
*/
void add_1_album (Artist a);

/*
 * @brief Adiciona a receita inteira que um artista recebe por música ouvida ao seu total ganho.
 *
 * @param a Artista ao qual se vai aumentar a receita total.
*/
void add_recipe (Artist a);

/**
 * @brief Adiciona uma percentagem de outra receita por stream a um Artista.
 * 
 * @param a Artista ao qual a receita total vai ser aumentada.
 * 
 * @param percentage valor a ser somado à receita total.
*/
void add_percentage_recipe (Artist a, double percentage);

/**
 * @brief Libera a memória ocupada por um registo de artista.
 *
 * "Dá free" separadamente de cada um dos campos do artista.
 * 
 * @param a Artista cujos dados devem ser liberados.
 */
void free_art(Artist a);

#endif
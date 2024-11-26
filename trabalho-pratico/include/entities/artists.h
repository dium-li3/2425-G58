#ifndef ARTISTS_H
#define ARTISTS_H

#include <glib.h>
#include <stdio.h>

#include "output.h"

/**
 * @brief Estrutura que representa um artista, com todas as informações que lhe são associadas.
 *
 * Mais especificamente, é um pointer para uma estrutura.
 */

typedef struct artist *Artist;

/**
 * @brief Inicializa um artista.
 *
 * Função que cria propriamente um artista, utilizada quando este é validado na
 * create_artist_from_tokens. O campo @c disc_duration será inicialmente posto 
 * a zero, sendo somente preenchido durante a leitura das músicas. Convencionou-se,
 * para o parâmetro @p type, um dos dois valores: @c 'I' para individual; @c 'G', para grupo.
 * 
 * @param id Id do artista.
 * @param name Nome do artista.
 * @param idc Array com os ids que constituem a entidade, caso seja do tipo @c "G".
 * @param coun Country do artista.
 * @param type Tipo do artista.
 * @return Devolve um artista inicializado.
 */

Artist create_art(int id, char *name, double rps, GArray *idc, char *coun, char type)

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Algoritmo necessário à componente de encapsulamento. 
 * 
 * @param a Artista cujo id será devolvido.
 * @return Id do artista.
 */

int get_art_id(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Algoritmo necessário à componente de encapsulamento. 
 * 
 * @param a Artista cujo país será devolvido.
 * @return País do artista recebido.
 */

const char *get_art_country(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Algoritmo necessário à componente de encapsulamento. 
 * 
 * @param a Artista cujo tempo de discografia será devolvido.
 * @return Tempo de discografia o país do artista recebido.
 */

int get_disc_duration(Artist a);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Algoritmo necessário à componente de encapsulamento. 
 * 
 * @param a Artista cujo nome será devolvido.
 * @return Devolve o nome do artista recebido.
 */

const char *get_art_name(Artist a);

/**
 * @brief Dada uma string com o tipo do artista, devolve o caracter que o representa.
 *
 * A princípio, o artista tem o tipo @c 'E' (que representa "error"). O seu campo @c type
 * será então comparado com as strings @c "individual" e @c "group", adquirindo o valor
 * @c 'I' ou @c 'G' consoante se verifique uma das respetivas igualdades.
 * 
 * @param a Artista cujo país será devolvido.
 * @return Devolve o país do artista recebido.
 */

char get_art_type(char *art_type);

/**
 * @brief Função que "printa" um artista.
 *
 * Gera um array de strings onde serão armazenadas as informações do artista. Ademais, recebe
 * o tipo de caracter (correspondente ao parâmetro @p separador ) que a query exije que separe o output.
 * 
 * @param a Artista cujo país será devolvido.
 * @param separador Carácter que vai separar os outputs de uma mesma linha.
 * @param out Struct usado pelo output.
 * @return Retorna @c void.
 */

void print_art_info(Artist a, char separador, Output out);

/**
 * @brief Função usada de base para ordenar o array de artistas do @b artist_manager
 *
 * É um dos parâmetros que a função da glib @b g_array_sort() precisa para ordenar
 * o array de artistas do @b artist_manager segundo o tempo de discografia de cada um.
 * Quando a função for invocada, os parâmetros @p a e @p b serão dois artistas a ser
 * comparados.
 * 
 * A função retorna menor que zero se @p b for menor que @p a, zero se for igual,
 * maior que zero se @p b for maior que @p a. É uma versão modificada do tipo
 * qsort()-style, para que o array fique ordenado em ordem decrescente, facilitando
 * a sua exposição posteriormente.
 * 
 * @param a um g_pointer para um artista constante.
 * @param b outro artista que será comparadio com @p a.
 * @return Resultado da comparação.
 */

int compare_dur (gconstpointer a, gconstpointer b);

/**
 * @brief Cria um artista se todas as suas informações forem válidas.
 *
 * Faz a validação sintática do artista, verificando se a lista dos
 * artistas constituintes daquele id começa e termina com os caracteres
 * @c "[" e @c "]", respectivamente. Somado a isso, também faz a validação lógica
 * do artista, conferindo se o campo @c id_constituent de um artista 
 * individual não possui elementos.
 * 
 * Satisfeitas essas checagens, a função cria um artista.
 * 
 * @param tokens Strings segmentadas que contêm os dados necessários para criar
 * um artista.
 * @return Resultado da comparação.
 */

Artist create_artist_from_tokens(char **tokens);

/**
 * @brief Adiciona uma duração ao tempo de discografia de um artista individual.
 *
 * Este algoritmo será posteriormente usado pela função @c add_dur_artists, cujo
 * propósito é parecido, pois adiciona uma duração a todos os artistas que
 * produziram uma determinada música.
 * 
 * @param a Artista cuja discografia será incrementada.
 * @param duration Valor a incrementar na discografia.
 * @return Retorna @c void.
 */

void add_disc_duration(Artist a, int duration);

/**
 * @brief Libera a memória ocupada por um registo de artista.
 *
 * "Dá free" separadamente de cada um dos campos do artista.
 * 
 * @param a Artistas cujos dados devem ser liberados.
 * @return Retorna @c void.
 */

void free_art(Artist a);

#endif
/**
 * @file artists.h
 * @author Grupo 58
 * @date Out 2024
 * @brief Header file da entidade Artist
 */
#ifndef ARTISTS_H
#define ARTISTS_H

#include <glib.h>
#include <stdio.h>
#include "output.h"

typedef struct artist *Artist;

/**
 * @brief Getter que devolve o id de um artista.
 *
 * Devolve o identificador de @p a.
 * 
 * @param a Artista cujo id será devolvido.
 * @return Id do artista.
 */
int get_art_id(Artist a);

/**
 * @brief Devolve o identificador de um artista.
 *
 * Getter que devolve o país de @p a.
 * 
 * @param a Artista cujo país será devolvido.
 * @return País do artista .
 */
const char *get_art_country(Artist a);

/**
 * @brief Getter que devolve a duração da discografia de um artista.
 *
 * Devolve o tempo total da discografia de @p a.
 * 
 * @param a Artista cujo tempo de discografia será devolvido.
 * @return Tempo de discografia o país do artista recebido.
 */
int get_disc_duration(Artist a);

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

/**
 * @brief Devolve o valor que um artista @p a ganha sempre que a sua música é ouvida.
 * 
 * Getter que retorna o campo @b recipe_per_stream do artista @p a. 
 * 
 * @param a Artista cuja receita por reprodução se deseja.
 * @return Receita por música ouvida.
 */
double get_art_recipe_stream (Artist a);

/**
 * @brief Devolve o tempo de reprodução de um artista numa determinada semana.
 * 
 * Retorna a duração total em qua o artista @p a foi reproduzido durante a semana @p week. 
 * 
 * @param a Artista.
 * @param week Semana.
 * @return -1 se @p week é fora do array do artista ou o tempo de reprodução caso @c week<a->weeks->len.
 */
int get_week_listening_time(Artist a, int week);

/**
 * @brief Devolve um array com os ids dos artistas que fazem parte do grupo de @p a.
 * 
 * Só devolve algo se o artista @p a for um grupo, caso contrário devolve NULL.
 * 
 * @param a Estrutura que representa um grupo de artistas.
 * @return Array de ids de artistas.
 */
const GArray *get_group_id_constituent (Artist a);

/**
 * @brief Função que "printa" um artista.
 *
 * Gera um array de strings onde serão armazenadas as informações do artista. Ademais, recebe
 * o tipo de caracter (correspondente ao parâmetro @p separador ) que a query exije que separe o
 * output.
 * 
 * @param a Artista cujo país será devolvido.
 * @param separador Carácter que vai separar os outputs de uma mesma linha.
 * @param out Struct usado pelo output.
 */
void print_art_res(Artist a, Output out);

/**
 * @brief Escreve num dado ficheiro de output @p out certas informações de um artista @p a.
 * 
 * Em base funciona da merma forma que a função print_art_res() mas dá print de um
 * grupo de valores diferentes do artista.
 * 
 * @param a Artista
 * @param out Estrutura que se mantém a par do ficheiro de output.
 */
void print_art_info(Artist a, Output out);

/**
 * @brief Função auxiliar para imprimir o artista com mais "top 10".
 * 
 * Escreve no ficheiro de output o id de @p a, o seu tipo e o número de vezes em que ficou entre os
 * dez melhores artistas, i.e., @p top_count. Tal código invocado pela função 
 * @b print_most_freq_top_art. 
 * 
 * @param a Artista cujas informações serão escritas no ficheiro @p out.
 * @param top_count Número de vezes em que ficou entre os dez melhores artistas.
 * @param out Ficheiro de resposta.
 */
void print_top_count_art(Artist a, int top_count, Output out);

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
 * @brief Compara o tempo de reprodução de @p a e @p b na semana dada por @p week .
 * 
 * Função para ser usada na organização das heaps.
 * 
 * @param a Artista
 * @param b Artista
 * @param week Semana em questão
 * @return TRUE -> a<b ou FALSE -> a>b
 */
int compare_listening_time(void *a, void *b, void *week);

/**
 * @brief Cria um artista se todas as suas informações forem válidas.
 *
 * Faz a validação sintática do artista, verificando se a lista dos
 * artistas constituintes daquele id começa e termina com os caracteres
 * @b "[" e @b "]", respectivamente. Somado a isso, também faz a validação lógica
 * do artista, conferindo se o campo @b id_constituent de um artista 
 * individual não possui elementos. Satisfeitas essas checagens, a função cria um
 * artista.
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
 * @brief Define o artista como presente no top 10 numa dada semana.
 * 
 * Coloca a posição @p week do array @b weeks de @p a igual a -1.
 * 
 * @param a Artista a marcar.
 * @param week Semana em que o artista esteve no top 10.
 */
void mark_top10(Artist a, int week);

/**
 * @brief Calcula o array de frequências acumuladas de top 10 de um artista.
 * 
 * O array @c weeks , que continha os tempos de reprodução e as semanas em que o artista figurou no top 10
 * marcadas com -1, passa a ter as frequências acumuladas das aparições do artista no top 10.
 * 
 * @param a Artista.
 */
void acc_freq_top10_1art(Artist a);

/**
 * @brief Devolve o número de "tops" total de um artista.
 * 
 * Retorna o número de vezes em que @p a ficou entre os dez melhores artistas.
 * 
 * @param a Artista cujas informações serão verificadas.
 * @return Número total de "tops".
 */
int get_art_max_top(Artist a);

/**
 * @brief Aumenta em 1 o número de albuns que um artista tem.
 * 
 * Incrementa em uma unidade o número de álbuns do artista @p a.
 *
 * @param a Artista que vai ter o seu número de albuns incrementado.
*/
void add_1_album (Artist a);

/**
 * @brief Adiciona a receita inteira que um artista recebe por música ouvida ao seu total ganho.
 * 
 * Soma o valor do campo @b recipe_per_stream ao campo @b total_recipe de @p a.
 *
 * @param a Artista ao qual se vai aumentar a receita total.
*/
void add_recipe (Artist a);

/**
 * @brief Adiciona uma percentagem de outra receita por stream a um Artista.
 * 
 * Soma o valor @p percentage ao campo @b total_recipe de @p a.
 * 
 * @param a Artista ao qual a receita total vai ser aumentada.
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
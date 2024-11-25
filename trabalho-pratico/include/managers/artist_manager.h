#ifndef ARTIST_H
#define ARTIST_H

#include <glib.h>

#include "artists.h"

typedef struct art_manager *Art_Manager;

typedef struct array_art *Arr_art;

/**
 * @brief Inicializa um gestor de artistas.
 *
 * Função que retorna um gestor de artistas, constituído por
 * uma hash table de artistas cuja chave são os seus ids, e
 * um array com os mesmos artistas, que será ordenado de forma
 * decrescente com base no tempo de discografia dos seus membros.
 * Esta função é chamada no @b master_manager.
 * 
 * @return Gestor de artista, com sua hash e seu array vazios.
 */

Art_Manager create_art_manager();

/**
 * @brief Insere um artista na tabela hash do gestor de artistas
 *
 * Recebe o artista e o seu gestor, de forma a obter o id do artista
 * com o @b get_art_id e inserir esse valor (como chave) e o artista
 * em si na tabela hash do gestor. 
 * 
 * @param a Artista que será inserido na tabela.
 * @param art_manager Gestor proprietário da tabela.
 * @return Retorna @c void.
 */

void insert_artists_by_id(Artist a, Art_Manager art_manager); 

/**
 * @brief Ordena o array de artistas do gestor pelo tempo de discografia
 * dos seus membros
 *
 * Após o armazenamento dos artistas e das músicas nos seus respetivos
 * gestores, essa função ordena o array dos artistas (cada um com o campo
 * @c disc_duration já preenchido) pelo seu tempo de discografia.
 * 
 * @param artist_manager Gestor cujo array será ordenado.
 * @return Retorna @c void.
 */

void order_duration (Art_Manager artist_manager);

/**
 * @brief Preenche um gestor com todos os artistas que estão num ficheiro
 * cujo path é dado.
 *
 * Função chamada pelo @b master_manager para guardar no gestor de artistas
 * - especificamente no seu array e na sua hash - todos os artistas
 * presentes em um ficheiro dado o caminho deste.
 * 
 * Ademais, o algoritmo também expõe algum artista que esteja incorreto.
 * 
 * @param art_path caminho do ficheiro que contém os artistas.
 * @param artist_manager Gestor que será preenchido com os artistas do ficheiro.
 * @return Retorna @c void.
 */

void store_Artists (char *art_path, Art_Manager artists_manager);

/**
 * @brief Confere se todos os artistas de um array estão presentes no gestor.
 *
 * Função que devolve @c TRUE se todos os artistas de um array se encontram
 * no array do gestor de artistas.
 * 
 * Esta função é utilizada durante o armazenamento das músicas, assegurando
 * que todos os artistas que produziram uma determinada música estão registados
 * no gestor, para que a duração dessa música seja adicionada ao campo 
 * @c disc_duration de cada um deles.
 * 
 * Ademais, o algoritmo também expõe algum artista que esteja incorreto.
 * 
 * @param artists Array dos artistas produtores de dada música.
 * @param artist_manager Gestor usado de parâmetro.
 * @return @c TRUE se todos os artistas se encontram no gestor; @c FALSE, do contrário.
 */

gboolean all_artists_exist (const GArray *artists, Art_Manager am);

/**
 * @brief Adiciona um intervalo de tempo à discografia de um grupo de artistas.
 *
 * Após a @b all_artists_exist confirmar a existência de todos os artistas que
 * compuseram uma determinada música, será a @b add_dur_artists a responsável por
 * adicionar, em cada um dos artistas, a duração dessa faixa aos seus campos
 * @c disc_duration
 * 
 * @param music_artists Array dos artistas produtores de dada música.
 * @param duration Valor que será adicionado ao tempo de discografia de cada membro
 * do array.
 * @param am Gestor de artistas.
 * @return Retorna @c void.
 */

void add_dur_artists (const GArray *music_artists , int duration, Art_Manager am);

/**
 * @brief Expõe os N primeiros artistas do array do gestor, dado o seu country.
 *
 * Logo depois dos artistas estarem devidamente guardados no seu gestor, tendo o
 * array deste já sido devidamente ordenado em função do tempo de discografia dos
 * seus integrantes, a função percorre todo esse array e revela os seus @p N primeiros
 * membros cujo país coincida com o @p country dado como parâmetro.
 * 
 * @param am Gestor que fornece o array de artistas.
 * @param country país usado como filtro na busca.
 * @param N Números de artistas a serem expostos.
 * @param separador Carácter que vai separar os outputs de uma mesma linha.
 * @param out Struct usado pelo output.
 * do array. 
 * @return Retorna @c void.
 */

void print_N_country_art_info (Art_Manager am, char *country, int N, char separador, Output out);

/**
 * @brief Expõe os N primeiros artistas do array do gestor.
 *
 * "Dá print" dos @p N primeiros artistas do array já ordenado do gestor.
 * 
 * @param am Gestor que fornece o array de artistas.
 * @param N Números de artistas a serem expostos.
 * @param separador Carácter que vai separar os outputs de uma mesma linha.
 * @param out Struct usado pelo output.
 * do array. 
 * @return Retorna @c void.
 */

void print_N_art_info (Art_Manager am, int N, char separador, Output out);

/**
 * @brief Devolve o tamanho do array de artistas do gestor.
 *
 * Dado um gestor de artistas, devolve o tamanho do seu array de artistas.
 * 
 * @param am Gestor proprietário do array de artistas.
 * @return O tamanho do array de artistas do gestor.
 */

int length_arr_disc (Art_Manager am);

/**
 * @brief Libera a memória ocupada por um gestor de artistas.
 *
 * "Dá free" separadamente de cada um dos campos do gestor de artistas.
 * 
 * @param art_manager Gestor cujos dados devem ser liberados.
 * @return Retorna @c void.
 */

void free_art_manager(Art_Manager art_manager);


/*
Arr_art create_arr_art(Art_Manager art_manager, char *country);

void add_arr_art(gpointer *key, gpointer *value, gpointer *art_data);

int cmp_art (gconstpointer a, gconstpointer b);

void fill_arr_art(Art_Manager art_manager, char *country);

void free_arr_art(Arr_art arr_art);

void responde_querie2 (FILE *fp_queries, Art_Manager *art_manager);
*/
#endif
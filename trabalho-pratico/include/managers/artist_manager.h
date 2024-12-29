#ifndef ARTIST_MANAGER_H
#define ARTIST_MANAGER_H

#include <glib.h>
#include "artists.h"


#define TOP 10

typedef struct art_manager *Art_Manager;

/**
 * @brief Inicializa um gestor de artistas.
 *
 * Função que retorna um gestor de artistas, constituído por
 * uma hash table de artistas cuja chave são os seus ids, e
 * um array com os mesmos artistas, que será ordenado de forma
 * decrescente com base no tempo de discografia dos seus membros.
 * Esta função é chamada no @b master_manager.
 * 
 * @return Apontador para o novo gestor de artista, com sua hash e seu array vazios.
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
 */
void insert_artists_by_id(Artist a, Art_Manager art_manager); 

/**
 * @brief Ordena o array de artistas do gestor pelo tempo de discografia
 * dos seus membros.
 *
 * Após o armazenamento dos artistas e das músicas nos seus respetivos
 * gestores, essa função ordena o array dos artistas (cada um com o campo
 * @b disc_duration já preenchido) pelo seu tempo de discografia.
 * 
 * @param artist_manager Gestor cujo array será ordenado.
 */
void order_duration (Art_Manager artist_manager);

/**
 * @brief Incrementa a receita total de um dado conjunto de artistas.
 * 
 * No caso de artistas que são grupos, incrementa-se também a receita total
 * dos seus membros por uma fração igual a 1 / nº membros.
 * 
 * @param artists GArray contendo os artistas os quais terão sua receita incrementada.
 * @param am Gestor que por sua vez armazena os membros desse array.
 */
void add_recipe_artists (const GArray *artists, Art_Manager am);

/**
 * @brief Preenche um gestor com todos os artistas que estão num ficheiro
 * cujo path é dado.
 *
 * Função chamada pelo @b master_manager para guardar no gestor de artistas
 * - especificamente no seu array e na sua hash table - todos os artistas
 * presentes em um ficheiro dado o caminho deste. Ademais, também imprime
 * algum artista que esteja incorreto.
 * 
 * @param art_path caminho do ficheiro que contém os artistas.
 * @param artist_manager Gestor que será preenchido com os artistas do ficheiro.
 */
void store_Artists (char *art_path, Art_Manager artists_manager);

/**
 * @brief Confere se todos os artistas de um array estão presentes no gestor.
 *
 * Função que devolve @b TRUE se todos os artistas de um array se encontram
 * no array do gestor de artistas.
 * 
 * Esta função é utilizada durante o armazenamento das músicas, assegurando
 * que todos os artistas que produziram uma determinada música estão registados
 * no gestor, para que a duração dessa música seja adicionada ao campo 
 * @b disc_duration de cada um deles.
 * 
 * @param artists Array dos artistas produtores de dada música.
 * @param artist_manager Gestor usado de parâmetro.
 * @return @b TRUE se todos os artistas se encontram no gestor; @b FALSE, do contrário.
 */
gboolean all_artists_exist (const GArray *artists, Art_Manager am);

/**
 * @brief Adiciona uma duração à discografia de um grupo de artistas.
 *
 * Após a @b all_artists_exist confirmar a existência de todos os artistas que
 * compuseram uma determinada música, será a @b add_disc_duration a responsável por
 * adicionar, em cada um dos artistas, a duração dessa faixa aos seus campos
 * @b disc_duration
 * 
 * @param music_artists Array dos artistas produtores de dada música.
 * @param duration Valor que será adicionado ao tempo de discografia de cada membro
 * do array.
 * @param am Gestor de artistas.
 */
void add_disc_dur_artists (const GArray *music_artists , int duration, Art_Manager am);

/**
 * @brief Adiciona um tempo de reprodução a um array de artistas.
 * 
 * Adiciona @p time a cada artista de @p artists na posição @p week .
 * 
 * @param artists Array de IDs de artistas cujo tempo de reprodução deve ser aumentado.
 * @param week Semana em que a reprodução ocorreu.
 * @param time Duração da reprodução em segundos.
 * @param am Contém a hash table de artistas por IDs.
 */
void add_listening_time_artists(const GArray *artists, int week, int time, Art_Manager am);

/**
 * @brief Preenche o campo @b max_week de @p am.
 * 
 * Define a semana mais distante do 9/9/2024 onde ocorreram históricos.
 * 
 * @param am Artist manager.
 * @param mw Semana mais distante.
 */
void set_max_week(Art_Manager am, int mw);

/**
 * @brief Devolve a semana máxima em que houve históricos.
 * 
 * Retorna o campo @b max_week do gestor de artistas.
 * 
 * @param am Contém a semana máxima.
 * @return Semana máxima.
 */
int get_max_week(Art_Manager am);

/**
 * @brief Calcula os top 10 de todas as semanas.
 * 
 * Os artistas que compõem o top 10 de uma semana são marcados com -1 na posição correspondente
 * do seu array @c weeks .
 * 
 * @param am Contém o array com todos os artistas.
 */
void calc_top10s(Art_Manager am);

/**
 * @brief Calcula as frequências acumuladas de top 10 de todos os artistas.
 * 
 * Aplica a funcão @b acc_freq_top10_1art em cada membro do array de artistas do gestor @p am.
 * 
 * @param am Contém o array com todos os artistas.
 */
void acc_freq_top10s(Art_Manager am);

/**
 * @brief Encontra o artista com mais tops 10 num dado intervalo de semanas.
 * 
 * Devolve o identificador único do artista que apareceu mais vezes no top 10 de um dado intervalo.
 * 
 * @param begin_week Começo do intervalo.
 * @param end_week Fim do intervalo.
 * @param am Contém o array com todos os artistas.
 * @param top_count Apontador para guardar o número de tops do artista em questão.
 * @return ID do artista com mais tops.
 */
int find_most_freq_top_art(int begin_week, int end_week, Art_Manager am, int *top_count);

/**
 * @brief Incrementa o número de álbuns de todos os artistas cujos IDs estão no array.
 * 
 * Invoca a função @b add_1_album em cada elemento do array @p album_artists, o que resulta no aumento
 * em uma unidade do número de álbuns de cada integrante desse vetor.
 * 
 * @param album_artists Array com os IDs dos artistas cujo número de álbuns deve ser incrementado.
 * @param am Contém a hash table de artistas por IDs.
 */
void add_1_album_to_artists (const GArray *album_artists, Art_Manager am);

/**
 * @brief Imprime o resumo de um artista com um dado ID.
 * 
 * Expõe as informações do artista com o id @p id.
 * 
 * @param am Contém a hash table de artistas por IDs
 * @param id ID do artista a imprimir.
 * @param out Ficheiro de output.
 */
void print_art_res_by_id (Art_Manager am, int id, Output out);

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
 * @param out Struct usado pelo output.
 * do array. 
 */
void print_N_country_art_info (Art_Manager am, char *country, int N, Output out);

/**
 * @brief Expõe os N primeiros artistas do array do gestor.
 *
 * "Dá print" dos @p N primeiros artistas do array já ordenado do gestor.
 * 
 * @param am Gestor que fornece o array de artistas.
 * @param N Números de artistas a serem expostos.
 * @param out Struct usado pelo output.
 * do array. 
 */
void print_N_art_info (Art_Manager am, int N, Output out);

/**
 * @brief Imprime para um ficheiro de resposta a informação do artista com mais top 10s.
 * 
 * Expõe no ficheiro de respostas o artista que apareceu mais vezes no top 10 artistas,
 * dado o seu identificador @p art_id.
 * 
 * @param art_id Artista com mais tops.
 * @param top_count Nº de tops de @p art_id .
 * @param am Contém a tabela de hash com todos os artistas.
 * @param out Ficheiro de resposta.
 */
void print_most_freq_top_art(int art_id, int top_count, Art_Manager am, Output out);

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
 */
void free_art_manager(Art_Manager art_manager);

#endif

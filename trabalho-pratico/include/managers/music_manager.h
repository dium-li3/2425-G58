#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <stdio.h>
#include "musics.h"
#include "artist_manager.h"
#include "album_manager.h"
#include "output.h"

typedef struct music_manager *Music_Manager;


/**
 * @brief Cria um Music Manager.
 * 
 * Aloca memória para um Music Manager (hash table de músicas e array de géneros, com as respetivas funções
 * para libertar as entidades) com tudo inicializado a 0/NULL.
 * 
 * @return Apontador para o Music Manager criado.
 */
Music_Manager create_music_manager();


/**
 * @brief Adidiona likes ao género de cada música.
 *
 * Itera sobre o array de músicas e, para cada uma, adiciona um like ao seu género,
 * segundo a idade do utilizador.
 *
 * @param musics Array das liked musics de um utilizador.
 *
 * @param mm Ccontém o array de Genres.
 *
 * @param age Idade do utilizador. 
 */
void add_like_genres (const GArray *musics, Music_Manager mm, short age);


/**
 * @brief Transforma todos os arrays de likes dos géneros do Music Manager em arrays de likes acumulados.
 *
 * Percorre o array de Genres do Music Manager e, para cada Genre, transforma o seu array de likes num array
 * de likes acumulados, de forma a que por exemplo, no índice 20 esteja o total de likes do género
 * de utilizadores com 20 ou menos anos.
 *
 * @param mm Contém o array de Genres.
 */
void gen_arr_freq_acum(Music_Manager mm);


/**
 * 
 */
void sort_gen(Music_Manager mm, int min_age, int max_age);


/**
 * @brief Insere uma múscia na hash table segundo o seu id.
 *
 * A chave (o int correspondente ao id) é armazenada na forma de int, recorrendo à macro GINT_TO_POINTER(),
 * para evitar fazer uma cópia do id e armazenar o endereço dessa cópia, poupando assim tempo e memória.
 *
 * @param m Música a inserir.
 *
 * @param music_manager Contém a hash table de músicas.
 */
void insert_music_by_id(Music m, Music_Manager music_manager);


/**
 * @brief Verifica se todas as músicas de um array existem.
 *
 * Verifica se todas as músicas de um array constam no Music Manager, de forma a validar logicamente um utilizador.
 *
 * @param musics Array de músicas a verificar.
 *
 * @param mm Contém a hash table de músicas.
 *
 * @return Booleano (TRUE->todas as músicas existem e FALSE->pelo menos uma música não existe).
 */
gboolean all_musics_exist (const GArray *musics, Music_Manager mm);


const GArray *get_music_artists_from_id (int id, Music_Manager mm);

//GArray *get_music_artists_copy_from_id (int id, Music_Manager mm);


/**
 * @brief Armazena todas as músicas e géneros.
 *
 * Armazena a informação necessária das músicas de um ficheiro dado pelo caminho @p music_path
 * numa hashtable de músicas e preenche o array de Genres sem nenhum repetido.
 * Antes de efetivamente armazenar uma música, é feita a sua validação lógica (verificar se todos os seus
 * artistas e o seu álbum existem).
 * Também adiciona a duração de cada música aos seus artistas se a música for válida e então guardada.
 * Escreve todas as linhas de músicas inválidas no ficheiro de erros correspondente.
 * 
 * @param music_path Caminho para o ficheiro CSV das músicas.
 *
 * @param mm Music Manager que armazena as músicas e os géneros.
 *
 * @param am Contém a hash table dos artistas.
 * 
 * @param alm Contém a hash table dos álbuns.
 */
void store_Musics(char *music_path, Music_Manager mm, Art_Manager am, Album_Manager alm);


/**
 * @brief Imprime os géneros para um ficheiro de resposta (query 3).
 *
 * Imprime apenas um '\n' caso não imprima nenhum género.
 *
 * @param mm Contém o array de géneros.
 *
 * @param out Contém o ficheiro de resultado e o separador (';' ou '=', consoante o tipo da query (S)).
 */
void print_all_genres_info(Music_Manager mm, Output out);


/**
 * @brief Liberta um Music Manager.
 *
 * Liberta a memória alocada para a hash table das músicas e para o array dos géneros, usando as funções
 * que foram passadas na criação do Music Manager.
 *
 * @param mm Music Manager a ser libertado.
 */
void free_music_manager(Music_Manager mm);

Music search_music_by_id(int id, Music_Manager music_manager);

int search_gen_index_by_id(int music_id,Music_Manager mm);

int get_total_genres(Music_Manager mm);

char **get_genre_names(Music_Manager mm);



#endif
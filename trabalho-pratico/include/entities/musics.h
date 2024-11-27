#ifndef MUSICS_H
#define MUSICS_H

#include <glib.h>

typedef struct music *Music;


/**
 * @brief Cria uma Music a partir dos tokens de informação.
 *
 * Cria uma estrutura music com os dados necessários de uma linha do CSV, fazendo a validação sintática dos
 * campos desta última.
 *
 * @param tokens Array com as strings correspondentes a cada campo.
 *
 * @return Apontador para uma música (NULL caso a música seja inválida).
 */
Music create_music_from_tokens (char **tokens);


/**
 * @brief Devolve o id de uma música.
 *
 * @param m Música cujo id se quer obter.
 *
 * @return Id da música.
 */
int get_music_id(Music m);


/**
 * @brief Devolve o género de uma música.
 *
 * Em vez de ser feita uma cópia da string, é retornado o apontador com o modificador const, de maneira a não
 * fazer uma cópia.
 *
 * @param m Música cujo género se quer obter.
 *
 * @return Apontador const para o ínicio da string.
 */
const char* get_genre(Music m);


/**
 * @brief Devolve a duração de uma música em segundos.
 *
 * @param m Música cuja duração se quer obter.
 *
 * @return Duração da música.
 */
int get_music_duration(Music m);


/**
 * @brief Devolve um apontador para o array de artistas de uma música.
 *
 * O apontador devolvido é o mesmo presente na estrutura da música e por isso possui o modificador const,
 * para que o seu conteúdo não seja modificado.
 *
 * @param m Música cujos artistas se querem obter.
 *
 * @return Apontador const para o array.
 */
const GArray *get_music_artists (Music m);


/**
 * @brief Liberta a memória alocada para uma música.
 *
 * Liberta o género e, caso exista, o array dos artistas (usando para isso a função que foi passada
 * (g_array_set_clear_func) ao GArray).
 *
 * @param m Música a ser libertada.
 */
void free_music(Music m);


/**
 * @brief Valida uma duração.
 *
 * Verifica se a duração segue o formato hh:mm:ss, com mm e ss <= 59 e hh <= 99.
 *
 * @param durarion String com a duração.
 *
 * @return Booleano (1->válida e 0->inválida).
 */
int valid_duration (char *duration);

#endif
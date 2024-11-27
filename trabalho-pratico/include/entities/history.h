#ifndef HISTORY_H
#define HISTORY_H

#include <glib.h>

typedef struct history *History;

/**
 * @brief Dados tokens, processa a informação contida neles
 * e coloca-a num struct history.
 * Também devolve o ano do histórico através do *year.
 * 
 * @param tokens Array de strings a processar e transformar na criação de um struct history.
 * @param year inteiro que irá guardar o ano do histórico.
 * 
 * @result History criado com os dados dos tokens.
*/
History create_history_from_tokens (char **tokens, int *year);

/**
 * @brief Devolve o id da música à qual o histórico faz referencia.
 * 
 * @param h histórico que contém o id da música que queremos.
*/
int get_history_music (History h);

/**
 * @brief Atualiza os ids de artistas guardados num histórico.
 * 
 * @param h Histórico a ser atualizado.
 * @param artist_ids array que contém os artistas relevantes a este histórico.
*/
void set_artist_ids (History h, GArray *artist_ids);

/**
 * @brief Liberta o espaço usado por um histórico.
 * 
 * @param h Pointer para o struct history a ser libertado.
*/
void free_history (History h);

#endif
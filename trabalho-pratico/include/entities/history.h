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
 * @return History criado com os dados dos tokens.
*/
History create_history_from_tokens (char **tokens, long file_pos, int *hist_id, int *user_id, int *music_id, int *year, int *month, int *day, int *dur);

/**
 * @brief Devolve a posição de um dado histórico @p h no ficheiro CSV de históricos.
 * 
 * @param h Histórico
 * 
 * @return Posição das informações deste histórico no CSV.
 */
long get_history_pos (History h);

/**
 * @brief Lê a timestamp escrita numa string e guarda os elementos 
 * da data em variáveis dadas e devolve a hora que a data.
 * 
 * Lê uma timestamp que está escrita como yyyy/mm/dd hh:mm:ss
 * e guarda os 3 elementos da data e devolve o primeiro elemento
 * da duração.
 * 
 * @param str timestamp que contém uma data seguida de uma duração
 * @param year Variável que irá conter o primeiro elemento da data.
 * @param month Variável que irá conter o segundo elemento da data.
 * @param day Variável que irá conter o último elemento da data.
 * 
 * @return Primeiro elemento da duração.
 */
int read_timestamp_elements (char *str, int *year, int *month, int *day);

/**
 * @brief Liberta o espaço usado por um histórico @p h .
 * 
 * @param h Pointer para o struct history a ser libertado.
*/
void free_history (History h);

#endif
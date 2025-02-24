/**
 * @file history.h
 * @author Grupo 58
 * @date Out 2024
 * @brief Header file da entidade History
 */
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
History create_history_from_tokens (char **tokens, int *year);

/**
 * @brief Devolve o dia do mês ao qual um histórico @p h se refere.
 * 
 * @param h histórico que contém a informação.
 * @return Dia do mês em que o histórico foi gravado.
*/
int get_history_day (History h);

/**
 * @brief Devolve o mês ao qual um histórico se refere.
 * 
 * @param h histórico que contém a informação.
 * @return Mês em que o histórico foi gravado.
*/
int get_history_month (History h);

/**
 * @brief Devolve a hora à qual um histórico se refere.
 * 
 * @param h histórico que contém a informação.
 * 
 * @return Hora do dia à qual o histórico se refere.
*/
int get_history_hour (History h);

/**
 * @brief Devolve quanto um user ouviu uma música segundo um histórico, em segundos.
 * 
 * @param h histórico que contém a informação.
 * 
 * @return Duração, em segundos, da audição de uma música por um utilizador.
*/
int get_history_listening_time (History h);

/**
 * @brief Devolve o id da música à qual o histórico faz referência.
 * 
 * @param h histórico que contém a informação.
 * @return id da música à qual o histórico faz referência.
*/
int get_history_music (History h);

/**
 * @brief Devolve a duração em segundos de um histórico.
 * 
 * @param h Histórico cuja duração se pretende obter.
 * @return Duração em segundos.
 */
int get_history_dur(History h);

/**
 * @brief Liberta o espaço usado por um histórico @p h .
 * 
 * @param h Pointer para o struct history a ser libertado.
*/
void free_history (History h);

#endif
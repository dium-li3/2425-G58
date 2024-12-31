#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <glib.h>

#include "history.h"
#include "user_manager.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "history_manager.h"

typedef struct history_manager *History_Manager;

History_Manager create_history_manager ();

void free_history_manager (History_Manager am);

void insert_history_by_id (History al, int id, History_Manager history_manager);

History search_history_by_id(int id, History_Manager history_manager);

int store_History (char *history_path, History_Manager history_man, Art_Manager am, Music_Manager mm, User_Manager um, int interativo);

int **get_matrix(History_Manager hm);

/**
 * @brief Copia a informação guardada num histórico para outras variaveis.
 * 
 * @param history_id id do histórico cuja informação queremos.
 * @param listening_time apontador para onde vamos guardar a duração guardado pelo histórico.
 * @param music_id apontador para onde vamos guardar o id da música à qual o histórico faz referência. 
 * @param month apontador para onde vamos guardar o mês no qual o histórico aconteceu.
 * @param day apontador para onde vamos guardar o dia no qual o histórico aconteceu.
 * @param hour apontador para onde vamos guardar a hora do dia na qual o histórico aconteceu.
 * @param hm Gestor que contém todos os históricos.
*/
void get_history_info (int history_id, int *listening_time, int *music_id, int *month, int *day, int *hour, History_Manager hm);

#endif
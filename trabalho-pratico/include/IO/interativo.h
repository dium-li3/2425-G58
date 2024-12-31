#ifndef INTERATIVO_H
#define INTERATIVO_H

#include "master_manager.h"

//Pares de cores, iniciais são a cor de texto e de fundo. Definidos na print_init()
#define WBa_PAIR 1
#define RBa_PAIR 2
#define GBa_PAIR 3


/**
 * @brief Inicializa o ncurses e imprime o ecrã inicial do programa.
 * 
 * Além de inicializar a janela, também define os pares e ajusta as cores que serão usadas futuramente.
 */
void print_init();


/**
 * @brief Armazena os dados dos CSVs para o programa interativo.
 * 
 * Executa a store_Entities até o utilizador inserir um caminho válido.
 * 
 * @param mm Apontador para o Master Manager que foi declarado na main
 * @param interativo Booleano.
 * 
 * @return Código de sucesso: - 0->sucesso
 *                            - 1->armazenamento não correu bem (este caso não deverá acontecer)
 *                            - 2->o utilizador saiu do programa
 */
int store_data_until_correct(Master_Manager *mm, int interativo);

#endif
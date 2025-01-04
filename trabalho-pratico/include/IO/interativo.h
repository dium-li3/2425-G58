#ifndef INTERATIVO_H
#define INTERATIVO_H

#include "master_manager.h"

//Pares de cores, iniciais são a cor de texto e de fundo. Definidos na print_init()
#define WBa_PAIR 1
#define RBa_PAIR 2
#define GBa_PAIR 3


//Teclas
#define ENTER 10
#define ESC 27

#define BUFSIZ_QUERYLINE 110

#define TEMP_FILE_PATH "query_commands_temp.txt"


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
 * Termina o programa caso o utilizador escreva 'sair'.
 * 
 * @param mm Apontador para o Master Manager que foi declarado na main
 * @param interativo Booleano.
 * 
 * @return Código de sucesso: - 0->sucesso
 *                            - 1->armazenamento não correu bem (este caso não deverá acontecer)
 *                            - 2->o utilizador saiu do programa
 */
int store_data_until_correct(Master_Manager *mm, int interativo);


/**
 * @brief Lê o input do utilizador relativo às queries até este decidir fechar o programa.
 * 
 * Lê o tipo de query, modificador de separador e argumentos de um comando e responde no terminal ou num ficheiro
 * de resposta, conforme o utilizador preferir. Também existe a opção de usar o recomendador dos professores ou o nosso.
 * A leitura é feita de forma repetitiva, até o utilizador decidir sair.
 * 
 * @param mm Master Manager.
 * @param interativo Booleano.
 * 
 * @return Código de sucesso: - 0->sucesso
 *                            - 1->resposta não correu bem (este caso não deverá acontecer)
 *                            - 2->o utilizador saiu do programa 
 */
int input_queries(Master_Manager mm, int interativo);

#endif
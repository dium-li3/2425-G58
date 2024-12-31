#include <ncurses.h>

#include "interativo.h"
#include "master_manager.h"


void print_init() {
    initscr();
    start_color();

    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_GREEN, 0, 1000, 0);

    init_pair(WBa_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(RBa_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GBa_PAIR, COLOR_GREEN, COLOR_BLACK);

    char text[] = "=====PROGRAMA=INTERATIVO=====";
    int begin_x = (getmaxx(stdscr) / 2) - (strlen(text) / 2); //centrar o texto

    attron(A_BOLD | COLOR_PAIR(WBa_PAIR));
    mvaddstr(0, begin_x, text);
    attroff(A_BOLD);
}


/*
    Imprime o ecrã de saída.
*/
int print_sair() {
    move(3, 0);
    clrtobot();
    attron(A_BLINK); curs_set(0);
    addstr("[Pressione qualquer tecla para fechar a janela]");
    return 2;
}

/*
    Imprime a mensagem de erro no carregamento dos dados.
*/
void print_store_error() {
    int key_buffer = -1;

    attron(COLOR_PAIR(RBa_PAIR));
    addstr("\nErro no carregamento dos dados.\n\n");
    attroff(COLOR_PAIR(RBa_PAIR));

    attron(A_BLINK); curs_set(0); noecho();
    addstr("[Prima ENTER para inserir um novo caminho]");
    do key_buffer = getch(); while (key_buffer != 10);
    attroff(A_BLINK); curs_set(1); echo();
}

/*
    Limpa o ecrã e lê o input do utilizador.
*/
void get_input_path(char *buffer) {
    move(3, 0);
    clrtobot();
    attron(A_BOLD);
    addstr("Introduza o caminho para os ficheiros de dados (digite 'sair' para fechar o modo interativo):\n\n");
    attroff(A_BOLD);
    getnstr(buffer, BUFSIZ-1);    //BUFSIZ-1 para poder guardar o \0
    addstr("\nA carregar dados...\n\n");
    refresh();
}


int store_data_until_correct(Master_Manager *mm, int interativo) {
    char input_buffer[BUFSIZ];
    
    get_input_path(input_buffer);
    *mm = create_master_manager();
    
    if(strcmp(input_buffer, "sair") == 0) return print_sair();
    
    char **entity_paths = pathEntities(input_buffer);
    int store = store_Entities(entity_paths, *mm, interativo);

    while(store != 0) {
        free_master_manager(*mm);
        freeEntityPaths(entity_paths);

        print_store_error();
        
        get_input_path(input_buffer);
        *mm = create_master_manager();

        if(strcmp(input_buffer, "sair") == 0) return print_sair();
        
        entity_paths = pathEntities(input_buffer);
        store = store_Entities(entity_paths, *mm, interativo);
    }
    
    freeEntityPaths(entity_paths);
    attron(COLOR_PAIR(GBa_PAIR));
    addstr("Dados carregados com sucesso.\n");
    attroff(COLOR_PAIR(GBa_PAIR));
    refresh();

    return store;
}
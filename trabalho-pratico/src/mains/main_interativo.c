#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "master_manager.h"
#include "parser.h"
#include "queries.h"



int print_sair() {
    move(4, 0);
    clrtobot();
    addstr("[Pressione qualquer tecla para fechar a janela]");
    return 2;
}


void get_input_path(char *buffer) {
    move(4, 0);
    clrtobot();
    addstr("Introduza o caminho para os ficheiros de dados: ");
    getnstr(buffer, BUFSIZ-1);    //BUFSIZ-1 para poder guardar o \0
    addstr("\nA carregar dados...\n\n");
    refresh();
}


/*
    Executa a store_Entities até o utilizador inserir um caminho válido.
*/
int store_data_until_correct(char *input_buffer, Master_Manager *mm, int interativo) {
    int key_buffer = -1;
    
    get_input_path(input_buffer);
    *mm = create_master_manager();
    
    if(strcmp(input_buffer, "sair") == 0) return print_sair();
    
    char **entity_paths = pathEntities(input_buffer);
    int store = store_Entities(entity_paths, *mm, interativo);

    while(store != 0) {
        free_master_manager(*mm);
        freeEntityPaths(entity_paths);

        addstr("\nErro no carregamento dos dados.\n[Prima ENTER para inserir um novo caminho]");
        noecho();
        do key_buffer = getch(); while (key_buffer != 10);
        echo();
        
        get_input_path(input_buffer);
        *mm = create_master_manager();

        if(strcmp(input_buffer, "sair") == 0) return print_sair();
        
        entity_paths = pathEntities(input_buffer);
        store = store_Entities(entity_paths, *mm, interativo);
    }
    
    freeEntityPaths(entity_paths);
    addstr("Dados carregados com sucesso.\n");
    refresh();

    return store;
}


int trabalho_interativo(Query_stats qs, int interativo){
    int store = 0;
    char input_buffer[BUFSIZ];
    
    initscr();
    attron(A_BOLD);
    addstr("\t\t\t\t\t\t    =====PROGRAMA=INTERATIVO=====\n\t\t\t\t\t    Digite 'sair' para fechar o modo interativo.\n\n");
    attroff(A_BOLD);

    Master_Manager master_manager = NULL;
    store = store_data_until_correct(input_buffer, &master_manager, interativo);

    //queries

    free_master_manager(master_manager);

    getch();
    endwin();
    return store;
}

int main (int argc, char **argv){
    if (argc != 1){
        printf("Argumentos a mais.\n");
        return 1;
    }
    Query_stats qs = NULL;
    int interativo = 1;

    int r = trabalho_interativo(qs, interativo);
    
    return r;
}

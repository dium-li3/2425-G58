#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "master_manager.h"
#include "parser.h"
#include "queries.h"
#include "interativo.h"



int trabalho_interativo(Query_stats qs, int interativo){
    int store = -1;

    print_init();

    Master_Manager master_manager = NULL;
    store = store_data_until_correct(&master_manager, interativo);

    if(store == 0) store = input_queries(master_manager, interativo);

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

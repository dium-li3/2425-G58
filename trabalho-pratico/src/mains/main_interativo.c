#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "master_manager.h"
#include "parser.h"
#include "queries.h"

int trabalho_interativo(Query_stats qs, int interativo){
    char input_buffer[BUFSIZ] = {'\0'};
    
    initscr();
    
    getnstr(input_buffer, BUFSIZ-1);    //BUFSIZ-1 para poder guardar o \0

    char **entity_paths = pathEntities(input_buffer);
    Master_Manager master_manager = create_master_manager (); 

    //Armazenamento e ordenação da informação + validação
    store_Entities(entity_paths, master_manager);
    freeEntityPaths(entity_paths);

    //Resposta às queries
    answer_all_queries(parser_queries, master_manager, qs, interativo);

    //free moment
    free_master_manager(master_manager);
    close_parser(parser_queries);

    endscr();

    return 0;
}

int main (int argc, char **argv){
    if (argc != 0){
        printf("Argumentos a mais.\n");
        return 1;
    }
    Query_stats qs = NULL;
    int interativo = 1;

    int r = trabalho_interativo(qs, interativo);
    
    return r;
}

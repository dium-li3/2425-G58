#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "master_manager.h"
#include "parser.h"
#include "queries.h"

int trabalho (char **argv, Query_stats qs, int interativo){
    int store = 0;
    
    char *path = argv[1];

    Parser parser_queries = open_parser (argv[2]);
    if(parser_queries == NULL) {
        perror("trabalho(11)");
        return 1;
    }

    char **entity_paths = pathEntities (path);
    Master_Manager master_manager = create_master_manager (); 

    //Armazenamento e ordenação da informação + validação
    store = store_Entities(entity_paths, master_manager, interativo);
    freeEntityPaths(entity_paths);

    //Resposta às queries
    if(store == 0) answer_all_queries(parser_queries, master_manager, qs);

    //free moment
    free_master_manager(master_manager);
    close_parser(parser_queries);

    return store;
}

int main (int argc, char **argv){
    if (argc != 3){
        printf("não incluiste os ficheiros direito.\n");
        return 1;
    }
    Query_stats qs = NULL;
    int interativo = 0;

    int r = trabalho (argv, qs, interativo);
    
    return r;
}

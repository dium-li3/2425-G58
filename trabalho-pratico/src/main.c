#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "master_manager.h"
#include "parser.h"
#include "queries.h"

int trabalho (int argc, char **argv, Query_stats qs){
    char *path = argv[1];
    Parser parser_queries = open_parser (argv[2]);
    
    if (path[0] == '\0') {
        //perror("ERROR: "); //o ficheiro com o nome dado não existe ou a diretoria foi mal escrita
        return 2;
    }

    char **entity_paths = path3Entities (path);
    Master_Manager master_manager = create_master_manager (); 
    
    //Armazenamento e ordenação da informação + validação
    store_Entities(entity_paths, master_manager);
    free3Entities(entity_paths);
    
    //Resposta às queries
    answer_all_queries(parser_queries, master_manager, qs);
    
    //free moment
    free_master_manager(master_manager);
    close_parser(parser_queries);
    
    return 0;
}

int main (int argc, char **argv){
    if (argc != 3){
        printf("não incluiste os ficheiros direito.\n");
        return 1;
    }
    Query_stats qs = NULL;
    int r = trabalho (argc, argv, qs);
    
    return r;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity_manager.h"
#include "parser.h"
#include "queries.h"
#include <sys/resource.h>
#include <time.h>
#include "output.h"
#include "testagem.h"


int trabalho (int argc, char **argv){
    char *path = argv[1];


    Parser parser_queries = open_parser (argv[2]);
    
    if (path[0] == '\0') {
        //perror("ERROR: "); //o ficheiro com o nome dado não existe ou a diretoria foi mal escrita
        return 2;
    }
    char **entity_paths = path3Entities (path);
    Entity_Manager entity_manager = create_entity_manager (); 
    //Armazenamento e ordenação da informação + validação
    store_Entities(entity_paths, entity_manager);
    free3Entities(entity_paths);
    //Resposta às queries
    answer_all_queries (parser_queries, entity_manager);
    //free moment
    free_entity_manager (entity_manager);
    close_parser(parser_queries);
    return 0;
}



int main (int argc, char **argv){
    if(argc != 4) {
        printf("Os argumentos são inválidos.\n");
        return 1;
    }

    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);

    int r = trabalho (argc, argv);

    if(r==0) {
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        struct rusage r_usage;
        getrusage(RUSAGE_SELF, &r_usage);

        testagem(argv[3]);

        printf("Utilização de memória: %ld KB\n\n", r_usage.ru_maxrss);
        printf("Tempo total: %.6fs\n\n", elapsed);
    }

    return r;
}

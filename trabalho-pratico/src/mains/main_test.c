#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "master_manager.h"
#include "parser.h"
#include "queries.h"
#include <sys/resource.h>
#include <time.h>
#include "output.h"
#include "testagem.h"


int trabalho_test(char **argv, Query_stats qs, double elapsed[], int interativo){
    int store = 0;  //código de sucesso para o armazenamento
    
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);


    char *path = argv[1];

    Parser parser_queries = open_parser (argv[2]);
    if(parser_queries == NULL) {
        free(qs);
        perror("Inputs das queries:");
        return 1;
    }


    char **entity_paths = pathEntities (path);
    Master_Manager master_manager = create_master_manager(); 


    //Armazenamento e ordenação da informação + validação
    store = store_Entities(entity_paths, master_manager, interativo);
    freeEntityPaths(entity_paths);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed[1] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;


    //Resposta às queries
    if(store == 0) answer_all_queries(parser_queries, master_manager, qs, interativo);
    else free(qs);  
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed[2] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    elapsed[2] -= elapsed[1];
    

    //free moment
    free_master_manager (master_manager);
    close_parser(parser_queries);
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed[3] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    elapsed[3] = elapsed[3] - elapsed[2] - elapsed[1];

    return store;
}



int main (int argc, char **argv){
    if(argc != 4) {
        printf("Os argumentos são inválidos.\n");
        return 1;
    }

    Query_stats qs = create_query_stats();
    int interativo = 0;
    
    struct timespec start, end;
    double elapsed[4];    //0->total; 1->store; 2->answer; 3->free

    clock_gettime(CLOCK_REALTIME, &start);

    int r = trabalho_test(argv, qs, elapsed, interativo);

    if(r==0) {
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed[0] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        struct rusage r_usage;
        getrusage(RUSAGE_SELF, &r_usage);

        testagem(argv[3]);

        printf("\nTempos de execução para cada query (média|total):\n");
        for(int i = 0; i < QUERYTYPES; i++) print_query_time(i+1, get_query_stats_n(qs, i), get_query_stats_time(qs, i));
        
        free(qs);

        print_elapsed_times(elapsed);
        printf("\nUtilização de memória: %ld KB\n\n", r_usage.ru_maxrss);
    }

    return r;
}

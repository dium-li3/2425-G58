#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity_manager.h"
#include "parser.h"
#include "queries.h"
#include <sys/resource.h>
#include <time.h>
#include "output.h"


void update_paths(char *rf, char *ef, char *rd, char *ed, int i) {
    snprintf(rf, 40, "%scommand%d_output.txt", rd, i);
    snprintf(ef, 50, "%scommand%d_output.txt", ed, i);
}


void close_files(FILE *p1, FILE *p2) {
    fclose(p1);
    fclose(p2);
}


/*
    Retorna 0 se houver discrepâncias ou 1 se não houver.
*/
int compare_files(FILE *rfp, FILE *efp) {
        char *line_r = NULL, *line_e = NULL;
        size_t nr = 0, ne = 0;
        int r = 1;

        while((getline(&line_r, &nr, rfp) != -1) && ((getline(&line_e, &ne, efp) != -1)) && r) {
            r = !(strcmp(line_r, line_e));
        }

        free(line_r);
        free(line_e);

        return r;
}


void testagem(char *expected) {
    printf("\n\n===TESTAGEM===\n\n\n");

    char *results_dir = strdup("resultados/");
    char *expected_dir = strdup(expected);

    char results_file[40] = {'\0'};
    char expected_file[50] = {'\0'};

    FILE *rfp = NULL, *efp = NULL;
    int i = 1, corrects = 0, temp = 0;

    update_paths(results_file, expected_file, results_dir, expected_dir, i);

    while((rfp = fopen(results_file, "r")) && (efp = fopen(expected_file, "r"))) {
        temp = compare_files(rfp, efp);

        (temp==0)? printf("Erro na query %d.\n", i) : corrects++;
 
        update_paths(results_file, expected_file, results_dir, expected_dir, ++i);

        close_files(rfp, efp);
    }

    free(results_dir);
    free(expected_dir);

    printf("Queries certas: %d/%d\n\n", corrects, i-1);
}



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

        testagem(argv[3]);

        struct rusage r_usage;
        getrusage(RUSAGE_SELF, &r_usage);
        printf("Utilização de memória: %ld KB\n\n", r_usage.ru_maxrss);
        printf("Tempo total: %.6fs\n\n", elapsed);
    }

    return r;
}

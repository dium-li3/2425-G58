#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity_manager.h"
#include "parser.h"
#include "queries.h"
#include <sys/resource.h>
#include "utils.h"

int trabalho (int argc, char **argv){
    char *path = argv[1];

    FILE *fp_queries = fopen (argv[2], "r");
    
    if (fp_queries == NULL || path[0] == '\0') {
        //perror("ERROR: "); //o ficheiro com o nome dado não existe ou a diretoria foi mal escrita
        return 2;
    }
    FILE **fp_entities = fopen3Entities (path);
    if (fp_entities == NULL)
        return -1;
    make_dir ("dataset-errors");
    make_dir ("resultados");
    Entity_Manager entity_manager = create_entity_manager (); 
    //Armazenamento e ordenação da informação (e validação sintática :3)
    store_Entities(fp_entities, entity_manager);
    fclose3Entities (fp_entities);
    //validação lógica da informação
    //Resposta às queries
    answer_all_queries (fp_queries, entity_manager);
    //free moment
    free_entity_manager (entity_manager);
    fclose(fp_queries);
    return 0;
}

int main (int argc, char **argv){
    if (argc != 3){
        printf("não incluiste os ficheiros direito.\n");
        return 1;
    }
    int r = trabalho (argc, argv);
    
    // Execute work
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    printf("Memory usage: %ld KB\n", r_usage.ru_maxrss);

    return r;
}
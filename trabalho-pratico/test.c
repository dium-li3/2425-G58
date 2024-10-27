#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void update_paths(char *rf, char *ef, char *rd, char *ed, int i) {
    snprintf(rf, 40, "%scommand%d_output.txt", rd, i);
    snprintf(ef, 50, "%scommand%d_output.txt", ed, i);
}



void close_files(FILE *p1, FILE *p2) {
    fclose(p1);
    fclose(p2);
}



/*
    Retorna 0 se houver discrepâncias ou 1 se não houver. Isto para ir somando o nº de queries certas.
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



int main(int argc, char** argv) {
    if(argc != 4) {
        printf("Os argumentos são inválidos.\n");
        return 1;
    }

    char *results_dir = strdup("resultados/");
    char *expected_res_dir = strdup(argv[3]);

    char results_file[40] = {'\0'};
    char expected_res_file[50] = {'\0'};

    FILE *rfp = NULL, *efp = NULL;
    int i = 1, corrects = 0, temp = 0;

    update_paths(results_file, expected_res_file, results_dir, expected_res_dir, i);

    while((rfp = fopen(results_file, "r")) && (efp = fopen(expected_res_file, "r"))) {
        temp = compare_files(rfp, efp);

        (temp==0)? printf("Erro na query %d.\n", i) : corrects++;
 
        update_paths(results_file, expected_res_file, results_dir, expected_res_dir, ++i);

        close_files(rfp, efp);
    }

    free(results_dir);
    free(expected_res_dir);

    printf("Queries certas: %d/%d\n", corrects, i-1);
    
    return 0;
}
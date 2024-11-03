#include "testagem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"


void update_paths(char *rf, char *ef, char *rd, char *ed, int i) {
    snprintf(rf, 40, "%scommand%d_output.txt", rd, i);
    snprintf(ef, 50, "%scommand%d_output.txt", ed, i);
}


/*
    Retorna 0 se houver discrepâncias ou 1 se não houver.
*/
int compare_files(Parser rp, Parser ep) {
        char *line_r = NULL, *line_e = NULL;
        int r = 1;

        line_r = parse_1line_alt(rp);
        line_e = parse_1line_alt(ep);

        while((get_nRead(rp) != -1) && ((get_nRead(ep) != -1)) && r) {
            r = !(strcmp(line_r, line_e));

            free(line_r);
            free(line_e);

            line_r = parse_1line_alt(rp);
            line_e = parse_1line_alt(ep);
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

    Parser rp = NULL, ep = NULL;
    int i = 1, corrects = 0, temp = 0;

    update_paths(results_file, expected_file, results_dir, expected_dir, i);

    while((rp = open_parser(results_file)) && (ep = open_parser(expected_file))) {
        temp = compare_files(rp, ep);

        (temp==0)? printf("Erro na query %d.\n", i) : corrects++;
 
        update_paths(results_file, expected_file, results_dir, expected_dir, ++i);

        close_parser(rp);
        close_parser(ep);
    }

    if(rp != NULL) close_parser(rp);
    if(ep != NULL) close_parser(ep);

    free(results_dir);
    free(expected_dir);

    printf("Queries certas: %d/%d\n\n", corrects, i-1);
}

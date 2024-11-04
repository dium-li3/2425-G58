#include "testagem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "output.h"


void update_paths(char *rf, char *ef, char *rd, char *ed, int i) {
    snprintf(rf, 100, "%scommand%d_output.txt", rd, i);
    snprintf(ef, 55, "%scommand%d_output.txt", ed, i);
}


/*
    Retorna 0 se houver discrepâncias ou 1 se não houver.
*/
int compare_files(Parser rp, Parser ep, GSList **l) {
        char *line_r = NULL, *line_e = NULL;
        int i;

        parse_1line(rp, &line_r);
        parse_1line(ep, &line_e);

        for(i = 1; (get_nRead(rp) != -1) && ((get_nRead(ep) != -1)); i++) {
            if((strcmp(line_r, line_e)) != 0){
                *l = g_slist_prepend(*l, GINT_TO_POINTER(i));
            }

            free(line_r);
            free(line_e);

            parse_1line(rp, &line_r);
            parse_1line(ep, &line_e);
        }

        free(line_r);
        free(line_e);

        return ((*l != NULL)? 0 : 1);
}


void testagem(char *expected) {
    printf("\n\n===TESTAGEM===\n\n\n");

    char *results_dir = strdup("resultados/");
    char *expected_dir = strdup(expected);

    char results_file[100] = {'\0'};
    char expected_file[55] = {'\0'};

    Parser rp = NULL, ep = NULL;
    int i = 1, corrects = 0;
    GSList *error_lines = NULL;

    update_paths(results_file, expected_file, results_dir, expected_dir, i);

    for (rp = open_parser(results_file), ep = open_parser(expected_file); (rp && ep); rp = open_parser(results_file), ep = open_parser(expected_file)){
        corrects += compare_files(rp, ep, &error_lines);
        
        if(error_lines != NULL){
            printf("\nErro na query %d:\n", i);
            error_lines = g_slist_reverse(error_lines);
            print_query_errors(error_lines);
            g_slist_free(error_lines);
            error_lines = NULL;
        }
 
        update_paths(results_file, expected_file, results_dir, expected_dir, ++i);

        close_parser(rp);
        close_parser(ep);
    }

    if(rp != NULL) close_parser(rp);
    if(ep != NULL) close_parser(ep);

    free(results_dir);
    free(expected_dir);

    printf("\nQueries certas: %d/%d\n\n", corrects, i-1);
}

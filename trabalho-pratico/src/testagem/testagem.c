#include "testagem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "output.h"

#define LEN_RESULTS 40


void update_paths(char *rf, char *ef, char *rd, char *ed, int i, size_t len_expected) {
    snprintf(rf, LEN_RESULTS, "%scommand%d_output.txt", rd, i);
    snprintf(ef, len_expected, "%scommand%d_output.txt", ed, i);
}


/*
    Retorna 0 se houver discrepâncias ou 1 se não houver.
*/
int compare_files(Parser rp, Parser ep, GArray *a) {
        char *line_r = NULL, *line_e = NULL;
        int i;

        parse_1line(rp, &line_r);
        parse_1line(ep, &line_e);

        for(i = 1; (get_nRead(rp) != -1) && ((get_nRead(ep) != -1)); i++) {
            if((strcmp(line_r, line_e)) != 0){
                a = g_array_insert_val(a, a->len, i);
            }

            free(line_r);
            free(line_e);

            parse_1line(rp, &line_r);
            parse_1line(ep, &line_e);
        }

        free(line_r);
        free(line_e);

        return ((a->len != 0)? 0 : 1);
}


void testagem(char *expected) {
    printf("\n\n===TESTAGEM===\n\n");

    char *results_dir = strdup("resultados/");
    char *expected_dir = strdup(expected);

    size_t len_expected = strlen(expected) + 25;
    char *results_file = calloc(LEN_RESULTS, sizeof(char));
    char *expected_file = calloc(len_expected, sizeof(char));

    Parser rp = NULL, ep = NULL;
    int i = 1, corrects = 0;
    GArray *error_lines = g_array_sized_new(FALSE, FALSE, sizeof(int), 10);

    update_paths(results_file, expected_file, results_dir, expected_dir, i, len_expected);

    for (rp = open_parser(results_file), ep = open_parser(expected_file); (rp && ep); rp = open_parser(results_file), ep = open_parser(expected_file)){
        corrects += compare_files(rp, ep, error_lines);
    
        if(error_lines->len != 0){
            printf("\nErro no comando %d:\n", i);
            print_query_errors(error_lines);
            error_lines->len = 0;
        }
 
        update_paths(results_file, expected_file, results_dir, expected_dir, ++i, len_expected);

        close_parser(rp);
        close_parser(ep);
    }
    
    g_array_free(error_lines, TRUE);
    if(rp != NULL) close_parser(rp);
    if(ep != NULL) close_parser(ep);

    free(results_dir);
    free(expected_dir);
    free(results_file);
    free(expected_file);

    printf("\nResultados corretos: %d/%d\n\n", corrects, i-1);
}

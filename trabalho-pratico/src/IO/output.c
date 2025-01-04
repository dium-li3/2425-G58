#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "output.h"
#include "parser.h"

typedef struct output{
    FILE *fp;
    char separador;
    int terminal;
} *Output;



Output open_out (char *path, char separador, int terminal){
    Output out = calloc (1, sizeof (struct output));
    out->fp = fopen (path, "a");
    out->separador = separador;
    out->terminal = terminal;
    return out;
}

void close_output(Output out){
    fclose (out->fp);
    free (out);
}

void output_empty (Output out){
    fprintf(out->fp, "\n");
}

void output_geral (char **infos, int n_infos, Output out){
    if(out->terminal != 0) {
        printw("%s", infos[0]);
        for (int i = 1; i < n_infos; i++){
            printw("%c%s", out->separador, infos[i]);
        }
        addch('\n');
    }
    
    fprintf(out->fp, "%s", infos[0]);
    for (int i = 1; i < n_infos; i++){
        fprintf(out->fp, "%c%s", out->separador, infos[i]);
    }
    fprintf(out->fp, "\n");
}

void error_output (Parser p, Output out){
    char *line = NULL;
    go_back_1line (p);
    parse_1line (p, &line);
    fprintf(out->fp, "%s", line);
    free (line);
}


void set_terminal_true(Output out) {
    out->terminal = 1;
}


void print_query_errors(GArray *a) {
    int i, len = a->len;

    for(i = 0; i < len; i++) {
        printf("    -Discrepância na linha %d.\n", g_array_index(a, int, i));
    }
}

void print_elapsed_times(double elapsed[]) {
    printf("\nTempo de armazenamento: %.6fs\n", elapsed[1]);
    printf("\nTempo de resposta a queries: %.6fs\n", elapsed[2]);
    printf("\nTempo de free: %.6fs\n", elapsed[3]);
    printf("\nTempo total: %.6fs\n\n", elapsed[0]);
}

void print_query_time(int type, int n, double time) {
    printf("    -Query %d: %.6fms | %.6fms\n", type, time/n, time);
}

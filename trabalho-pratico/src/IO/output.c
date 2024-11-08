#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "parser.h"

typedef struct output{
    FILE *fp;
} *Output;



Output open_out (char *path){
    Output out = calloc (1, sizeof (struct output));
    out->fp = fopen (path, "w");
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
    fprintf(out->fp, "%s", infos[0]);
    for (int i = 1; i < n_infos; i++){
        fprintf(out->fp, ";%s", infos[i]);
    }
    fprintf(out->fp, "\n");
}

void output_user(char *email, char *first_name, char *last_name, int age, char *country, Output out){
    fprintf(out->fp, "%s;%s;%s;%d;%s\n", email, first_name, last_name, age, country);
}

void output_artist (char *name, char *type, char *disc_duration, char *country, Output out){
    fprintf(out->fp, "%s;%s;%s;%s\n", name, type, disc_duration, country);
}

void output_genre (char *name_gen, int likes, Output out){
    fprintf(out->fp, "%s;%d\n", name_gen, likes);
}

void error_output (Parser p, Output out){
    char *line = NULL;
    go_back_1line (p);
    parse_1line (p, &line);
    fprintf(out->fp, "%s", line);
    free (line);
}

/*
    Percorre o array para ir printando as linhas onde há erros (programa de testes).
*/
void print_query_errors(GArray *a) {
    int i;
    for(i = 0; i < a->len; i++) {
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
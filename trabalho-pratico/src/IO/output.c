#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "output.h"
#include "parser.h"
#include "queries.h"

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
    Percorre a lista para ir printando as linhas onde há erros (programa de testes).
*/
void print_query_errors(GArray *a) {
    int i;
    for(i = 0; i < a->len; i++) {
        printf("    -Discrepância na linha %d.\n", g_array_index(a, int, i));
    }
}

void print_query_time(Query_stats qs) {
    int i;
    double t;

    printf("\nTempos de execução para cada query (média|total):\n");

    for(i = 0; i < 3; i++) {
        t = get_query_data_time(qs, i);
        printf("    -Query %d: %.6fms | %.6fms\n", i+1, t/get_query_data_n(qs, i), t);
    }
}
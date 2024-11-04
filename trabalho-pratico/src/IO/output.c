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
void print_query_errors(GSList *l) {
    while (l != NULL) {
        printf("    -Discrepância na linha %d.\n", GPOINTER_TO_INT(l->data));
        l = l->next;
    }
}

void print_query_time(Query_data qd) {
    int i;
    double t;

    printf("\nTempos de execução para cada query (média|total):\n");

    for(i = 0; i < 3; i++) {
        t = get_query_data_time(qd, i);
        printf("    -Query %d: %.6fms | %.6fms\n", i+1, t/get_query_data_n(qd, i), t);
    }
}
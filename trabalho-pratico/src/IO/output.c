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

void output_user(char *email, char *first_name, char *last_name, int age, char *country, Output out){
    fprintf(out->fp, "%s;%s;%s;%d;%s\n", email, first_name, last_name, age, country);
}

void output_artist (char *name, char *type, char *disc_duration, char *country, Output out){
    fprintf(out->fp, "%s;%s;%s;%s\n", name, type, disc_duration, country);
}

void error_output (Parser p, Output out, char **line){
    *line = NULL;
    go_back_1line (p);
    parse_1line (p, line);
    fprintf(out->fp, "%s", *line);
    free (*line);
}
#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include "parser.h"
#include "testagem.h"

typedef struct output *Output;

Output open_out (char *path);

void close_output(Output out);

void output_empty (Output out);

void output_user(char *email, char *first_name, char *last_name, int age, char *country, Output out);

void output_artist (char *name, char *type, char *disc_duration, char *country, Output out);

void output_genre (char *name_gen, int likes, Output out);

void error_output (Parser p, Output out);

void print_query_errors(GArray *a);

void print_elapsed_times(double elapsed[]);

#endif
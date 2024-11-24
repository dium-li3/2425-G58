#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include "parser.h"
#include "testagem.h"

typedef struct output *Output;

Output open_out (char *path, char separador);

void close_output(Output out);

void output_empty (Output out);

void output_geral (char **infos, int n_infos,Output out);

void error_output (Parser p, Output out);

void print_query_errors(GArray *a);

void print_elapsed_times(double elapsed[]);

void print_query_time(int type, int n, double time);

#endif
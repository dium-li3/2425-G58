#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include "parser.h"

typedef struct output *Output;

Output open_out (char *path);

void close_output(Output out);

void output_empty (Output out);

void output_user(char *email, char *first_name, char *last_name, int age, char *country, Output out);

void output_artist (char *name, char *type, char *disc_duration, char *country, Output out);

void error_output (Parser p, Output out, char **line);

#endif
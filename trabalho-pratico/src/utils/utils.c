#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "parser.h"


void error_output (FILE *fp_reading, FILE *fp_writing, char **line, int nRead){
    *line = NULL;
    fseek (fp_reading, -nRead, SEEK_CUR);
    parse_1line (fp_reading, line);
    fprintf(fp_writing, "%s", *line);
    free (*line);
}
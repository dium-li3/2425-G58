#include <glib.h>
#include <ctype.h>
#include <stdio.h>

#include "utils.h"
#include "history.h"

typedef struct history{
    long file_pos;
} *History;


History create_history (long file_pos){
    History h = malloc (sizeof (struct history));
    h->file_pos = file_pos;
    return h;
}

gboolean valid_platform (char *platform){
    string_to_lower (platform);
    return same_string (platform, "mobile", "desktop");
}

//Devolve a data e duração de uma string que contém essa informação
int read_timestamp_elements (char *str, int *year, int *month, int *day){
    int dur = 0;
    sscanf(str, "%d/%d/%d %d", year, month, day, &dur);
    return dur;
}

History create_history_from_tokens (char **tokens, long file_pos, int *hist_id, int *user_id, int *music_id, int *year, int *month, int *day, int *dur){
    History h = NULL;
    if (valid_duration (tokens[4]) && valid_platform (tokens[5])){
        *hist_id = atoi (tokens[0]+1);
        *user_id = atoi (tokens[1]+1);
        *music_id = atoi (tokens[2]+1);
        
        sscanf(tokens[3], "%d/%d/%d", year, month, day);
        *dur = calc_duration_s (tokens[4]);
        h = create_history (file_pos);
    }
    return h;
}

long get_history_pos (History h){
    return h->file_pos;
}

void free_history (History h){
    free (h);
}

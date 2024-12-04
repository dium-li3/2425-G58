#include <glib.h>
#include <ctype.h>

#include "utils.h"
#include "history.h"

typedef struct history{
    int day;
    int month;
    int hour_of_day; //char?? ;)
    int duration;
    int music_id;
    //GArray *artist_ids; 200MB no dataset grande
} *History;

History create_history (int day, int month, int hour, int duration, int music_id){
    History h = malloc (sizeof (struct history));
    h->day = day;
    h->month = month;
    h->hour_of_day = hour;
    h->duration = duration;
    h->music_id = music_id;
    //h->artist_ids = NULL;
    return h;
}

gboolean valid_platform (char *platform){
    string_to_lower (platform);
    return same_string (platform, "mobile", "desktop");
}

History create_history_from_tokens (char **tokens, int *year){
    History h = NULL;
    int day, month, hour_of_day, music_id, duration;
    if (valid_duration (tokens[4]) && valid_platform (tokens[5])){
        //timestamp são todas => yyyy/mm/dd hh:mm:ss   nunca têm erros, e são o tokens[3]
        *year = atoi (tokens[3]);
        month = atoi (tokens[3] + 5);
        day = atoi (tokens[3] + 8);
        hour_of_day = atoi (tokens[3] + 11);

        duration = calc_duration_s (tokens[4]);
        music_id = atoi (tokens[2] + 1); 

        h = create_history (day, month, hour_of_day, duration, music_id);
    }
    return h;
}

int get_history_music (History h){
    return h->music_id;
}

// void set_artist_ids (History h, GArray *artist_ids){
//     h->artist_ids = artist_ids;
// }

void free_history (History h){
    // if (h->artist_ids != NULL)
    //     g_array_free(h->artist_ids, TRUE);
    free (h);
}

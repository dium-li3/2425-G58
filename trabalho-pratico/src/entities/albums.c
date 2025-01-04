#include <stdlib.h>
#include <glib.h>

#include "parser.h"
#include "albums.h"
#include "utils.h"

typedef struct album{
    int id;
    char *title;
} *Album;

Album create_album_from_tokens (char* title){
    Album album = malloc (sizeof (struct album));
    album->title = strdup (title);
    return album;
}

void free_album (Album album){
    free (album->title);
    free (album);
}
#include <stdlib.h>
#include <glib.h>

#include "parser.h"
#include "albums.h"
#include "utils.h"

typedef struct album{
    int id;
    char *title;
} *Album;

/*
    Cria um album com um dado título.
*/
Album create_album_from_tokens (char* title){
    Album album = malloc (sizeof (struct album));
    album->title = strdup (title);
    return album;
}

//Liberta o espaço usado por um album.
void free_album (Album album){
    free (album->title);
    free (album);
}
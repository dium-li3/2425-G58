#include <stdlib.h>
#include <glib.h>

#include "parser.h"
#include "albums.h"
#include "utils.h"

typedef struct album{
    int id;
    char *title;
} *Album;

typedef struct generic_album{
    int id;
    char *title;
    GArray *artists;
} *Generic_Album;

/*
    Cria um album que apenas contém informação
    100% util e que vai ser guardado na hashtable.

*/
Album create_album (char* title){
    Album album = malloc (sizeof (struct album));
    album->title = strdup (title);
    return album;
}

/*
    Cria um album generico com dados parâmetros.
*/
Generic_Album create_generic_album(int id, char *title, GArray *artists){
    Generic_Album album = malloc (sizeof (struct generic_album));
    album->id = id;
    album->title = strdup (title);
    album->artists = artists;
    return album;
}

/*
    Cria um album generico atraves de tokens.
    Sendo que cada token contém informação que validará
    o album, ou será guardada por ser útil mais à frente.
*/
Generic_Album create_generic_album_from_tokens (char **tokens){
    int valid_lists = valid_list (tokens[2]) && valid_list (tokens[4]);
    Generic_Album album = NULL;
    if (valid_lists){
        int id = atoi (tokens[0]);
        GArray *artists = store_list(tokens[2]);
        album = create_generic_album (id, tokens[1], artists);
    }
    return album;
}

/*
    Liberta o espaço usado por um album genérico.
*/
void free_generic_album (Generic_Album a){
    free (a->title);
    g_array_free (a->artists, TRUE);
    free (a);
}

/*
    Cria o album que vai ser guardado na hashtable do manager
    e dá free do album generico.

    Pressupõe que já se atualizou os artistas que fizeram o album.
    e que se guardou o id do album para ser usado na inserção na hashtable.
*/
Album create_album_delete_generic (Generic_Album al){
    Album a = create_album (al->title);
    free_generic_album (al);
    return a;
}

//Devolve o id de um dado album genérico.
int get_album_id (Generic_Album al){
    return al->id;
}

//Devolve o array de artistas que participaram do album.
const GArray *get_album_artists (Generic_Album al){
    return al->artists;
}

//Liberta o espaço usado por um album.
void free_album (Album album){
    free (album->title);
    free (album);
}
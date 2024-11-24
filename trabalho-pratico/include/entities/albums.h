#ifndef ALBUMS_H
#define ALBUMS_H
#include <glib.h>

typedef struct album *Album;

typedef struct generic_album *Generic_Album;

/*
    Cria um album generico atraves de tokens.
    Sendo que cada token contém informação que validará
    o album, ou será guardada por ser útil mais à frente.
*/
Generic_Album create_generic_album_from_tokens (char **tokens);

/*
    Liberta o espaço usado por um album genérico.
*/
void free_generic_album (Generic_Album a);

/*
    Cria o album que vai ser guardado na hashtable do manager
    e dá free do album generico.

    Pressupõe que já se atualizou os artistas que fizeram o album.
    e que se guardou o id do album para ser usado na inserção na hashtable.
*/
Album create_album_delete_generic (Generic_Album al);

//Devolve o id de um dado album genérico.
int get_album_id (Generic_Album al);

//Devolve o array de artistas que participaram do album.
const GArray *get_album_artists (Generic_Album al);

//Liberta o espaço usado por um album.
void free_album (Album album);

#endif
#ifndef ALBUMS_H
#define ALBUMS_H

typedef struct album *Album;

/*
    Cria um album com um dado título.
*/
Album create_album_from_tokens (char* title);

//Liberta o espaço usado por um album.
void free_album (Album album);

#endif
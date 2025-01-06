/**
 * @file albums.h
 * @author Grupo 58
 * @date Nov 2024
 * @brief Header file da entidade Album
 */
#ifndef ALBUMS_H
#define ALBUMS_H

typedef struct album *Album;

/**
 * @brief Cria um album com um dado título @p title .
 * 
 * @param title Título a guardar no álbum.
 * @return Álbum recém-criado. 
 */
Album create_album_from_tokens (char* title);

/**
 * @brief Liberta o espaço usado por um dado álbum @p album .
 * 
 * @param album Álbum recém-criado.
 */
void free_album (Album album);

#endif
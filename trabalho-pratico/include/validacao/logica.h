#ifndef LOGICA_H
#define LOGICA_H

#include "users.h"
#include "musics.h"
#include "artists.h"
#include "sintatica.h"
#include "artist_manager.h"
#include "music_manager.h"


/*
    Verifica que todos os artistas de uma dada lista existem.
    Caso existam, adiciona a duration dada à discografia de todos os
    artistas.
*/
int valid_artists(GSList *artists, int duration, Art_Manager am);

//Verifica se um dado utilizador é válido ou não.
int valid_musics (GSList *musicas, Music_Manager mm, short age);

#endif
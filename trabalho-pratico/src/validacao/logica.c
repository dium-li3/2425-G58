#include "logica.h"
#include "artist_manager.h"
#include "music_manager.h"

/*
    Verifica que todos os artistas de uma dada lista existem.
    Caso existam, adiciona a duration dada à discografia de todos os
    artistas.
*/
int valid_artists(GSList *artists, int duration, Art_Manager am)
{
    int r = 1;
    Artist a = NULL;
    GSList *temp = NULL;
    for (temp = artists; temp != NULL && r; temp = temp->next)
    {
        a = search_artist_by_id(get_art_id(temp->data), am); // hmmm, acho q vai ter de ser fora da criaçao da Music :c
        if (a == NULL)
            r = 0;
    }
    for (temp = artists; temp != NULL && r; temp = temp->next)
    {
        a = search_artist_by_id(get_art_id(temp->data), am);
        add_disc_duration(a, duration);
    }
    return r;
}

// Verifica se um dado utilizador é válido ou não.
int valid_musics(GSList *musics, Music_Manager mm, short age)
{
    int r = 1;
    Music m = NULL;
    GSList *temp = NULL;
    for (temp = musics; temp != NULL && r; temp = temp->next)
    {
        m = search_music_by_id(get_music_id(temp->data), mm); // hmmm, acho q vai ter de ser fora da criaçao da Music :c
        if (m == NULL)
            r = 0;
        else
        {
            char *gen = get_genre(m);
            add_like_genre(mm, gen, age);
            free (gen);
        }
    }
    // for (temp = musics; temp != NULL && r; temp = temp->next)
    // {
    //     m = search_music_by_id(get_music_id(temp->data), mm);
    // }
    return r;
}

#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <stdio.h>
#include "musics.h"
#include "artist_manager.h"
#include "output.h"

typedef struct genre *Genre;

typedef struct music_manager *Music_Manager;


/*
    @brief Cria um Music Manager.

    Aloca memória para um Music Manager (hash table de músicas e array de géneros, com as respetivas funções
    para libertar as entidades) com tudo inicializado a 0/NULL.

    @return Apontador para o Music Manager criado.
*/
Music_Manager create_music_manager();


/*
    @brief Adidiona likes ao género de cada música.

    Itera sobre o array de músicas e, para cada uma, adiciona um like ao seu género,
    segundo a idade do utilizador.

    @param musics Array das liked musics de um utilizador.

    @param mm Music Manager (que contém o array de Genres).

    @param age Idade do utilizador. 
*/
void add_like_genres (const GArray *musics, Music_Manager mm, short age);


/*
    @brief Transforma todos os arrays de likes dos géneros do Music Manager em arrays de likes acumulados.

    Percorre o array de Genres do Music Manager e, para cada Genre, transforma o seu array de likes num array
    de likes acumulados, de forma a que por exemplo, no índice 20 esteja o total de likes do género
    de utilizadores com 20 ou menos anos.

    @param mm Music Manager (que contém o array de Genres).
*/
void gen_arr_freq_acum(Music_Manager mm);

int cmp_like_gen (gconstpointer g1, gconstpointer g2);

void sort_gen(Music_Manager mm,int min_age, int max_age);

void insert_music_by_id(Music m, Music_Manager music_manager);

gboolean all_musics_exist (const GArray *musics, Music_Manager mm);

void store_Musics(char *music_path, Music_Manager mm, Art_Manager am);

void update_arr_total_likes(Music_Manager mm,int min_age, int max_age);

void print_all_genres_info(Music_Manager mm, char separador, Output out);


/*
    @brief Liberta um Music Manager.

    Liberta a memória alocada para a hash table das músicas e para o array dos géneros, usando as funções
    que foram passadas na criação do Music Manager.

    @param mm Music Manager a ser libertado.
*/
void free_music_manager(Music_Manager mm);

#endif
#ifndef LOGICA_H
#define LOGICA_H

#include "users.h"
#include "musics.h"
#include "artists.h"
#include "sintatica.h"

/*
typedef int ID;

//Verifica se um artista existe através do seu id.
int artistID_exists (int a);

//Verifica se uma música existe através do seu id.
int musicID_exists (int m);

//apesar de não ser necessário para verificar se é válido, poderia ficar aqui tb a funçao q procura o id do utilizador?
int userID_exists (int u);
*/

//Verifica se um artista é válido ou não.
int valid_artist (Artist a);

//Verifica se uma dada música é válida ou não.
int valid_music (Music m);

//Verifica se um dado utilizador é válido ou não.
int valid_user (User u);

#endif
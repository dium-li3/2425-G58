#ifndef _ARTISTS_
#define _ARTISTS_
#include <glib.h>

typedef struct artist *Artist;

//Dada uma linha dos dados de artistas, cria um artista.
Artist create_art (int id, char *nome, char *desc, float rps, int *idc, char *coun, char type);

//User create_user_from_tokens (char **token);??????

//Devolve o id de um artista.
int get_art_id (Artist a);

//Dada uma string com o tipo do artista, devolve o caracter que o representa.
char get_art_type (char *art_type);

//Dado o id do artista, devolve o seu tempo de discografia.
int tempo_disc_art (Artist a); //da pra fazer isso??? :(

//Dá print do nome, tipo, tempo de discografia e país do utilizador.
void print_art_info (Artist a);

//Dá free do artista
void free_art (Artist a);

#endif
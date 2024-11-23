#ifndef USERS_H
#define USERS_H

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "output.h"
#include "users.h"

typedef struct user *User;

/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
User create_user_from_tokens (char **token);

//Devolve o username de um utilizador.
int get_user_id (User u);

//Devolve o pointer do id do user, só é usada por uma única funçção da User_Manager
int *get_user_id_pointer (User u);

short get_user_age (User u);

const GArray *get_liked_musics(User u);

/*
    Dá print do email, nomes, idade e pais do utilizador.
*/
void print_user_info(User u, char separador, Output out);

//Dá free do user
void free_user (User u);

int valid_subscription (char *subs_type);

//Verifica se os campos que têm de ser sintáticamente validados de um dado utilizador estão direito/válidos.
int valid_user_sintatic (char *email, char *date, char *sub_type);

#endif
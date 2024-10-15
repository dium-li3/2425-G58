#ifndef USERS_H
#define USERS_H
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

#include "users.h"
#include "sintatica.h"
#include "parser.h"

typedef struct user *User;

//Dada uma linha dos dados de utilizadores, cria um User.
User create_user (int id, char *email, char *fn, char *ln, char *bd, short age, char *c, char st, GSList *lmids);

/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
void *create_user_from_tokens (char **token);

//Devolve o username de um utilizador.
int get_user_id (User u);

//Devolve o pointer do id do user, só é usada por uma única funçção da User_Manager
int *get_user_id_pointer (User u);

char *get_user_email (User u);

char *get_user_first_name(User u);

char *get_user_last_name(User u);

char *get_user_country(User u);

short get_user_age (User u);

//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type);


//Dá free do user
void free_user (User u);

#endif
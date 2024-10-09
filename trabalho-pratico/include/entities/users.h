#ifndef _USERS_
#define _USERS_
#include <glib.h>

typedef struct user *User;

//Dada uma linha dos dados de utilizadores, cria um User.
User create_user (int id, char *email, char *fn, char *ln, char *bd, short age, char *c, char st, GSList *lmids);

/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
User create_user_from_tokens (char **token);

//Devolve o username de um utilizador.
int get_user_id (User u);

//Devolva os anos passados de uma string que representa uma data.
short get_age (char *bd);

//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type);

//Dá print do email, nomes, idade e pais do utilizador.
void print_user_info (User u);

//Dá free do user
void free_user (User u);

#endif
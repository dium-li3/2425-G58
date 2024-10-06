#ifndef USER_H
#define USER_H

#include <glib.h>

typedef struct user {
    int username;
    char *email;
    char *first_name;
    char *last_name;
    char* birth_date;
    short age;
    char *country;
    char subscription_type; //normal = N, premium = P e inválido = E
    GSList *liked_musics_ids; 
}User;

//Dada uma linha dos dados de utilizadores, cria um User.
User create_user (int id, char *email, char *fname, char* lname, char* bd, short age, char *country, char st, GSList *lmids);

//Devolve a idade de um dado utilizador.
short get_age (User *u);

//Dá print do email, nomes, idade e pais do utilizador.
void print_user_info (User *u);

#endif
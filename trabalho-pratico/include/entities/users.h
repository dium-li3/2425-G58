#ifndef _USERS_
#define _USERS_
#include <glib.h>

typedef struct user{
    int username;
    char *email;
    char *first_name;
    char *last_name;
    char *birth_date; //possivelmente vai ser retirado
    short age;
    char *country;
    char subscription_type;//normal = N, premium = P e inválido = E?
    GSList *liked_music_ids;
} *User;


//Dada uma linha dos dados de utilizadores, cria um User.
User create_user (int id, char *email, char *fn, char *ln, char *bd, short age, char *c, char st, GSList *lmids);

//Devolva os anos passados de uma string que representa uma data.
short get_age (char *bd);

//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type);

//Dá print do email, nomes, idade e pais do utilizador.
void print_info (User u);

//Dá free do user
void free_user (User u);

#endif
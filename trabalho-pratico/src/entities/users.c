#include <stdlib.h>
#include <stdio.h>
#include "users.h"
#include "sintatica.h"

User create_user (int id, char *email, char *fn, char *ln, char *bd, short age, char *c, char st, GSList *lmids){
    User u = malloc(sizeof (struct user));
    u->username = id;
    u->email = strdup (email);
    u->first_name =strdup (fn);
    u->last_name = strdup (ln);
    u->birth_date = strdup (bd);
    u->age = age;
    u->country = strdup (c);
    u->subscription_type = st;
    u->liked_music_ids = lmids;
    return u;
}

//Devolva os anos passados de uma string que representa uma data.
short get_age (char *bd){
    short age = 0;
    if (!valid_date(bd))
        age = -1;
    else{
        short year_month_day[3];
        char *y_m_d = NULL;
        char *strpt = NULL;
        y_m_d = strtok_r (bd, "/", &strpt);
        year_month_day[0] = (short)atoi (y_m_d);
        for (int i = 1;(y_m_d = strtok_r (NULL, "/\n", &strpt)) != NULL && i < 3; i++)
            year_month_day[i] = (short)atoi (y_m_d);
        age = 2024 - year_month_day[0];
        if (year_month_day[1] < 9) age++;
        if (year_month_day[1] == 9){
            if (year_month_day[2] <= 9) age++;
        }
    }
    return age;
}

//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type){
    char c = 'E';
    if  (strcmp (sub_type, "normal") == 0)
        c = 'N';
    if (strcmp(sub_type, "premium") == 0)
        c = 'P';
    return c;
}

//Dá print do email, nomes, idade e pais do utilizador.
void print_info (User u){
    printf("%s;%s;%s;%d;%s\n",u->email, u->first_name, u->last_name, u->age, u->country);
}

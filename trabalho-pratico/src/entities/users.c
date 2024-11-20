#include <ctype.h>
#include "users.h"
#include "utils.h"
#include "output.h"

typedef struct user{
    int id;
    char *email;
    char *first_name;
    char *last_name;
    short age;
    char *country;
    GArray *liked_music_ids;
} *User;

User create_user (int id, char *email, char *fn, char *ln, short age, char *c, GArray *lmids){
    User u = malloc(sizeof (struct user));
    u->id = id;
    u->email = strdup (email);
    u->first_name =strdup (fn);
    u->last_name = strdup (ln);
    u->age = age;
    u->country = strdup (c);
    u->liked_music_ids = lmids;
    return u;
}

/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
User create_user_from_tokens (char **tokens){
    int valid = valid_user_sintatic (tokens[1], tokens[4], tokens[6]) && valid_list(tokens[7]);
    int id;
    int age;
    GArray *liked_musics = NULL;
    User u = NULL;
    if (valid){ //store
        id = atoi (tokens[0]+1);
        age = read_date_to_age (tokens[4]);
        liked_musics = store_list (tokens[7]);
        u = create_user (id, tokens[1], tokens[2], tokens[3], age, tokens[5], liked_musics);
    }
    return u;
}

//Devolve o username do utilizador.
int get_user_id(User u){
    return u->id;
}

int *get_user_id_pointer (User u){
    int *copy = malloc(sizeof(int));
    *copy = u->id;

    return copy;
}


short get_user_age (User u){
    return u->age;
}

const GArray *get_liked_musics(User u){
    return u->liked_music_ids;
}



/*
    Dá print do email, nomes, idade e pais do utilizador.
*/
void print_user_info(User u, Output out){
    if (u != NULL){
        char *email = strdup (u->email);
        char *first_name = strdup (u->first_name);
        char *last_name = strdup (u->last_name);
        char *country = strdup (u->country);
        output_user (email, first_name, last_name, u->age, country, out);
        free (email);
        free (first_name);
        free (last_name);
        free (country);
    }
    else output_empty (out);
}

void free_user(User u){
    free(u->email);
    free(u->first_name);
    free(u->last_name);
    free(u->country);
    if (u->liked_music_ids != NULL)
        g_array_free(u->liked_music_ids, TRUE);
    free(u);
}


/*
    Verifica se um email está escrito direito.
    username@lstring.rstring
    username só minusculas ou numeros
    lstring tamanho superior a 1
    rstring tamanho entre 2 e 3 inclusive
    lstring e rstring só têm minusculas.
*/
int valid_email_string (char *email){
    int us = 0;
    int ls = 0;
    int rs = 0;
    int r = 1;
    //Valida o username do email
    for (; (isdigit (email[us]) || islower (email[us])) && email[us] != '\0'; us++);
    if (us == 0 || email[us] != '@')
        r = 0; //return 0;
    else {//Valida a lstring
        ls = us+1;
        for (; islower(email[ls]) && email[ls] != '\0'; ls++);
        if (ls == 0 || email[ls] != '.')
            r = 0;
        else {//valida a rstring
            rs = ls + 1;
            for (;rs - ls < 4 && islower(email[rs]); rs++);
            if (rs - ls < 2 || email[rs] != '\0')
                r = 0;
        }
    }
    return r;
}

/*
    Verifica se uma data de nascimento é válida e está escrita direito.
    Datas são escritas na forma aaaa/mm/dd com
    meses entre 1 e 12 e dias entre 1 e 31
*/
int valid_date (char *date){
    int v;
    int mes = 0;
    int dia = 0;
    int ano = 0;
    v = strlen(date) == 10 ? 1 : 0;
    v = v ? date[4] == '/' && date[7] == '/' : 0;
    char *svptr;
    if (v){
        char *s = strdup(date); //já n se verifica se é td digitos entre 0-3, 5-6 e 8-9...
        ano = atoi (strtok_r (s, "/", &svptr));
        mes = atoi (strtok_r (NULL, "/", &svptr));
        dia = atoi (strtok_r (NULL, "/", &svptr));
        free(s);
        if (ano > 2024 || mes > 12 || dia > 31) v = 0;
        if (v && ano == 2024){
            if (mes > 9)
                v = 0;
            if (mes == 9)
                if (dia > 9)
                    v = 0;
        }
    }
    return v;
}

/*
    Verifica se uma subscription_type está escrita direito.
    Possivelmente vai ser apagada por ser kinda trivializada pela get_sub_type
    que nos dá logo se a subscription é válida ou não.
*/
int valid_subscription (char *subs_type){
    return (strcmp (subs_type, "normal") == 0 || strcmp (subs_type, "premium") == 0);
}

//Verifica se os campos que têm de ser sintáticamente validados de um dado utilizador estão direito/válidos.
int valid_user_sintatic (char *email, char *date, char *sub_type){
   // printf("CALL VALID DATE%s\n",date);
    return valid_email_string(email) && valid_date(date) && valid_subscription(sub_type);
}
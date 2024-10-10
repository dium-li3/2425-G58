#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "sintatica.h"


//Verifica se uma duração é válida e está escrita direito.
int valid_duration (char *duration){
    int r = 1;
    if (strlen (duration) != 8)
        r = 0;
    if (r && duration [2] != ':' && duration [5] != ':')
        r = 0;
    for (int i = 0; r && duration[i] != '\0'; i++){
        if (i == 2 || i == 5){
            i++;
            r = isdigit(duration[i]) && (duration[i] < '6');
        }
        r = isdigit (duration[i]);
    }
    return r;
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
    for (; (isdigit (email[us]) || islower (email[us])) && email[us] != '@' && email[us] != '\0'; us++);
    if (us == 0 || email[us] != '@')
        r = 0; //return 0;
    else {//Valida a lstring
        ls = us+1;
        for (; islower(email[ls]) && email[ls] != '.' && email[ls] != '\0'; ls++);
        if (ls == 0 || email[ls] != '.')
            r = 0;
        else {//valida a rstring
            rs = ls + 1;
            for (;rs - ls < 4 && email[rs] != '\0' && islower(email[rs]); rs++);
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
        char *s = strdup(date);
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
int valid_user_sintatic (char *email, char *date, char sub_type){
   // printf("CALL VALID DATE%s\n",date);
    return valid_email_string(email) && valid_date(date) && (sub_type != 'E');
}


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <glib.h>
//#include <regex.h>
#include "utils.h"

int valid_list(char *lista) {
    return (lista[0] == '[' && lista[strlen(lista)-1] == ']');
}

void free_tokens(char **tokens, int n){
    for (int i = 0; i < n; i++){
        free (tokens[i]);
    }
    free (tokens);
}

int calc_duration_s(char *st) {
    int h = 0, m = 0, s = 0;
    sscanf(st, "%d:%d:%d", &h, &m, &s);

    return (h*3600 + m*60 + s);
}

char *calc_duration_hms(int segs){
    int h = segs / 3600;
    int t = segs % 3600;
    int m = t / 60;
    int s = t % 60;
    char *hms =calloc (33, sizeof(char));
    sprintf(hms, "%02d:%02d:%02d", h, m, s);
    return hms;
}

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

void string_to_lower (char *s){
    int i;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = tolower (s[i]);
}

int same_string (const char *comp, const char *s1, const char *s2){
    int r = 0;
    if (strcmp (comp, s1) == 0)
        r = 1;
    else if (strcmp (comp, s2) == 0)
        r = 2;
    return r;
}

int valid_duration (char *duration){
    int r = 1;
    if (strlen (duration) != 8) r = 0;

    for (int i = 0; r && duration[i] != '\0'; i++){
        if (i == 2 || i == 5) r = duration[i] == ':';
        else if (i == 3 || i == 6) r = isdigit (duration[i]) && duration[i] < '6';
        else r = isdigit (duration[i]);
    }

    return r;
}

int calc_week(int d, int m, int y) {
    int t, leap, week;

    leap = (y%4 == 0)? 1 : 0;

    if(m > 8)
        t = d + (m-1)*31 - m/2 - 1 + leap; //- 1 + leap é o ajuste por causa dos anos bissextos
    else if (m > 2)
        t = d + (m-1)*31 - (m-1)/2 - 2 + leap; //- 2 + leap é o ajuste por causa dos anos bissextos
    else
        t = d + (m-1)*31;
    
    if(y != 2024) {
        t = 365 + leap - t;
        t = t + 365*(2023-y) + (2023-y)/4 + 253; //253 é o número de dias em 2024 até 09/09/2024
    }
    else t = 253 - t; //253 é o número de dias em 2024 até 09/09/2024

    week = (t+4)/7;

    return week;
}

int array_max (int array[], int N){
    int i, r = 0;
    for (i = 1; i < N; i++)
        if (array[i] > array[r])
            r = i;
    return r;
}
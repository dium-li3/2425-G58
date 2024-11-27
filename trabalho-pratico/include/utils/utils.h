#ifndef UTILS_H
#define UTILS_H

int valid_list(char *lista);

/*
    Dados n tokens, liberta a memoria usada por eles
    e pelo pointer que os guarda.
*/
void free_tokens(char **tokens, int n);

/*
    Calcula a duração em segundos. Só pode ser usada depois de se validar a string.
*/
int calc_duration_s(char *st);

/*
    Converte segundos numa string hh:mm:ss
*/
char *calc_duration_hms(int segs);

/*
    Verifica se uma data de nascimento é válida e está escrita direito.
    Datas são escritas na forma aaaa/mm/dd com
    meses entre 1 e 12 e dias entre 1 e 31
*/
int valid_date (char *date);

//Passa todas as maiusculas de uma string em minúsculas.
void string_to_lower (char *s);

//Verifica se uma dada string é igual a uma de duas dadas.
int same_string (const char *comp, const char *s1, const char *s2);

//Verifica se uma duração é válida e está escrita direito.
int valid_duration (char *duration);

#endif
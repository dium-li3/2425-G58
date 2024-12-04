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

/**
 * @brief Valida uma duração.
 *
 * Verifica se a duração segue o formato hh:mm:ss, com mm e ss <= 59 e hh <= 99.
 *
 * @param durarion String com a duração.
 *
 * @return Booleano (1->válida e 0->inválida).
 */
int valid_duration (char *duration);


/**
 * @brief Calcula a semana de uma dada data decomposta em dia, mês e ano.
 * 
 * A semana calculada é a semana de distância ao dia atual (9/9/2024).
 * 
 * @param d Dia
 * 
 * @param m Mês
 * 
 * @param y Ano
 * 
 * @return Semana
 */
int calc_week(int d, int m, int y);
#endif
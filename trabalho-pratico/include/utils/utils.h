#ifndef UTILS_H
#define UTILS_H

/*
    
*/
/**
 * @brief Valida o campo lista dos CSVs. 
 * 
 * Verifica se um campo de um ficheiro CSV inicia e termina com os caracteres "[" e "]",
 * respectivamente.
 * 
 * @param tokens Tokens a desalocar.
 * @param n Quantidade de tokens que cuja memória libertará espaço.
*/
int valid_list(char *lista);

/**
 * @brief Libera a memória alocada pelos tokens. 
 * 
 * Dados @p n tokens, "dá free" do espaço usado por eles e pelo pointer que os guarda.
 * 
 * @param tokens Tokens a desalocar.
 * @param n Quantidade de tokens que cuja memória libertará espaço.
*/
void free_tokens(char **tokens, int n);

/**
 * @brief Calcula uma duração em segundos. 
 * 
 * Recebe uma contagem em horas, minutos e segundos (hh:mm:ss) e devolve o seu valor
 * convertido em segundos. Só pode ser invocada depois de se validar a entrada.
 * 
 * @param st Valor a se converter em segundos.
 * @result Resultado da conversão.
*/
int calc_duration_s(char *st);

/**
 * @brief Converte um valor medido em segundos numa string "hh:mm:ss". 
 * 
 * Por meio de múltiplas divisões, transforma uma contagem em segundos numa string de formato
 * "hh:mm:ss".
 * 
 * @param segs Valor a ser convertido na string com horas, minutos e segundos.
 * @result Resultado da conversão.
*/
char *calc_duration_hms(int segs);

/**
 * @brief Analisa uma determinada data. 
 * 
 * Verifica se uma data de nascimento é válida e está escrita direito. Datas são escritas na
 * forma "aaaa/mm/dd" com meses entre 1 e 12, e dias entre 1 e 31.
 * 
 * @param date Data a ser inspecionada. 
 * @result Devolve @b 1 se a data for válida; @b 0, se inválida.
*/
int valid_date (char *date);

/**
 * @brief Passa todas as maiúsculas de uma string em minúsculas.
 * 
 * Aplica a função @b tolower a cada carácter da string @p s.
 * 
 * @param s String cujos caracteres serão convertidos em minúsculas.
*/
void string_to_lower (char *s);

/**
 * @brief Verifica se uma dada string é igual a uma de duas dadas.
 * 
 * Testa se a string @p comp é igual a string @p s1 ou igual a string @p s2.
 * 
 * @param comp String que será testada.
 * @param s1 Primeira string usada como modelo de comparação.
 * @param s2 Segunda string usada como modelo de comparação.
 * @result 1 caso @p comp seja igual a @p s1; 2, se @p comp for igual a @p s2.
*/
int same_string (const char *comp, const char *s1, const char *s2);

/**
 * @brief Valida uma duração.
 *
 * Verifica se a duração segue o formato "hh:mm:ss", esperando que os minutos e segundos sejam 
 * menores ou iguais a 59 e as horas menores ou iguais a 99.
 *
 * @param durarion String com a duração.
 * @return 1 caso a duração seja válida; 0, caso seja inválida).
 */
int valid_duration (char *duration);

/**
 * @brief Retorna a quantidade de semanas entre a data fornecida e 09/09/2024.
 * 
 * Calcula a semana do ano em que uma data se encontra, considerando o ponto de referência 
 * 09/09/2024. Além disso, a função ajusta para anos bissextos e aplica algumas correções 
 * específicas baseadas no número de dias entre a data fornecida e a referência em 2024.
 * 
 * @param d Dia da data fornecida.
 * @param m Mês da data fornecida.
 * @param y Ano da data fornecida.
 * @return Número de semanas que a semana de entrada se distancia da data 09/09/2024.
 */
int calc_week(int d, int m, int y);

/**
 * @brief Calcula o índice onde está o maior valor de um array.
 * 
 * Procura em um array o seu elemento com maior valor absoluto, considerando que cada um deles
 * é maior ou igual a zero.
 * 
 * @param array Array cujo índice do máximo se procura.
 * @param N Número de elementos do array.
 * @return Índice do maior elemento do array.
*/
int array_max (int array[], int N);

#endif
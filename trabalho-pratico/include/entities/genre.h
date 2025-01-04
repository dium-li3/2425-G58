#ifndef GENRE_H
#define GENRE_H

#include <glib.h>
#include "output.h"


#define GEN_LEN 151

typedef struct genre *Genre;

/**
 * @brief Inicializa um genêro.
 *
 * Aloca memória para struct gênero, enquanto atribui ao seu campo
 * nomeia como @p gen_name.
 * 
 * @param gen_name String a ser atribuída como nome do gênero.
 * @return Apontador para o novo genêro.
 */
Genre create_gen(const char *gen_name);

/**
 * @brief Libera a memória ocupada por um registo de gênero.
 *
 * "Dá free" dos campos do gênero recebido como parâmetro.
 * 
 * @param gen Apontador para o gênero a ser libertado.
 */
void free_genre(Genre *gen);

/**
 * @brief Compara o nome de um gênero com outro nome, também
 * de um gênero.
 *
 * Acessa o nome de um gênero recebido e o compara com uma string,
 * que nesse caso representa o nome de um gênero.
 * 
 * @param gen Gênero cujo nome será comparado.
 * @param genre String que será usada para a comparação.
 * @return 0 se as strings forem iguais;1, se diferentes.
 */
int compare_genre_names(Genre gen, const char *genre);

/**
 * @brief Função desenvolvida para satisfazer o encapsulamento.
 *
 * Função necessária à componente de encapsulamento. 
 * 
 * @param gen Genêro cujo total de curtidas será devolvido.
 * @return Total de likes do usuário recebido.
 */
int get_genre_total_likes (Genre gen);

/**
 * @brief Incrementa uma unidade numa posição do array dos gêneros.
 *
 * Soma uma unidade à posição @p age do array de curtidas do gênero.
 * Ou seja, contabiliza mais um usuário com @p age anos de idade que
 * curtiu uma música do gênero @p gen.
 * 
 * @param gen Gênero fornecedor do array.
 * @param age posição desse array que será incrementada em uma unidade.
 */
void increment_like(Genre gen, short age);

/**
 * @brief Atualiza um campo interno do género @p gen para ter o valor @p index .
 * 
 * Isto mais tarde permitirá que saibamos a posição deste género @p gen no array de 
 * géneros sem ter de percorrer o array e sim apenas pesquisando por ele
 * diretamente na hashtable.
 * 
 * @param gen Género cujo campo @b index será modificado.
 * @param index Índice do género @p gen no array.
 */
void add_gen_index (Genre gen, int index);

/**
 * @brief Devolve o nome de um dado Género.
 * 
 * @param gen Género portador do nome a devolver.
 * @return Nome do género.
 */
const char* get_genre_name (Genre gen);

/**
 * @brief Realiza a soma prefixada do array de um gênero.
 *
 * Efetua o processo de soma prefixada de um array de curtidas de um
 * gênero dado como parâmetro.
 * 
 * @param gen Gênero cujo array será transformado.
 */
void gen_freq_acum (Genre gen);

/**
 * @brief Calcula o número de curtidas de um determinado gênero, dada uma faixa etária.
 *
 * Após o array de cada gênero sujeitar-se à soma prefixada, a função
 * @b update_gen_total_likes calcula o campo total_likes de um dado gênero, o qual se
 * resume à diferença entre o valor das posições @p max e @p min - 1 (ou somente ao 
 * valor da posição @p max, caso @p min seja igual a zero).
 * 
 * @param gen Gênero que terá o seu campo total_likes calculado.
 * @param min_age Limite inferior da faixa etária.
 * @param max_age Limite superior da faixa etária.
 */
void update_gen_total_likes (Genre gen, int min_age, int max_age);

/**
 * @brief Função usada de base para ordenar o array de gêneros do @b music_manager.
 *
 * É um dos parâmetros que a função da glib @b g_array_sort() precisa para ordenar
 * o array de gêneros do @b music_manager segundo o total de curtidas de cada um.
 * Quando a função for invocada, os parâmetros @p a e @p b serão os dois gêneros a
 * serem comparados.
 * 
 * A função retorna menor que zero se @p b for menor que @p a, zero se for igual,
 * maior que zero se @p b for maior que @p a. É uma versão modificada do tipo
 * qsort()-style, para que o array fique ordenado em ordem decrescente, facilitando
 * a sua exposição posteriormente.
 * 
 * @param a Um g_pointer para um gênero constante.
 * @param b Outro gênero que será comparado com @p a.
 * @return Resultado da comparação.
 */
int cmp_like_gen(gconstpointer g1, gconstpointer g2);

/**
 * @brief Expõe as informações de um dado gênero.
 *
 * "Dá print" dos dados de um gênero especificado como parâmetro, dados esses
 * separados por um caractér guardado no output.
 * 
 * @param gen Gênero o qual terá as informações reveladas.
 * @param out Struct usado pelo output.
 * @return Número total de curtidas de @p gen.
 */
int print_genre_info(Genre gen, Output out);

#endif
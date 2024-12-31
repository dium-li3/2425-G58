#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <glib.h>

typedef struct parser *Parser;

/**
 * @brief Cria um parser que guarda o ficheiro com um caminho fornecido.
 * 
 * Cria uma estrutura parser que guarda o ficheiro com o dado @p path aberto.
 * 
 * @param path string com o path para um ficheiro.
 * @return Pointer para a struct parser criada.
*/
Parser open_parser(const char *path);

/**
 * @brief Fecha o do parser usa e liberta a memória da estrutura.
 * 
 * Fecha o ficheiro que o parser usa e liberta a memória da estrutura.
 * 
 * @param p Pointer para a estrutura sobre a qual se aplica a função.
*/
void close_parser(Parser p);

/**
 * @brief Devolve o número de bits que foram lidos da última vez que se acedeu ao Parser.
 * 
 * Devolve o número de bits que foram lidos da última vez que se acedeu ao ficheiro aberto pelo Parser.
 * 
 * @param p Estrutura que se mantém a par de um ficheiro aberto.
 * @return Número de bits lidos. 
*/
ssize_t get_nRead (Parser p);

/**
 * @brief Devole a posição no ficheiro.
 * 
 * Retorna a posição atual no ficheiro.
 * 
 * @param p Parser.
 * @return Posição no ficheiro. 
 */
long get_file_pos(Parser p);

/**
 * @brief Muda a posição no Parser.
 * 
 * Muda a posição no ficheiro de @p p.
 * 
 * @param p Parser cuja posição deve ser modificada.
 * @param new_fp Nova posição a definir.
 */
void set_file_pos(Parser p, long new_fp);

/**
 * @brief Volta uma linha no ficheiro que do parser.
 * 
 * Retorna uma linha no ficheiro que o parser tem aberto.

 * @param p Estrutura que se mantém a par de um ficheiro aberto. 
*/
void go_back_1line (Parser p);

/**
 * @brief Guarda os valores de uma string de uma lista num array da glib.
 * 
 * Percorre a string e separa-a em tokens, sendo cada token um elemento
 * da lista que a string representa. Como queremos esses elementos como inteiros
 * eles são transformados em inteiros e só depois é que são guardados num array
 * da glib.
 * 
 * @param line Array do qual se vão converter os elementos para int.
 * @return GArray com os valores da @p line guardados como inteiros.
*/
GArray *store_list (char *line);

/**
 * @brief Separa uma linha em tokens.
 * 
 * Divide uma linha em @p n_elems tokens.
 * 
 * @param p Estrutura que se mantém a par de um ficheiro aberto.
 * @param n_elems Número de elementos no qual dividir a linha lida.
 * @return Pointer para strings que contém @p n_elems string.
*/
char **parse_line (Parser p, int n_elems);

/**
 * @brief Lê uma linha do Parser @p p e separa-a em tokens com base no seu 1º caracter.
 * 
 * Este parser é próprio de queries, então para diferentes 1ºs caracteres,
 * que designam o tipo de query, prepara-se de antemão para dividir
 * a linha num certo número de tokens, com certas peculiaridades, pois,
 * por exemplo na query 2, a country dada pode ter espaços no nome, coisa
 * única dela que pede por um caso particular.
 * 
 * @param p Estrutura que se mantém a par de um ficheiro aberto.
 * @param infos Pointer para strings com espaço para contar as divisões feitas na linha lida.
 * @param n_tokens Número de tokens máximo no qual se consegue dividir a linha e guardar a informação.
 * @return Número de bits lidos na linha do ficheiro.
*/
int parse_1line_query(Parser p, char **info, int n_tokens);

/**
 * @brief Guarda 1 linha do ficheiro aberto pelo @p p na @p line dada.
 * 
 * Permite verificar se foi lida alguma coisa consultando o nRead do @p p
 * através da função get_nRead.
 * 
 * @param p Estrutura que se mantém a par de um ficheiro aberto.
 * @param line Pointer para o array no qual a linha lida vai ser escrita.
*/
void parse_1line (Parser p, char **line);

/**
 * @brief Lê a data escrita numa string e devolve quanto anos ela tem do presente.
 * 
 * Calcula a idade de uma data considerando que a 
 * data atual é 09/09/2024 para fazer os seus cálculos.
 * 
 * @param bd Data escrita em string.
 * @return Idade convertida da string, em short.
*/
short read_date_to_age (char *bd);

/**
 * @brief Lê a timestamp escrita numa string e guarda os elementos 
 * da data em variáveis dadas e devolve a hora que a data.
 * 
 * Lê uma timestamp que está escrita como yyyy/mm/dd hh:mm:ss
 * e guarda os 3 elementos da data e devolve o primeiro elemento
 * da duração.
 * 
 * @param str timestamp que contém uma data seguida de uma duração
 * @param year Variável que irá conter o primeiro elemento da data.
 * @param month Variável que irá conter o segundo elemento da data.
 * @param day Variável que irá conter o último elemento da data.
 * 
 * @return Primeiro elemento da duração.
 */
int read_timestamp_elements (char *str, int *year, int *month, int *day);

/**
 * @brief Dado um caminho @p path para a pasta dos csv, cria as strings com o path para cada csv individual.
 * 
 * Sabendo o caminho @p path para a pasta de csv, simplesmente adiciona o nome do csv à pasta,
 * para cada csv. Como há 5 csv diferentes, o char ** resultante tem tamanho 5,
 * e cada posição dele aponta para a string com o path para um csv.
 * 
 * @param path caminho para a pasta dos csv.
 * @return Array com os paths para os ficheiros csv em específico.
*/
char **path3Entities (char *path);

/**
 * @brief Liberta o espaço usado pelo array de paths.
 * 
 * Dá free o espaço alocado pelo array @p fp_entities.
 * 
 * @param fp_entitiesss Pointer para o array de paths.
*/
void freeEntityPaths (char **fp_entities);

#endif
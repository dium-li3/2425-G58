#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>

#include "output.h"
#include "parser.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "history_manager.h"
#include "testagem.h"

#define QUERYTYPES 6

typedef struct query_stats *Query_stats;

typedef struct query *Query;

typedef struct query1 *Query1;

typedef struct query2 *Query2;

typedef struct query3 *Query3;

typedef struct query4 *Query4;

typedef struct query5 *Query5;

typedef struct query6 *Query6;

/**
 * @brief Aloca espaço para um struct query.
 * 
 * Aloca espaço na memória para as 6 queries, inicializando-as vazias a princípio.
 * O campo @b separador é inicialmente definido como o carácter ' '.
 * 
 * @return Struct query recém-criado.
*/
Query create_query();

/**
 * @brief Devolve o número (tipo) da query a ser respondida.
 * 
 * Acessa o campo @b query do struct @p q, que representa o número da query (de um
 * a 6) que se pretende responder. O valor no campo acedido é então retornado.
 * 
 * @param q Struct cujo número será devolvido.
 * @return Número da query @p q.
*/
short get_query_type(Query q);

/**
 * @brief Retorna o caractér a ser usado como separador no output de uma dada query.
 * 
 * Acessa o campo @b separador do struct @p q, caracter que separa os membros do output,
 * caso o tipo da query tenha um S. O valor no campo acedido é então retornado.
 * 
 * @param q Struct cujo separador será devolvido.
 * @return Struct query recém-criado.
*/
char get_separador (Query q);

/**
 * @brief Guarda a informação de uma linha de queries numa query.
 * 
 * Recebe uma linha do ficheiro de input e atribui o seu conteúdo a um struct query @p q,
 * definindo o seu separador mediante o comprimento do primeiro token, e extraindo o seu 
 * tipo para que seja possível invocar o inicializador de queries apropriado, o qual passará
 * as restantes informações da linha para um dos substructs de @p q (Query1, Query1, Query2,
 * Query3, Query4, Query5 ou Query6). Os tokens alocados no final da função têm sua memória
 * libertada.
 * 
 * @param pq Estrutura que se mantém a par de um ficheiro aberto.
 * @param q Struct que armazenará as informações da linha.
*/
void read_query_line(Parser pq, Query q);

/**
 * @brief Libera o espaço alocado por uma query.
 * 
 * Dá free da query @p q e de todos os seus substructs, invocando funções de free especias para
 * os campos @b query2 e @b query5, que possui strings nos campos destas e por isso requerem um
 * cuidado especial.
 * 
 * @param q Query a ser libertada.
*/
void free_query (Query q);

/**
 * @brief Responde a query 1.
 * 
 * Verifica se o tipo do subcampo @b query1 de @p q é do tipo @b U/A, para então escrever no
 * ficheiro output as informações de um usuário/artista dado o seu id. Além disso, a função cronometra o
 * seu tempo de execução.
 * 
 * @param q Estrutura que fornece as informações para a resolução da query.
 * @param um Gestor detentor do usuário o qual se deseja expor os seus dados, caso o tipo da query
 * seja @b U.
 * @param am Gestor detentor do artista o qual se deseja expor os seus dados, caso o tipo da query
 * seja @b A.
 * @param out Ficheiro onde as informações são escritas.
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
 * 
*/
void answer1(Query q, User_Manager um, Art_Manager am, Output out, Query_stats qs);

/**
 * @brief Responde a query 2.
 * 
 * Verifica se o subcampo @b country de @p q é nulo, onde nesse caso escreve os primeiros N artistas do array
 * ordenado por discografia de artistas. No caso do subcampo não ser igual a @b NULL, expõe somente os artistas
 * do país especificado. Além disso, a função cronometra o seu tempo de execução.
 * 
 * @param q Estrutura que fornece as informações para a resolução da query.
 * @param am Gestor portador do array ordenado.
 * @param out Ficheiro onde as informações são escritas.
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
*/
void answer2(Query q, Art_Manager am, Output out, Query_stats qs);

/**
 * @brief Responde a query 3.
 * 
 * Preenche o campo @b total_likes de cada gênero presente no gestor @p mm dada uma faixa etária, e ordena o 
 * array de gêneros desse gestor, expondo os elementos e o número de curtidas destes. Além disso, a função
 * cronometra o seu tempo de execução.
 * 
 * @param q Estrutura que fornece as informações para a resolução da query.
 * @param mm Gestor portador do array de gêneros.
 * @param out Ficheiro onde as informações são escritas.
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
*/
void answer3(Query q, Music_Manager mm, Output out, Query_stats qs);

void answer4(Query q, Output out, Query_stats qs, Art_Manager am);

/**
 * @brief Responde a query 5.
 * 
 * Primeiramente verifica se o id do usuário fornecido para a query 5 existe. Em seguida, começa a obter cada
 * parâmetro exigido pela função @b recomendaUtilizadores a partir da aplicação de getters aos gestores @p um,
 * @p mm e @p hm, posteriormente chamando essa função. Além disso, essa answer cronometra o seu tempo de execução.
 * 
 * @param q Estrutura que fornece as informações para a resolução da query.
 * @param um Gestor portador dos identificadores dos usuários assim como a sua quantidade total.
 * @param mm Gestor portador dos nomes dos gêneros assim como a sua quantidade total.
 * @param hm Gestor portador da matriz que contém o número de reproduções de cada gênero por cada usuário.
 * @param out Ficheiro onde as informações são escritas.
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
*/
void answer5(Query q, User_Manager um, Music_Manager mm,History_Manager hm, Output out, Query_stats qs);

void answer6(Query q, Art_Manager am, Music_Manager mm, User_Manager um, History_Manager hm, Output out, Query_stats qs);

/**
 * @brief Inicializa o status das queries.
 * 
 * Aloca espaço para as contagens referentes as queries, inicialmente marcadas como nulas.
 * 
 * @return Status das queries recém-criado.
*/
Query_stats create_query_stats();

/**
 * @brief Incrementa o número de execuções e o tempo de execução total de uma determinada query.
 * 
 * Dada uma query de número @p type, aumenta em uma unidade e em @p time unidades o seu número total de execuções e
 * o seu tempo total de execução, respectivamente.
 * 
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
 * @param time Tempo a incrementar no tempo total de execução.
 * @param type Número da query a ser modificado. 
*/
void add_query_stats(Query_stats qs, double time, int type);

/**
 * @brief Retorna o tempo total de execução de uma query.
 * 
 * Devolve o tempo total de execução da query número @p i.
 * 
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
 * @param i Número da query que se deseja o tempo de execução.
 * @return Tempo total de execução. 
*/
double get_query_stats_time(Query_stats qs, int i);

/**
 * @brief Retorna o número total de execuções de uma query.
 * 
 * Devolve o número total de execuções da query número @p i.
 * 
 * @param qs Estrutura que armazena o número de execuções de cada query e o tempo total das suas execuções.
 * @param i Número da query que se deseja o número de execuções.
 * @return Número total de execuções. 
*/
int get_query_stats_n(Query_stats qs, int i);

#endif
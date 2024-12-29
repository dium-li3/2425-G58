#ifndef RECOMENDADOR_XPTO_H
#define RECOMENDADOR_XPTO_H

/**
 * @brief Recomenda os @p numRecomendacoes utilizadores semelhantes ao utilizador pedido.
 * 
 * Esta função olha para o quanto o @p idUtilizadorAlvo ouviu cada género de música e
 * torna esses valores em frequências relativas.
 * De seguida calcula e compara as frequências relativas de todos os outros utilizadores
 * e associa a cada utilizador um valor de 0 a 1, dependendo do quão próximos os seus gostos sejam
 * segundo o alguritmo usado.
 * Depois de fazer estes calculos e comparações para todos os utilizadores, executa @p numRecomendacoes
 * bubbleUps de forma a colocar nas primeiras @p numRecomendacoes posições os utilizadores que
 * foram considerados mais semelhantes (valor associado mais alto) face ao nosso @p idUtilizadorAlvo 
 * e devolve-os.
 * 
 * @param idUtilizadorAlvo Id do utilizador para o qual vamos gerar recomendações
 * @param matrizClassificacaoMusicas Matriz com @p numUtilizadores linhas e @p numGeneros colunas, 
 * na qual cada linha contém os números de vezes que um utilizador ouviu músicas de cada género.
 * @param idsUtilizadores Array com os ids dos utilizadores pela ordem que aparecem na matriz
 * @param nomesGeneros Array com os nomes dos géneros pela ordem que aparecem na matriz
 * @param numUtilizadores Número total de utilizadores
 * @param numGeneros Número total de géneros de música
 * @param numRecomendacoes Número de recomendações pedidas
 * 
 * @return Array de ids de utilizadores a recomendar, com @p numRecomendacoes elementos 
 */
char **recomendaUtilizadores_xpto(char *idUtilizadorAlvo, int **matrizClassificacaoMusicas, char **idsUtilizadores, char **nomesGeneros, int numUtilizadores, int numGeneros, int numRecomendacoes);

#endif
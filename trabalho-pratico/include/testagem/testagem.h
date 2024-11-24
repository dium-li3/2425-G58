#ifndef TESTAGEM_H
#define TESTAGEM_H

/*
    @brief Faz a validação dos resultados.

    Valida os resultados obtidos após a execução do programa, comparando os ficheiros criados com os esperados.
    É feita uma comparação linha a linha e é impresso para o terminal o número da Query onde houve discrepâncias
    e o(s) número(s) da(s) linha(s) em concreto onde ocorreram.

    @param expected Caminho para a pasta que contém os resultados esperados.
*/
void testagem(char *expected);

#endif
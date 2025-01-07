#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "heaps.h"

typedef struct tuplo {
    char *id;
    float valorizacao;
} *Tuplo_Rec;

//Copia a informação da matriz para um array e devolve o número total de vezes que ouviu qualquer género.
int atualiza_valorizacao_genero (float valorizacao[], int **matriz, int alvo, int numGeneros){
    int i, total = 0;
    for (i = 0; i < numGeneros; i++){
        valorizacao[i] = (float) matriz[alvo][i];
        total += matriz[alvo][i];
    }
    return total;
}


int compare_tuplos(void *a, void *b, void *data) {
    Tuplo_Rec aa = (Tuplo_Rec) a;
    Tuplo_Rec bb = (Tuplo_Rec) b;

    if(aa->valorizacao < bb->valorizacao) return 1;
    else return 0;
}

//Ordena o array de forma a no inicio estarem os primeiros 'top' maiores elementos.
Tuplo_Rec* top_n_tuplos_ordenados (Tuplo_Rec *valorizacao_user, int N, int top){
    /*int i, j;
    Tuplo_Rec t;
    for (i = 0; i < N && top > 0; i++, top--)
        for (j = N - i - 1; j > i; j--)
            if (valorizacao_user[j]->valorizacao > valorizacao_user[j-1]->valorizacao){
                t = valorizacao_user[j];
                valorizacao_user[j] = valorizacao_user[j-1];
                valorizacao_user[j-1] = t;
            }*/
    
    int i;
    Heap h = heap_new(top, compare_tuplos, NULL, NULL);

    for(i = 0; i < top; i++) heap_add(h, valorizacao_user[i]);
    for(; i < N; i++) heap_swap_fst_elem(h, valorizacao_user[i]);

    Tuplo_Rec *maiores = (Tuplo_Rec*) heap_unwrap_array(h, &top);
    return maiores;
}

char **recomendaUtilizadores_xpto(char *idUtilizadorAlvo, int **matrizClassificacaoMusicas, char **idsUtilizadores, char **nomesGeneros, int numUtilizadores, int numGeneros, int numRecomendacoes){
    float *valorizacao_genero_alvo = calloc (numGeneros, sizeof (float));
    float *valorizacao_genero_rec = calloc (numGeneros, sizeof (float));
    int alvo, i, j;
    //encontrar o user alvo e guardar a sua posicao em 'alvo'
    for (alvo = 0; alvo < numUtilizadores && strcmp(idUtilizadorAlvo, idsUtilizadores[alvo]) != 0; alvo++);

    //preencher as suas audições no array (freq abs)
    int N_alvo = atualiza_valorizacao_genero (valorizacao_genero_alvo, matrizClassificacaoMusicas, alvo, numGeneros);
    
    // printf ("valores do array de generos: ");
    // for (i = 0; i < numGeneros; i++)
    //     printf ("|%0.0f", valorizacao_genero_alvo[i]);
    // printf ("|\n");

    //tornar o seu array de audições (freq abs) num array de percentagens/valorização (freq rel) de audições
    if (N_alvo > 0)
        for (i = 0; i < numGeneros; i++)
            valorizacao_genero_alvo[i] = (float) valorizacao_genero_alvo[i] / N_alvo;
    
    // printf ("valores do array de generos: ");
    // for (i = 0; i < numGeneros; i++)
    //     printf ("|%0.2f", valorizacao_genero_alvo[i]);
    // printf ("|\n");

    //criar uma lista com as valorizações de cada user para comparar com o nosso alvo e associar um valor a cada user
    Tuplo_Rec *valorizacao_user = calloc (numUtilizadores - 1, sizeof (Tuplo_Rec));
    int index, N_rec;
    for (i = 0, index = 0; i < numUtilizadores; i++)
        if (i != alvo){
            valorizacao_user[index] = calloc (1, sizeof (struct tuplo));
            //preenche as audições do user 'i' no array 
            N_rec = atualiza_valorizacao_genero (valorizacao_genero_rec, matrizClassificacaoMusicas, i, numGeneros);
            //transforma as freq absolutas do user 'i' em freq relativas, com max = freq relativa no alvo para cada género
            if (N_rec > 0)
                for (j = 0; j < numGeneros; j++){
                    valorizacao_genero_rec[j] = (float) valorizacao_genero_rec[j] / N_rec;
                    if (valorizacao_genero_rec[j] > valorizacao_genero_alvo[j])
                        //Aqui há 2 opções semelhantes
                        //valorizacao_genero_rec[j] -= 2*(valorizacao_genero_rec[j] - valorizacao_genero_alvo[j]);
                        valorizacao_genero_rec[j] = valorizacao_genero_alvo[j];
                    valorizacao_user[index]->valorizacao += valorizacao_genero_rec[j];
                }
            valorizacao_user[index]->id = idsUtilizadores[i];
            //Se nem o alvo nem o user i têm preferências, têm preferências iguais
            if (N_alvo == 0 && N_rec == 0)
                valorizacao_user[index]->valorizacao = 1;
            index++;
        }

    //Encontra e guarda o 'numRecomendacoes' de recomendações pedidas
    char **recomendacoes = calloc (numRecomendacoes, sizeof (char*));
    //ordenamos o array de forma a termos no inicio em ordem decrescente o 'numRecomendacoes' de users a recomendar
    Tuplo_Rec *maiores = top_n_tuplos_ordenados (valorizacao_user, numUtilizadores-1, numRecomendacoes);
    
    //passamos os 'numRecomendacoes' primeiros elementos do array para a lista de array que vamos devolver
    for (i = 0; i < numRecomendacoes; i++)
        recomendacoes[i] = maiores[i]->id;

    free (valorizacao_genero_alvo);
    free (valorizacao_genero_rec);
    for (i = 0; i < numUtilizadores - 1; i++)
        free (valorizacao_user[i]);
    free (valorizacao_user);
    free(maiores);
    return recomendacoes;
}

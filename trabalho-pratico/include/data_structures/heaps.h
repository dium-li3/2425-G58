#ifndef HEAPS_H
#define HEAPS_H

typedef struct heap *Heap;

/**
 * @brief Aloca espaço para uma nova heap, com capacidade para @p n elementos de tamanho @p s .
 * 
 * A função comp será utilizada para organizar a heap, portanto também determina se esta última será uma max ou min heap.
 * a<b e compare(a,b)==TRUE -> min-heap
 * a>b e compare(a,b)==TRUE -> max-heap
 *
 * @param n Número de elementos a pré alocar.
 * @param s Tamanho em bytes de cada elemento.
 * @param comp Função de comparação.
 * @param free_elem Deve libertar um elemento da heap. Caso não se pretenda libertar os elementos individuais
 *                  aquando da libertação da heap, este campo deve ser NULL.
 * 
 * @return Apontador para a Heap criada.
 */
Heap heap_new(size_t n, size_t s, int (*comp)(void *a, void *b), void (*free_elem)(void *a));


/**
 * @brief Imprime uma heap.
 * 
 * Imprime @p h para o terminal e indica se é uma heap válida ou não
 * (com base na função de comparação passada na criação).
 * 
 * @param h Heap a ser imprimida.
 * @param print Deve imprimir apenas o elemento que lhe é passado.
 */
void heap_print(Heap h, void (*print)(void *x));


/**
 * @brief Puxa o elemento que está na posição @p i da heap até à sua posição correta.
 * 
 * @param i Índice do elemento que deve ser puxado.
 * @param h Heap.
 */
void heap_bubbleUp(int i, Heap h);


/**
 * @brief Empura o elemento que está na posição @p i da h até à sua posição correta.
 * 
 * @param i Índice do elemento que deve ser empurrado.
 * @param h Heap.
 */
void heap_bubbleDown(int i, Heap h);


/**
 * @brief Adiciona @p x à heap.
 * 
 * O novo elemento é colocado no final da Heap, sendo depois executado um @c heap_bubbleUp para manter a ordem da Heap.
 * Caso a heap esteja cheia, realoca o array, multiplicando o tamanho máximo por 2.
 * 
 * @param h Heap.
 * @param x Elemento a adicionar.
 * 
 * @return Código de sucesso (1 se a inserção falhou ou 0 se a inserção funcionou).
 */
int heap_add(Heap h, void *x);


/**
 * @brief Remove o elemento no índice @p i da Heap.
 * 
 * Coloca em @p rem o valor do índice @p i da heap.
 * De seguida, o último elemento é colocado na posição @p i e o tamanho é decrementado.
 * Por fim, é feito um @c bubbleDown para colocar o elemento que passou para a posição @p i no sítio certo.
 * Obs: a função deverá ser chamada da seguinte forma: @c heap_remove(h,i,&r) , em que r é um void*
 * definido onde a invocação é feita.
 * 
 * @param h Heap.
 * @param i Índice do elemento a remover.
 * @param rem Endereço onde deve ser colocado o elemento removido.
 * 
 * @return Código de sucesso (1 se @p i é fora da Heap ou 0 se é um índice válido).
 */
int heap_remove (Heap h, int i, void **rem);


/**
 * @brief Substitui o 1º elemento de @p h .
 * 
 * Troca o 1º elemento da Heap com @p new e faz um bubbleDown, de forma a manter a ordenação de @p h .
 * O objetivo desta função é ser usada no algoritmo que determina os k (tamanho da heap) maior elementos de um array.
 * 
 * @param h Heap.
 * @param new Novo elemento.
 */
void heap_swap_fst_elem(Heap h, void *new);


/**
 * @brief Liberta a memória usada por uma Heap.
 * 
 * Liberta também os elementos individuais caso, na criação, tenha sido passada uma função para isso.
 * 
 * @param h Heap a ser libertada.
 */
void heap_free(Heap h);


/*
//Organiza v de forma a ser uma min-heap, usando o bubbleDown.
void heapify(int *v, int N);

//Organiza v de forma a ser uma min-heap, usando o bubbleUp.
void heapify_slow(int *v, int N);

//Transforma v num array ordenado por ordem decresecente.
void heapSort(int *v, int N);
*/
#endif

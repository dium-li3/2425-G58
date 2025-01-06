/**
 * @file heaps.h
 * @author Marco
 * @date Nov 2024
 * @brief Header file das heaps
 */
#ifndef HEAPS_H
#define HEAPS_H

typedef struct heap *Heap;

/**
 * @brief Aloca espaço para uma nova heap, com capacidade para @p n elementos (void pointers).
 * 
 * A função comp será utilizada para organizar a heap, portanto também determina se esta última será uma max ou min heap.
 * a<b e compare(a,b)==TRUE -> min-heap
 * a>b e compare(a,b)==TRUE -> max-heap
 * O parámetro @c data da função (que será alimentado com o @p data passado como argumento)
 * serve para o utilizador poder passar dados necessários à comparação.
 *
 * @param n Número de elementos a pré alocar.
 * @param comp Função de comparação.
 * @param free_elem Deve libertar um elemento da heap. Caso não se pretenda libertar os elementos individuais
 * aquando da libertação da heap, este campo deve ser NULL.
 * @param data Dados úteis a definir pelo utilizador.
 * @return Apontador para a Heap criada.
 */
Heap heap_new(size_t n, int (*comp)(void *a, void *b, void *data), void (*free_elem)(void *a), void *data);

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
 * @brief Atualiza o campo @b data da heap.
 * 
 * Atribui ao campo @b data de @p h o valor @p new_data.
 * 
 * @param h Heap cujo o campo será modificado.
 * @param new_data Novos dados a inserir.
 */
void heap_set_data(Heap h, void *new_data);

/**
 * @brief Eleva um elemento da heap até que esta satisfaça a sua definição.
 * 
 * Puxa o elemento que está na posição @p i de @p h até à sua posição correta.
 * 
 * @param i Índice do elemento que deve ser puxado.
 * @param h Heap.
 */
void heap_bubbleUp(int i, Heap h);

/**
 * @brief Decai um elemento da heap até que esta satisfaça a sua definição.
 * 
 * Empura o elemento que está na posição @p i de @p h até à sua posição correta.
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
 * @return Código de sucesso (1 se a inserção falhou ou 0 se a inserção funcionou).
 */
int heap_add(Heap h, void *x);

/**
 * @brief Remove o 1º elemento da Heap.
 * 
 * Coloca em @p rem o 1º elemento.
 * De seguida, o último elemento é colocado na posição 0 e o tamanho é decrementado.
 * Por fim, é feito um @c bubbleDown para colocar o elemento que passou para a posição 0 no sítio certo.
 * Obs: a função deverá ser chamada da seguinte forma: @c heap_remove(h,&r) , em que r é um void*
 * definido onde a invocação é feita.
 * 
 * @param h Heap.
 * @param rem Endereço onde deve ser colocado o elemento removido.
 * @return Código de sucesso (1 se a Heap está vazia ou 0 se foi possível remover).
 */
int heap_remove (Heap h, void **rem);

/**
 * @brief Substitui o 1º elemento de @p h .
 * 
 * Caso @p new seja maior que o 1º elemento da heap (caso seja uma min-heap; se for uma max-heap, é caso
 * seja menor), troca os dois e faz um bubbleDown, de forma a manter a ordenação de @p h .
 * O objetivo desta função é ser usada no algoritmo que determina os k (tamanho da heap) maior elementos
 * de um array (max-heap -> k menores).
 * 
 * @param h Heap.
 * @param new Novo elemento.
 */
void heap_swap_fst_elem(Heap h, void *new);

/**
 * @brief "Desembrulha" a Heap, devolvendo o array.
 * 
 * Liberta a memória alocada para @p h e retorna o array usado, devendo este ser libertado posteriormente.
 * Também coloca em size o tamanho do array.
 * 
 * @param h Heap a desembrulhar.
 * @param size Apontador para colocar o tamanho.
 * @return Array da Heap.
 */
void** heap_unwrap_array(Heap h, int *size);

/**
 * @brief Liberta a memória usada por uma Heap.
 * 
 * Liberta também os elementos individuais caso, na criação, tenha sido passada uma função para isso.
 * 
 * @param h Heap a ser libertada.
 */
void heap_free(Heap h);

#endif
#include <stdio.h>
#include <stdlib.h>
#include "heaps.h"

#define pf(i) ((i<<1) + 1) //calcula o índice do primeiro filho/descendente da esquerda (2*i + 1)
#define sf(i) ((i<<1) + 2) //calcula o índice do segundo filho/descendente da direita (2*i + 2)
#define pai(i) ((i-1)>>1)  //calcula o índice do antecedente/pai ((i-1) / 2)

typedef struct heap {
    void **heap;
    int size;
    int max;
    int (*compare)(void*, void*, void*);
    void (*free_elem)(void*);
    void *data;
} *Heap;



Heap heap_new(size_t n, int (*comp)(void *a, void *b, void *data), void (*free)(void *a), void *data){
    Heap h = calloc(1, sizeof(struct heap));

    if(h != NULL) {
        h->heap = calloc(n, sizeof(void*));
        h->max = n;
        h->compare = comp;
        h->free_elem = free;
        h->data = data;
    }

    return h;
}


int valid_heap(Heap h){
    int i, r = 1, N = h->size;

    for(i = 0; i < N/2 && pf(i) < N && r; i++) {
        r = h->compare(h->heap[i], h->heap[pf(i)], h->data);
        
        if(sf(i) < N && r) r = h->compare(h->heap[i], h->heap[sf(i)], h->data);
    }

    return r;
}


void swap(void **a, int i, int j){
    void *t = a[i];
    a[i] = a[j];
    a[j] = t;
}


void heap_print(Heap h, void (*print)(void *x)){
    putchar('[');
    
    int i, s = h->size-1;
    
    for(i = 0; i < s; i++) {
        print(h->heap[i]);
        printf(", ");
    }
    print(h->heap[i]);
    printf("]\n");

    (valid_heap(h))? printf("(válida)\n\n") : printf("(inválida)\n\n");
}



void heap_set_data(Heap h, void *new_data){
    h->data = new_data;
}


void heap_bubbleUp(int i, Heap h) {
    while(i > 0 && h->compare(h->heap[i], h->heap[pai(i)], h->data)) {
        swap(h->heap, i, pai(i));
        i = pai(i);
    }
}


void heap_bubbleDown(int i, Heap h) {
    int f, N = h->size;

    while((f = pf(i)) < N) {
        if(f+1 < N && h->compare(h->heap[f+1], h->heap[f], h->data)) f++;
        if(h->compare(h->heap[i], h->heap[f], h->data)) break;

        swap(h->heap, i, f);
        i = f;
    }
}


int heap_add(Heap h, void *x){
    if(h->size == h->max) {
        h->heap = realloc(h->heap, h->max*2*sizeof(void*));
        if(h->heap == NULL) return 1;
        h->max *= 2;
    }

    h->heap[h->size] = x;
    heap_bubbleUp(h->size, h);
    h->size++;

    return 0;
}


int heap_remove (Heap h, void **rem) {
    if(h->size == 0) return 1;

    *rem = h->heap[0];
    h->heap[0] = h->heap[--h->size];
    heap_bubbleDown(0, h);

    return 0;
}


void heap_swap_fst_elem(Heap h, void *new){
    if(h->compare(h->heap[0], new, h->data) != 0){
        h->heap[0] = new;
        heap_bubbleDown(0, h);
    }
}


void** heap_unwrap_array(Heap h, int *size){
    void **array = h->heap;
    *size = h->size;

    free(h);
    
    return array;
}


void heap_free(Heap h){
    int size = h->size;
    
    if(h->free_elem != NULL)
        for(int i = 0; i < size; i++) h->free_elem(h->heap[i]);
    
    free(h->heap);
    free(h);
}
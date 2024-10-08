#ifndef HASH_HELPER_H
#define HASH_HELPER_H

/*
    Cria uma hash table para guardar dados por id.
*/
GHashTable *create_hash_by_id();

/*
    Liberta o espaço gasto pela hash table.
*/
void destroy_hash_by_id(GHash_table *ht);

#endif
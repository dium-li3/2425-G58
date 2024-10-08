#include <glib.h>

#include "hash_helper.h"

/*
    Cria uma hash table para guardar dados por id.
*/
GHashTable *create_hash_by_id(){
    g_hash_table_new (g_int_hash(), g_int_equal())
};
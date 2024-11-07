#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <stdio.h>
#include "parser.h"
#include "queries.h"


typedef struct entity_manager *Entity_Manager;

Entity_Manager create_entity_manager();

void store_Entities (char **entity_paths, Entity_Manager entity_M);

void answer_all_queries (Parser queries, Entity_Manager em);

void answer_all_queries_test (Parser queries, Entity_Manager em, Query_stats qs);

void free_entity_manager (Entity_Manager e);

#endif
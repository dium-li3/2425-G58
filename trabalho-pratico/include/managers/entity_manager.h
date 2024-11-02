#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <stdio.h>
#include "user_manager.h"
#include "users.h"
#include "parser.h"
#include "sintatica.h"
#include "queries.h"


typedef struct entity_manager *Entity_Manager;

Entity_Manager create_entity_manager();

void store_Entities (char **entity_paths, Entity_Manager entity_M);

void answer_querie (Querie q, Entity_Manager em, int type, int n_querie);

void answer_all_queries (Parser queries, Entity_Manager em);

void free_entity_manager (Entity_Manager e);

#endif
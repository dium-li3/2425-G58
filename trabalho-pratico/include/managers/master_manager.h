#ifndef MASTER_MANAGER_H
#define MASTER_MANAGER_H
#include <stdio.h>
#include "parser.h"
#include "queries.h"


typedef struct master_manager *Master_Manager;

Master_Manager create_master_manager();

void store_Entities (char **entity_paths, Master_Manager master_M);

void answer_all_queries(Parser queries, Master_Manager mm, Query_stats qs);

void free_master_manager (Master_Manager m);

#endif
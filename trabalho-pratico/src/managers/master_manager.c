#include <stdio.h>
#include <time.h>

#include "master_manager.h"
#include "artists.h"
#include "musics.h"
#include "users.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "user_manager.h"
#include "queries.h"
#include "parser.h"

typedef struct master_manager{
    Art_Manager artist_M;
    Music_Manager music_M;
    User_Manager user_M;
} *Master_manager;

Master_Manager create_master_manager(){
    Master_Manager e = calloc(1, sizeof(struct master_manager));
    e->user_M = create_user_manager();
    e->music_M = create_music_manager();
    e->artist_M = create_art_manager();
    return e;
}

void store_Entities(char **entity_paths, Master_Manager entity_M){
    store_Artists(entity_paths[2], entity_M->artist_M);
    store_Musics(entity_paths[1], entity_M->music_M, entity_M->artist_M);
    order_duration (entity_M->artist_M);
    store_Users(entity_paths[0], entity_M->user_M, entity_M->music_M);
}



void answer_querie(Querie q, Master_Manager mm, int n_querie, Query_stats qs){
    short type = get_querie_type(q);
    if (type > 0){
        char output_file[34];
        snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
        Output out = open_out (output_file);
        switch (type)
        {
        case (1):
            answer1(q, mm->user_M, out, qs);
            break;
        case (2):
            answer2(q, mm->artist_M, out, qs);
            break;
        case (3):
            answer3(q, mm->music_M, out, qs);
            break;
        }
        close_output (out);
    }
}

void answer_all_queries(Parser queries, Master_Manager mm, Query_stats qs){
    int i;
    Querie q = create_querie();
    for (i = 1; get_querie_type(q) != -1; i++)
    {
        read_querie_line(queries, q);
        answer_querie(q, mm, i, qs);
    }
    free_querie(q);
}



void free_master_manager(Master_Manager m){
    free_user_manager(m->user_M);
    free_music_manager(m->music_M);
    free_art_manager(m->artist_M);
    free(m);
}

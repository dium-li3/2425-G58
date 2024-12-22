#include <stdio.h>
#include <time.h>

#include "master_manager.h"
#include "artists.h"
#include "musics.h"
#include "users.h"
#include "artist_manager.h"
#include "album_manager.h"
#include "music_manager.h"
#include "user_manager.h"
#include "history_manager.h"
#include "queries.h"
#include "parser.h"

typedef struct master_manager{
    Art_Manager artist_M;
    Album_Manager album_M;
    Music_Manager music_M;
    User_Manager user_M;
    History_Manager hist_M;
} *Master_manager;

Master_Manager create_master_manager(){
    Master_Manager e = calloc(1, sizeof(struct master_manager));
    e->user_M = create_user_manager();
    e->album_M = create_album_manager();
    e->music_M = create_music_manager();
    e->artist_M = create_art_manager();
    e->hist_M = create_history_manager();
    return e;
}

void store_Entities(char **entity_paths, Master_Manager master_M){
    store_Artists(entity_paths[2], master_M->artist_M);
    
    store_Album (entity_paths[3], master_M->album_M, master_M->artist_M);
    
    store_Musics(entity_paths[1], master_M->music_M, master_M->artist_M, master_M->album_M);
    order_duration (master_M->artist_M);
    
    store_Users(entity_paths[0], master_M->user_M, master_M->music_M);
    
    store_History(entity_paths[4], master_M->hist_M, master_M->artist_M, master_M->music_M, master_M->user_M);
    calc_top10s(master_M->artist_M);
    acc_freq_top10s(master_M->artist_M);
}



void answer_query(Query q, Master_Manager mm, int n_query, Query_stats qs){
    short type = get_query_type(q);
    if (type > 0){
        char output_file[46];
        snprintf(output_file, 46, "resultados/command%d_output.txt", n_query);
        char separador = get_separador (q);
        Output out = open_out (output_file, separador);
        switch (type)
        {
        case (1):
            answer1(q, mm->user_M, mm->artist_M, out, qs);
            break;
        case (2):
            answer2(q, mm->artist_M, out, qs);
            break;
        case (3):
            answer3(q, mm->music_M, out, qs);
            break;
        case (4):
            answer4(q, out, qs, mm->artist_M);
            break;
        case (5):
            answer5(q,mm->user_M,mm->music_M,mm->hist_M, out, qs);
            break;
        case (6):
            answer6(q, mm->artist_M, mm->music_M, mm->user_M, mm->hist_M, out, qs);
            break;
        }
        close_output (out);
    }
}

void answer_all_queries(Parser queries, Master_Manager mm, Query_stats qs){
    int i;
    Query q = create_query();
    for (i = 1; get_nRead(queries) != -1; i++)
    {
        read_query_line(queries, q);
        answer_query(q, mm, i, qs);
    }
    free_query(q);
}



void free_master_manager(Master_Manager m){
    free_user_manager(m->user_M);
    free_album_manager(m->album_M);
    free_music_manager(m->music_M);
    free_art_manager(m->artist_M);
    free_history_manager(m->hist_M);
    free(m);
}
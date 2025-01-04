#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L

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

/* COM TEMPOS
void store_Entities(char **entity_paths, Master_Manager master_M){
    struct timespec startart, endart;
    double elapsedart;
    clock_gettime(CLOCK_REALTIME, &startart);
    
    store_Artists(entity_paths[2], master_M->artist_M);
    
    clock_gettime(CLOCK_REALTIME, &endart);
    elapsedart = (endart.tv_sec - startart.tv_sec) + (endart.tv_nsec - startart.tv_nsec)/1e9;
    printf("Store Artists: %lfs\n", elapsedart);



    struct timespec startalb, endalb;
    double elapsedalb;
    clock_gettime(CLOCK_REALTIME, &startalb);

    store_Album (entity_paths[3], master_M->album_M, master_M->artist_M);
    
    clock_gettime(CLOCK_REALTIME, &endalb);
    elapsedalb = (endalb.tv_sec - startalb.tv_sec) + (endalb.tv_nsec - startalb.tv_nsec)/1e9;
    printf("Store Albums: %lfs\n", elapsedalb);



    struct timespec startmus, endmus;
    double elapsedmus;
    clock_gettime(CLOCK_REALTIME, &startmus);

    store_Musics(entity_paths[1], master_M->music_M, master_M->artist_M, master_M->album_M);
    
    clock_gettime(CLOCK_REALTIME, &endmus);
    elapsedmus = (endmus.tv_sec - startmus.tv_sec) + (endmus.tv_nsec - startmus.tv_nsec)/1e9;
    printf("Store Musics: %lfs\n", elapsedmus);
    

    
    struct timespec startord, endord;
    double elapsedord;
    clock_gettime(CLOCK_REALTIME, &startord);

    order_duration (master_M->artist_M);

    clock_gettime(CLOCK_REALTIME, &endord);
    elapsedord = (endord.tv_sec - startord.tv_sec) + (endord.tv_nsec - startord.tv_nsec)/1e9;
    printf("Order Artists: %lfs\n", elapsedord);


    
    struct timespec startusr, endusr;
    double elapsedusr;
    clock_gettime(CLOCK_REALTIME, &startusr);

    store_Users(entity_paths[0], master_M->user_M, master_M->music_M);

    clock_gettime(CLOCK_REALTIME, &endusr);
    elapsedusr = (endusr.tv_sec - startusr.tv_sec) + (endusr.tv_nsec - startusr.tv_nsec)/1e9;
    printf("Store Users: %lfs\n", elapsedusr);


    
    struct timespec starthis, endhis;
    double elapsedhis;
    clock_gettime(CLOCK_REALTIME, &starthis);

    store_History(entity_paths[4], master_M->hist_M, master_M->artist_M, master_M->music_M, master_M->user_M);
    
    clock_gettime(CLOCK_REALTIME, &endhis);
    elapsedhis = (endhis.tv_sec - starthis.tv_sec) + (endhis.tv_nsec - starthis.tv_nsec)/1e9;
    printf("Store Hist: %lfs\n", elapsedhis);
    


    struct timespec starttop, endtop;
    double elapsedtop;
    clock_gettime(CLOCK_REALTIME, &starttop);

    calc_top10s(master_M->artist_M);
    acc_freq_top10s(master_M->artist_M);

    clock_gettime(CLOCK_REALTIME, &endtop);
    elapsedtop = (endtop.tv_sec - starttop.tv_sec) + (endtop.tv_nsec - starttop.tv_nsec)/1e9;
    printf("Top 10s: %lfs\n", elapsedtop);
}*/

int store_Entities(char **entity_paths, Master_Manager master_M, int interativo){
    if(store_Artists(entity_paths[2], master_M->artist_M, interativo) == 1) return 1;

    if(store_Album (entity_paths[3], master_M->album_M, master_M->artist_M, interativo) == 1) return 1;
    
    if(store_Musics(entity_paths[1], master_M->music_M, master_M->artist_M, master_M->album_M, interativo) == 1) return 1;
    
    order_duration (master_M->artist_M);
    
    if(store_Users(entity_paths[0], master_M->user_M, master_M->music_M, interativo) == 1) return 1;
    
    if(store_History(entity_paths[4], master_M->hist_M, master_M->artist_M, master_M->music_M, master_M->user_M, interativo) == 1) return 1;;
    
    calc_top10s(master_M->artist_M);
    acc_freq_top10s(master_M->artist_M);

    return 0;
}


void answer_query(Query q, Master_Manager mm, int n_query, Query_stats qs, int terminal, int rec_xpto){
    short type = get_query_type(q);
    if (type > 0){
        char output_file[46];
        snprintf(output_file, 46, "resultados/command%d_output.txt", n_query);
        char separador = get_separador (q);
        Output out = open_out (output_file, separador, terminal);
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
            answer5(q,mm->user_M,mm->music_M,mm->hist_M, out, qs, rec_xpto);
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
        answer_query(q, mm, i, qs, 0, 0);  // 0 nos campos dos booleanos porque esta função nunca é chamada no modo interativo
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
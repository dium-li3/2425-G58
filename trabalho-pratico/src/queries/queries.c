#define _GNU_SOURCE and #define _POSIC_C_SOURCE 1999309L
#include <glib.h>
#include <string.h>


#include "parser.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "music_manager.h"
#include "history_manager.h"
#include "output.h"
#include "queries.h"
#include "utils.h"
#include "recomendador.h"

/*
    Estrutura para armazenar o nº de execuções de uma dada query (n)
    e o tempo total para essas execuções(time).
    O índice para uma query do tipo q é q-1.
*/
typedef struct query_stats {
    int n[QUERYTYPES];
    double time[QUERYTYPES];
} *Query_stats;

/*
    O objetivo será criar uma única query, que tem os seus dados
    atualizados ao ler cada linha.
*/
typedef struct query{
    short query;
    Query1 query1;
    Query2 query2;
    Query3 query3;
    Query4 query4;
    Query5 query5;
    Query6 query6;
    char separador; 
} *Query;

typedef struct query1{
    int id; //id a procurar nos nossos dados.
    char type; // A == artist e U == User
} *Query1;

typedef struct query2{
    short N; //top N artistas
    char *country;//Pode ser NULL
} *Query2;

typedef struct query3{
    short min; //idade minima
    short max; //idade maxima
} *Query3;

typedef struct query4{
    int begin_week;
    int end_week;
} *Query4;

typedef struct query5{
    char *user_id;
    int N_results;
} *Query5;

typedef struct query6{
    int user_id;
    int year;
    int N_artists;
} *Query6;

Query create_query(){
    Query q = calloc (1, sizeof(struct query));//define logo int query a 0.
    q->query1 = calloc (1, sizeof(struct query1));
    q->query2 = calloc (1, sizeof(struct query2));
    q->query3 = calloc (1, sizeof(struct query3));
    q->query4 = calloc (1, sizeof(struct query4));
    q->query5 = calloc (1, sizeof(struct query5));
    q->query6 = calloc (1, sizeof(struct query6));

    q->separador = ' ';
    return q;
}


void set_query_invalid(Query q){
    q->query = -1;
}

void set_query1(int id, char entity, Query q){
    q->query = 1;
    q->query1->id = id;
    q->query1->type = entity;
}

void set_query2(short N, char *country, Query q){
    q->query = 2;
    q->query2->N = N;
    if (country != NULL){
        free(q->query2->country);
        q->query2->country = strdup (country);
    }
    else{
        free(q->query2->country);
        q->query2->country = NULL;
    }
}

void set_query3(short min, short max, Query q){
    q->query = 3;
    q->query3->min = min;
    q->query3->max = max;
}

void set_query4(int first_week, int last_week, Query q){
    q->query = 4;
    q->query4->begin_week = first_week;
    q->query4->end_week = last_week;
}

void set_query5(char *user_id, int N, Query q){
    q->query = 5;
    q->query5->N_results = N;
    if (user_id != NULL){
        free (q->query5->user_id);
        q->query5->user_id = strdup (user_id);
    }
    else{
        free (q->query5->user_id);
        q->query5->user_id = NULL;
    }
}

void set_query6(int user_id, int year, int N, Query q){
    q->query = 6;
    q->query6->user_id = user_id;
    q->query6->year = year;
    q->query6->N_artists = N;
}


short get_query_type(Query q){
    return q->query;
}

char get_separador (Query q){
    return q->separador;
}

void store_query_from_token (Query q, char **tokens, int n_tokens){
    int N = 0;
    int begin, end;
    int day, month, year;

    if (tokens[0] != NULL){
        
        if (strlen (tokens[0]) > 1)
            q->separador = '=';
        else
            q->separador = ';';
        
        q->query = (short) atoi(tokens[0]);//numero da query.
        switch (q->query){
            case (1):
                set_query1 ((int) atoi(tokens[1] + 1), *tokens[1], q); //+1 para ignorar logo o U ou A
                break;
            case (2):
                set_query2 ((short) atoi(tokens[1]), tokens[2], q);
                break;
            case (3):
                set_query3 ((short) atoi(tokens[1]), (short) atoi(tokens[2]), q);
                break;
            case (4):
                begin = end = -1;
                
                if (tokens[1] != NULL && tokens[2] != NULL){
                    sscanf (tokens[2], "%d/%d/%d", &year, &month, &day);
                    begin = calc_week (day, month, year);
                    
                    sscanf (tokens[1], "%d/%d/%d", &year, &month, &day);
                    end = calc_week (day, month, year);
                }

                set_query4 (begin, end, q);
                break;
            case (5):
                set_query5 (tokens[1], atoi (tokens[2]), q);
                break;
            case (6):
                if (tokens[3] != NULL)
                    N = atoi (tokens[3]);
                else N = 0;
                set_query6 (atoi (tokens[1] + 1), atoi (tokens[2]), N, q);
                break;
            default:
                set_query_invalid (q);
        }
    }
    else set_query_invalid(q);//não leu tokens
}

void read_query_line(Parser pq, Query q){
    char **tokens = calloc (4, sizeof(char *));//basta 3 espaços por agora
    int n_tokens = parse_1line_query(pq, tokens, 4);
    store_query_from_token (q, tokens, n_tokens);
    if (q->query != -1)
        for (int i = 0; i < 4; i++){
            free (tokens[i]);
        }
    free (tokens);
}


void free_query2 (Query2 q){
    free (q->country);
    free (q);
}

void free_query5 (Query5 q){
    free (q->user_id);
    free (q);
}

void free_query (Query q){
    free (q->query1);
    free_query2 (q->query2);
    free (q->query3);
    free (q->query4);
    free_query5 (q->query5);
    free (q->query6);
    free (q);
}


void answer1(Query q, User_Manager um, Art_Manager am, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query1 q1 = q->query1;
    if (q1->type == 'U')
        print_user_res_by_id (um, q1->id, out);
    else
        print_art_res_by_id (am, q1->id, out);
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;
    
    if (qs != NULL) add_query_stats(qs, elapsed, 1);
}

void answer2(Query q, Art_Manager am, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query2 q2 = q->query2;
    int N = q2->N;
    if (N == 0)
        output_empty (out);
    else {
        if (q2->country == NULL)
            print_N_art_info (am, N, out);
        else {
            char *country = NULL;
            country = strdup (q2->country);
            print_N_country_art_info (am, country, N, out);
            free (country);
        }
    }

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;

    if (qs != NULL) add_query_stats(qs, elapsed, 2);
}

void answer3(Query q, Music_Manager mm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    Query3 q3 = q->query3;
    sort_gen(mm, q3->min, q3->max);

    print_all_genres_info (mm, out);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;

    if (qs != NULL) add_query_stats(qs, elapsed, 3);
}

void answer4(Query q, Output out, Query_stats qs, Art_Manager am){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    
    int mw = get_max_week(am), art_id, top_count;
    
    if(q->query4->begin_week > mw) output_empty (out);
    else {
        art_id = find_most_freq_top_art(q->query4->begin_week, q->query4->end_week, am, &top_count);
        if(top_count == 0) output_empty(out);
        else print_most_freq_top_art(art_id, top_count, am, out);
    }
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;

    if (qs != NULL) add_query_stats(qs, elapsed, 4);
}

void answer5(Query q, User_Manager um, Music_Manager mm,History_Manager hm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);

    if(q->query5->N_results == 0 || !user_exists (atoi (q->query5->user_id + 1), um)) {
        output_empty(out);
    } 
    else {
        Query5 q5 = q->query5;
        char *idUtilizadorAlvo = q5->user_id;
        int **matrizClassificacaoMusicas = get_matrix(hm);
        char **idsUtilizadores = get_users_ids(um);
        char **nomesGeneros = get_genre_names(mm);
        int numUtilizadores = get_total_users(um);
        int numGeneros = get_total_genres(mm);
        int numRecomendacoes = q5->N_results;
        
        char **arrAnswer;

        arrAnswer = recomendaUtilizadores(idUtilizadorAlvo, matrizClassificacaoMusicas, idsUtilizadores,nomesGeneros,numUtilizadores,numGeneros,numRecomendacoes);
        
        for (int i = 0;i<numRecomendacoes;i++) {
            output_geral(arrAnswer + i,1,out);
        }
        
        free (arrAnswer);
    }
  
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;

    if (qs != NULL) add_query_stats(qs, elapsed, 5);
}

//Lógica de resposta à query 6

typedef struct favorite_artist_info{
    GHashTable *distinct_musics;
    int listening_time;
    int art_id;
} *Fav_art_info;

Fav_art_info create_fav_art_info (int music_id, int duration, int art_id){
    Fav_art_info fai = malloc (sizeof (struct favorite_artist_info));
    fai->distinct_musics = g_hash_table_new (g_direct_hash, g_direct_equal);
    g_hash_table_insert (fai->distinct_musics, GINT_TO_POINTER (music_id), GINT_TO_POINTER (music_id));
    fai->listening_time = duration;
    fai->art_id = art_id;
    return fai;
}

void free_fav_art_info (Fav_art_info fai){
    if (fai->distinct_musics != NULL)
        g_hash_table_destroy (fai->distinct_musics);
    free (fai);
}

void update_fav_art_info (Fav_art_info fai, int music_id, int duration){

    gpointer musics = g_hash_table_lookup (fai->distinct_musics, GINT_TO_POINTER (music_id));
    if (musics == NULL)
        g_hash_table_insert (fai->distinct_musics, GINT_TO_POINTER (music_id), GINT_TO_POINTER (music_id));
    fai->listening_time += duration;
}

int compare_fav_art_durations (gconstpointer f1, gconstpointer f2){
    int r = 0;
    Fav_art_info *pv1 = (Fav_art_info*) f1;
    Fav_art_info *pv2 = (Fav_art_info*) f2;
    if ((*pv1)->listening_time > (*pv2)->listening_time)
        r = -1;
    else if ((*pv1)->listening_time < (*pv2)->listening_time)
        r = 1;
    else if ((*pv1)->art_id < (*pv2)->art_id) //artistas que foram igualmente ouvidos.
        r = -1;
    else 
        r = 1;
    return r;
}

typedef struct max_dur{
    int id;
    int dur;
} *Max_dur;

Max_dur base_max(){
    Max_dur m = calloc (1, sizeof (struct max_dur));
    return m;
}

void max_in_hash(gpointer id, gpointer dur, gpointer user_data) {
    Max_dur max_par = (Max_dur)user_data;
    int listening_time = GPOINTER_TO_INT(dur);

    if (listening_time > (max_par)->dur) {
        (max_par)->dur = listening_time;
        (max_par)->id = GPOINTER_TO_INT (id);
    }
}


void answer6(Query q, Art_Manager am, Music_Manager mm, User_Manager um, History_Manager hm, Output out, Query_stats qs){
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);

    Query6 q6= q->query6;

    //Guardar a informação relativa a um dado ano de um dado utilizador.    
    int i, j, k, history_pos, music_id, album_id, artist_id, month, day, hour, listening_time, genre_ind, num_musics, album_dur;
    int hour_arr [24] = {0};
    int month_and_day_matriz [12][31] = {0};
    int listening_time_total = 0;
    int n_genres = get_total_genres (mm);
    int *genres = calloc (n_genres, sizeof(int));
    const char *favorite_genre = NULL;
    char ***favorite_artists = NULL;
    char **output_tokens = NULL;
    char **tokens = NULL;
    int useless;

    gpointer already_exists;

    GHashTable *diff_albums = g_hash_table_new(g_direct_hash, g_direct_equal);
    Max_dur max = base_max();

    GPtrArray *diff_artists = g_ptr_array_new_full (15, (GDestroyNotify) free_fav_art_info);
    Fav_art_info art_info;
    const GArray *artists_ids;

    GHashTable *diff_musics = g_hash_table_new(g_direct_hash, g_direct_equal);

    Parser hist_file = open_parser (get_history_path(hm));
    const GArray *yearly_history_positions = get_year_history_from_user_id(q6->user_id , q6->year, um);
    int number_histories;
    if (yearly_history_positions != NULL && yearly_history_positions->len > 0){
        number_histories = yearly_history_positions->len;

        //Percorrer os históricos e 'absorve' as suas informações
        for (i = 0; i < number_histories; i++){
            history_pos = g_array_index (yearly_history_positions, long, i);
            set_file_pos (hist_file, history_pos);
            tokens = parse_line (hist_file, 8);
            music_id = atoi (tokens[2] +1);
            hour = read_timestamp_elements (tokens[3], &useless, &month, &day);
            listening_time = calc_duration_s (tokens[4]);
            
            //get_history_info (history_id, &listening_time, &music_id, &month, &day, &hour, hm);
            artists_ids = get_music_artists_from_id (music_id, mm);
            genre_ind = search_gen_index_by_id (music_id, mm);
            album_id = get_music_album_by_id (music_id, mm);

            //Guarda albums
            already_exists = g_hash_table_lookup (diff_albums, GINT_TO_POINTER (album_id));
            if (already_exists == NULL)
                g_hash_table_insert (diff_albums, GINT_TO_POINTER (album_id), GINT_TO_POINTER (listening_time));
            else{
                album_dur = GPOINTER_TO_INT (already_exists);
                album_dur += listening_time;
                g_hash_table_replace (diff_albums, GINT_TO_POINTER(album_id), GINT_TO_POINTER (album_dur)); 
            }

            //Guarda musicas
            already_exists = g_hash_table_lookup (diff_musics, GINT_TO_POINTER (music_id));
            if (already_exists == NULL)
                g_hash_table_insert (diff_musics , GINT_TO_POINTER (music_id), GINT_TO_POINTER (music_id));
            
            //Guarda artistas
            for (j = 0; j < artists_ids->len; j++){
                artist_id = g_array_index (artists_ids, int, j);
                for (already_exists = NULL, k = 0; k < diff_artists->len && diff_artists->pdata[k] != NULL && ((Fav_art_info)g_ptr_array_index (diff_artists, k))->art_id != artist_id; k++);
                if (k < diff_artists->len)
                    already_exists = g_ptr_array_index (diff_artists, k);
                if (already_exists == NULL){
                    art_info = create_fav_art_info (music_id, listening_time, artist_id);
                    g_ptr_array_add (diff_artists, art_info);
                }
                else{
                    art_info = (Fav_art_info)already_exists;
                    update_fav_art_info (art_info, music_id, art_info->listening_time + listening_time);//posso?
                }
            }
            
            //Parte facil
            listening_time_total += listening_time;
            hour_arr[hour]+= listening_time;
            month_and_day_matriz[month-1][day-1]++;
            genres[genre_ind]+= listening_time;
        }
        //Calcular os favoritos
        hour = array_max (hour_arr, 24);
        month = 11; day = 30;
        for (i = 11; i >= 0; i --)
            for (j = 30; j >= 0; j--)
                if (month_and_day_matriz[i][j] > month_and_day_matriz[month][day]){
                    month = i; //mes favorito
                    day = j; //dia favorito
                }
        day++; month++;
        genre_ind = array_max (genres, n_genres);
        favorite_genre = get_genre_names(mm)[genre_ind];
        num_musics = g_hash_table_size (diff_musics);

        g_hash_table_foreach (diff_albums, max_in_hash, max);

        g_ptr_array_sort (diff_artists, compare_fav_art_durations);
        favorite_artists = calloc (q6->N_artists, sizeof (char**));
        for (i = 0; i < q6->N_artists && i < diff_artists->len && diff_artists->pdata[i] != NULL; i++){
            art_info = (Fav_art_info)g_ptr_array_index (diff_artists, i);

            favorite_artists [i] = calloc (3, sizeof (char *));
            favorite_artists [i][0] = calloc (12, 1);
            favorite_artists [i][1] = calloc (11, 1);
            
            snprintf (favorite_artists [i][0], 12, "A%07d", art_info->art_id);
            snprintf (favorite_artists [i][1], 11, "%d", g_hash_table_size (art_info->distinct_musics));
            favorite_artists [i][2] = calc_duration_hms (art_info->listening_time);
        }

        //Fase de print
        output_tokens = calloc (7, sizeof (char *));

        output_tokens[0] = calc_duration_hms (listening_time_total);
        
        output_tokens[1] = calloc (11, sizeof (char));
        snprintf (output_tokens[1], 11, "%d", num_musics);

        output_tokens[2] = calloc (12, sizeof (char));
        art_info = (Fav_art_info)g_ptr_array_index (diff_artists, 0);
        snprintf (output_tokens[2], 12, "A%07d", art_info->art_id);

        output_tokens[3] = calloc (33, sizeof (char));
        snprintf (output_tokens[3], 33, "%d/%02d/%02d", q6->year, month, day);

        output_tokens[4] = strdup (favorite_genre);
        
        output_tokens[5] = calloc (12, sizeof (char));
        snprintf(output_tokens[5], 12, "AL%06d", max->id);
        
        output_tokens[6] = calloc (11, sizeof (char));
        snprintf(output_tokens[6], 11, "%02d", hour);

        output_geral (output_tokens, 7, out);

        for (i = 0; i < q6->N_artists && i < diff_artists->len; i++)
            output_geral (favorite_artists[i], 3, out);

        for (i = 0; i < q6->N_artists && i < diff_artists->len; i++){
            for (j = 0; j < 3; j++)
                free (favorite_artists[i][j]);
            free (favorite_artists [i]);
        }
        free_tokens (output_tokens, 7);
        free (favorite_artists);
    }
    else
        output_empty (out);
    free (genres);
    free (max);
    if (diff_musics != NULL)
        g_hash_table_destroy (diff_musics);
    if (diff_albums != NULL)
        g_hash_table_destroy (diff_albums);
    g_ptr_array_free (diff_artists, TRUE);

    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9) * 1e3;

    if (qs != NULL) add_query_stats(qs, elapsed, 6);
}

Query_stats create_query_stats() {
    Query_stats r = calloc(1, sizeof(struct query_stats));

    return r;
}

void add_query_stats(Query_stats qd, double time, int type) {
    qd->n[type-1]++;
    qd->time[type-1] += time;
}

double get_query_stats_time(Query_stats qd, int i) {
    return (qd->time[i]);
}

int get_query_stats_n(Query_stats qd, int i) {
    return (qd->n[i]);
}

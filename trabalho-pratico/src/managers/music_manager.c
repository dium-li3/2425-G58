#include <glib.h>
#include <stdio.h>
#include "parser.h"
#include "music_manager.h"
#include "artist_manager.h"
#include "logica.h"
#include "output.h"

typedef struct music_manager {
    GHashTable *musics_by_id;
} *Music_Manager;


Music_Manager create_music_manager(){
    Music_Manager mm = malloc (sizeof(struct music_manager));
    mm->musics_by_id = g_hash_table_new_full (g_int_hash, g_int_equal, free, (void *)free_music); //hash
    return mm;
}


void insert_music_by_id(Music m, Music_Manager music_manager){
    int *id = get_music_id_pointer(m);
    g_hash_table_insert (music_manager->musics_by_id, id, m);
}


void store_Musics(char *music_path, Music_Manager mm, Art_Manager am){
    char **line = calloc(1, sizeof(char *));
    Parser p = open_parser(music_path);
    Output out = open_out("resultados/musics_errors.csv");
    Music music = NULL;
    while (get_nRead(p) != -1){
        music = parse_line(p, (void *)create_music_from_tokens);
        if (music != NULL){
            if (valid_artists(get_music_artists(music), get_music_duration(music), am))
                insert_music_by_id(music, mm);
            else{
                free_music(music);
                music = NULL;
                error_output(p, out, line);
            }
        }
        else{
            if (get_nRead(p) != -1)
                error_output(p, out, line);
        }
    }
    close_parser(p);
    close_output(out);
    free(line);
}


Music search_music_by_id(int id, Music_Manager music_manager){
    Music m = g_hash_table_lookup(music_manager->musics_by_id, &id);
    return m;
}


void free_music_manager(Music_Manager mm){
    g_hash_table_destroy (mm->musics_by_id);
    free (mm);
}
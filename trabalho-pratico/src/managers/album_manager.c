#include <glib.h>
#include <stdlib.h>

#include "album_manager.h"
#include "albums.h"
#include "parser.h"
#include "output.h"
#include "artist_manager.h"

typedef struct album_manager{
    GHashTable *albums_by_id;
} *Album_Manager;

Album_Manager create_album_manager (){
    Album_Manager album_m = malloc (sizeof (struct album_manager));
    album_m->albums_by_id = g_hash_table_new_full(g_direct_hash, g_direct_equal, FALSE, (void *)free_album);
    return album_m;
}

void free_album_manager (Album_Manager am){
    g_hash_table_destroy (am->albums_by_id);
    free (am);
}

void insert_album_by_id (Album al, int id, Album_Manager album_manager){
    g_hash_table_insert(album_manager->albums_by_id, GINT_TO_POINTER(id), al);
}

Album search_album_by_id(int id, Album_Manager album_manager){
    Album al = g_hash_table_lookup(album_manager->albums_by_id, GINT_TO_POINTER(id));
    return al;
}

gboolean album_exists (int id, Album_Manager am){
    return (search_album_by_id (id, am) != NULL);
}

void store_Album (char *album_path, Album_Manager album_man, Art_Manager art_man){
    Parser p = open_parser(album_path);
    if(p == NULL) {
        perror("store_Album(17)");
        exit(1);
    }

    Output out = open_out("resultados/albums_errors.csv", ';');
    Generic_Album generic_album = NULL;
    Album album = NULL;
    int id;
    const GArray *artists_ids = NULL;

    while (get_nRead(p) != -1){
        generic_album = parse_line (p, (void *)create_generic_album_from_tokens);
        
        if (generic_album != NULL){
            id = get_album_id (generic_album);
            artists_ids = get_album_artists (generic_album);
            
            add_1_album_to_artists (artists_ids, art_man);

            album = create_album_delete_generic (generic_album);
            insert_album_by_id (album, id, album_man);
        }
        else{
            if (get_nRead(p) != -1)
                error_output (p, out);
        }
    }
    close_parser (p);
    close_output (out);
}


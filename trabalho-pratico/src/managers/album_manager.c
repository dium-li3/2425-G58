#include <glib.h>
#include <stdlib.h>
#include <ncurses.h>

#include "album_manager.h"
#include "albums.h"
#include "parser.h"
#include "output.h"
#include "artist_manager.h"
#include "utils.h"

#define ALBUM_ELEMS 5

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

int store_Album (char *album_path, Album_Manager album_man, Art_Manager art_man, int interativo){
    Parser p = open_parser(album_path);
    if(p == NULL) {
        interativo ? printw("%s: ficheiro não encontrado.\n", album_path) : fprintf(stderr, "%s: %s\n", strerror(errno), album_path);
        return 1;
    }

    Album album = NULL;
    int id;
    GArray *artists_ids = NULL;
    char **tokens = NULL;

    tokens = parse_line (p, ALBUM_ELEMS); //ignorar a 1ª linha do ficheiro
    free_tokens(tokens, ALBUM_ELEMS);
    for (tokens = parse_line (p, ALBUM_ELEMS); tokens != NULL; tokens = parse_line (p, ALBUM_ELEMS)){
        id = atoi(tokens[0]+2);
        artists_ids = store_list (tokens[2]);
        album = create_album_from_tokens (tokens[1]);

        add_1_album_to_artists (artists_ids, art_man);
        insert_album_by_id (album, id, album_man);

        g_array_free (artists_ids, TRUE);
        
        free_tokens(tokens, ALBUM_ELEMS);
    }
  
    close_parser (p);
    return 0;
}

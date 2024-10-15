#include <glib.h>
#include "music_manager.h"

typedef struct music_manager {
    GHashTable *musics_by_id;
} *Music_Manager;




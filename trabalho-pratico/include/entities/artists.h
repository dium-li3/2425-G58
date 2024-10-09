#ifndef _ARTISTS_
#define _ARTISTS_

typedef struct artist{
    int id;
    char *name;
    char *description;
    float recipe_per_stream;
    int *id_constituent; //ou int e ignoramos o A de todos os artistas?
    char *country;
    char type; //solo = S grupo = G;
} Artist;

#endif
#include "users.h"
typedef struct user{
    int id;
    char *email;
    char *first_name;
    char *last_name;
    char *birth_date; //possivelmente vai ser retirado
    short age;
    char *country;
    char subscription_type;//normal = N, premium = P e inválido = E?
    GSList *liked_music_ids;
} *User;

User create_user (int id, char *email, char *fn, char *ln, char *bd, short age, char *c, char st, GSList *lmids){
    User u = malloc(sizeof (struct user));
    u->id = id;
    u->email = strdup (email);
    u->first_name =strdup (fn);
    u->last_name = strdup (ln);
    u->birth_date = strdup (bd);
    u->age = age;
    u->country = strdup (c);
    u->subscription_type = st;
    u->liked_music_ids = lmids;
    return u;
}

/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
void *create_user_from_tokens (char **tokens){
    char sub_type = get_sub_type (tokens[6]);
    int valid = valid_user_sintatic (tokens[1], tokens[4], sub_type);
    int id;
    int age;
    GSList *liked_musics = NULL;
    User u = NULL;
    if (valid){ //store
        id = atoi (tokens[0]+1);
        age = read_date_to_age (tokens[4]);
        liked_musics = store_list (tokens[7]);
        u = create_user (id, tokens[1], tokens[2], tokens[3], tokens[4], age, tokens[5], sub_type, liked_musics);
    }
    return u;
}

//Devolve o username do utilizador.
int get_user_id(User u){
    return u->id;
}

int *get_user_id_pointer (User u){
    return &(u->id);
}

char *get_user_email (User u){
    return u->email;
}

char *get_user_first_name (User u){
    return u->first_name;
}

char *get_user_last_name (User u){
    return u->last_name;
}

char *get_user_country (User u){
    return u->country;
}

short get_user_age (User u){
    return u->age;
}


//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type){
    char c = 'E';
    if  (strcmp (sub_type, "normal") == 0)
        c = 'N';
    if (strcmp(sub_type, "premium") == 0)
        c = 'P';
    return c;
}

void free_user(User u)
{
    free(u->email);
    free(u->first_name);
    free(u->last_name);
    free(u->birth_date);
    free(u->country);
    g_slist_free_full(u->liked_music_ids, free);
    free(u);
}

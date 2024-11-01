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
User create_user_from_tokens (char **tokens){
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
    int *copy = malloc(sizeof(int));
    *copy = u->id;

    return copy;
}

char *get_user_email (User u){
    return (strdup(u->email));
}

char *get_user_first_name (User u){
    return (strdup(u->first_name));
}

char *get_user_last_name (User u){
    return (strdup(u->last_name));
}

char *get_user_country (User u){
    return (strdup(u->country));
}

short get_user_age (User u){
    return u->age;
}

GSList *get_liked_musics(User u){
    return u->liked_music_ids;
}


//Dada uma string com o nome da subscription type, devolve o caracter que a representa.
char get_sub_type (char *sub_type){
    char c = 'E';
    if (strcmp (sub_type, "normal") == 0)
        c = 'N';
    else if (strcmp(sub_type, "premium") == 0)
        c = 'P';
    return c;
}

/*
    Dá print do email, nomes, idade e pais do utilizador.
*/
void print_user_info(User u, int n_querie){
    char output_file[34]; // dá para numeros até 9999
    snprintf(output_file, 34, "resultados/command%d_output.txt", n_querie);
    FILE *fp = fopen(output_file, "w+");
    if (u != NULL){
        fprintf(fp, "%s;%s;%s;%d;%s\n", u->email, u->first_name, u->last_name, u->age, u->country);
    }
    else fprintf(fp, "\n");
    fclose(fp);
}

void free_user(User u){
    free(u->email);
    free(u->first_name);
    free(u->last_name);
    free(u->birth_date);
    free(u->country);
    g_slist_free_full(u->liked_music_ids, free);
    free(u);
}

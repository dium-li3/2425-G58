#include <ctype.h>
#include "users.h"
#include "utils.h"
#include "output.h"

typedef struct user{
    int id;
    short age;
    GPtrArray *yearly_history_ids;
    int index;
    long file_pos;
} *User;


/*
    Cria um User, baseado nos tokens recebidos.
    Devolve NULL caso o user seja sintáticamente inválido.
*/
User create_user (int id, short age, int index, long fp){

    GArray *year;
    User u = malloc(sizeof (struct user));
    u->id = id;
    u->age = age;
    u->index = index;
    u->file_pos = fp;
  
    u->yearly_history_ids = g_ptr_array_sized_new (7);

    for (int i = 0; i < 7; i++){
        year = g_array_new (FALSE, TRUE, sizeof(int));
        g_ptr_array_add (u->yearly_history_ids, year);
    }
    return u;
}

int get_user_id(User u){
    return u->id;
}

int *get_user_id_pointer (User u){
    int *copy = malloc(sizeof(int));
    *copy = u->id;

    return copy;
}

short get_user_age (User u){
    return u->age;
}

int get_user_index(User u){
    return u->index;
}

const GArray *get_year_history(User u, int year){
    const GArray *history_ids;
    int id = 2024 - year;

    if (id >= u->yearly_history_ids->len)
        history_ids = NULL;
    else
        history_ids = g_ptr_array_index (u->yearly_history_ids, id);
    return history_ids;
}

void add_year_history_id(User u, int year, int history_id){
    GArray *year_history;
    int id = 2024 - year;
    int ptr_array_len = u->yearly_history_ids->len;
    if (id > u->yearly_history_ids->len){
        for (int i = ptr_array_len; i <= id; i++){
            year_history = g_array_new (TRUE, TRUE, sizeof(int));
            g_ptr_array_add (u->yearly_history_ids, year_history);
        }
    }
    year_history = (GArray *)g_ptr_array_index (u->yearly_history_ids, id);

    g_array_append_val (year_history, history_id);
}

void print_user_res(User u, Output out, Parser p){
    if (u != NULL){
        set_file_pos(p, u->file_pos);
        char **tokens = parse_line(p, 8);

        char **info = calloc (5, sizeof (char *));
        info[0] = strdup(tokens[1]);
        info[1] = strdup(tokens[2]);
        info[2] = strdup(tokens[3]);
        info[3] = malloc (11*sizeof(char)); //não haverá ninguem com + de 999 anos...
        sprintf(info[3], "%d", u->age);
        info[4] = strdup(tokens[5]);

        output_geral(info, 5, out);
        
        free_tokens(info, 5);
        free_tokens(tokens, 8);
    }
    else output_empty (out);
}

void free_user(User u){
    for (int i = 0; i < u->yearly_history_ids->len; i++)
        g_array_free (u->yearly_history_ids->pdata[i], TRUE);
    g_ptr_array_free (u->yearly_history_ids, TRUE);
    
    free(u);
}


/*
    Verifica se um email está escrito direito (username@lstring.rstring).
    username só minusculas ou numeros .lstring tamanho superior a 1.
    rstring tamanho entre 2 e 3 inclusive. lstring e rstring só têm minusculas.
*/
int valid_email_string (char *email){
    int us = 0;
    int ls = 0;
    int rs = 0;
    int r = 1;
    //Valida o username do email
    for (; (isdigit (email[us]) || islower (email[us])) && email[us] != '\0'; us++);
    if (us == 0 || email[us] != '@')
        r = 0; //return 0;
    else {//Valida a lstring
        ls = us+1;
        for (; islower(email[ls]) && email[ls] != '\0'; ls++);
        if (ls == 0 || email[ls] != '.')
            r = 0;
        else {//valida a rstring
            rs = ls + 1;
            for (;rs - ls < 4 && islower(email[rs]); rs++);
            if (rs - ls < 2 || email[rs] != '\0')
                r = 0;
        }
    }
    return r;
}

/*
    Verifica se uma subscription_type está escrita direito.
    Possivelmente vai ser apagada por ser kinda trivializada pela get_sub_type
    que nos dá logo se a subscription é válida ou não.
*/
int valid_subscription (char *subs_type){
    return (strcmp (subs_type, "normal") == 0 || strcmp (subs_type, "premium") == 0);
}

//Verifica se os campos que têm de ser sintáticamente validados de um dado utilizador estão direito/válidos.
int valid_user_sintatic (char *email, char *date, char *sub_type){
    return valid_email_string(email) && valid_date(date) && valid_subscription(sub_type);
}

User create_user_from_tokens (char **tokens, int index, long file_pos){
    int valid = valid_user_sintatic (tokens[1], tokens[4], tokens[6]) && valid_list(tokens[7]);
    int id;
    int age;
    User u = NULL;
    if (valid){ //store
        id = atoi (tokens[0]+1);
        age = read_date_to_age (tokens[4]);
        u = create_user (id, age, index, file_pos);
    }
    return u;
}
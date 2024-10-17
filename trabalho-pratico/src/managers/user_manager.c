#include <glib.h>
#include <stdio.h>

#include "user_manager.h"
#include "sintatica.h"
#include "parser.h"
#include "utils.h"

//g_hash_table_get_keys (user_manager->users_by_id) dá-nos logo todos os ids.
typedef struct user_manager {
    GHashTable *users_by_id;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_int_hash, g_int_equal, free, (void *)free_user); //hash
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    int *id = get_user_id_pointer(u);//REVER
    g_hash_table_insert (user_manager->users_by_id, id, u);
}


/*
    Guarda o user de uma linha na hash table do manager.
    Devolve 1 caso tenha corrido tudo bem, e
    devolve 0 caso o user tenha falhado a validação
    sintática, nesse caso não o guarda.
*/

/*
//SListas, 569-604MB, 1.8s-2s, 20M allocs ->667MB allocated
int store_User (User_Manager user_manager, GSList *linhas){
    int r = 1;
    char **tokens = calloc (8 ,sizeof(char *));
    char *linha = strdup (linhas->data);
    tokens = parse_line(linha, tokens);
    free (linha);
    User user = create_user_from_tokens (tokens);
    for (int i = 0; i < 8; i++){
        free (tokens[i]);
    }
    free (tokens);
    if (user != NULL){
        insert_user_by_id (user, user_manager);
    }
    else r = 0;
    return r;
}

void store_Users (FILE *fp_Users, User_Manager user_manager){
    GSList *linhas = parse_file (fp_Users);
    FILE *user_errors = fopen ("user_errors.csv", "w+");
    GSList *help = linhas;
    while (linhas != NULL){
        int user_valido = store_User (user_manager, linhas);
        if (!user_valido) {
            //print da linha para o documento de erros de user.
            fprintf(user_errors, "%s", (char *)linhas->data);
        }
        linhas = linhas->next;
    }
    g_slist_free_full(help, free);
}*/

/*
//Acessos repetidos ao ficheiro 450-470MB, 1.8s-2s, 20M allocs ->592MB allocated
int store_User (User_Manager user_manager, char *line){
    int r = 1;
    char **tokens = calloc (8 ,sizeof(char *));
    tokens = parse_line(line, tokens);
    User user = create_user_from_tokens (tokens);
    for (int i = 0; i < 8; i++){
        free (tokens[i]);
    }
    free (tokens);
    if (user != NULL){
        i
    }
    else r = 0;
    return r;
}*/

void store_Users (FILE *fp_Users, User_Manager user_manager){
    ssize_t nRead = 0;
    char **line = calloc(1, sizeof (char *));
    FILE *user_errors = fopen ("resultados/users_errors.csv", "w+");
    while (nRead != -1){
        User user = (User)parse_line (fp_Users, create_user_from_tokens, &nRead);
        if (user != NULL){
            insert_user_by_id (user, user_manager);
        }
        else{
            if (nRead != -1){
                *line = NULL;
                fseek (fp_Users, -nRead, SEEK_CUR);
                parse_1line (fp_Users, line);
                fprintf(user_errors, "%s", *line);
                free (*line);
            }
        }
    }
    fclose (user_errors);
    free (line); 
}


User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, &id);
    return u;
}

void free_user_manager(User_Manager um){
    g_hash_table_destroy (um->users_by_id);
    free (um);
}

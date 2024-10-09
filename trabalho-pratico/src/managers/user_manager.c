#include <glib.h>
#include <stdio.h>

#include "user_manager.h"
#include "sintatica.h"
#include "parser.h"

//g_hash_table_get_keys (user_manager->users_by_id) dá-nos logo todos os ids.
typedef struct user_manager {
    GHashTable *users_by_id;
} *User_Manager;


User_Manager create_user_manager(){
    User_Manager um = malloc (sizeof(struct user_manager));
    um->users_by_id = g_hash_table_new_full (g_int_hash, g_int_equal, NULL, (void *)free_user); //hash
    return um;
}

/*
    Insere um User na posição id da hash table.
*/
void insert_user_by_id(User u, User_Manager user_manager){
    int id = get_user_id(u);
    g_hash_table_insert (user_manager->users_by_id, &id, u);
}

void store_Users (FILE *fp_Users, User_Manager user_manager){
    GSList *linhas = parse_file (fp_Users);
    FILE *user_errors = fopen ("user_errors.csv", "w+");
    GSList *help = linhas;
    while (linhas != NULL){
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
        else {
            //print da linha para o documento de erros de user.
            fprintf(user_errors, "%s", (char *)linhas->data);
        }
        linhas = linhas->next;
    }
    g_slist_free_full(help, free);
}

User search_user_by_id(int id, User_Manager user_manager){
    User u = g_hash_table_lookup(user_manager->users_by_id, &id);
    return u;
}

void free_user_manager(User_Manager um){
    g_hash_table_destroy (um->users_by_id);
    free (um);
}



void responde_querie1 (FILE *fp_queries, User_Manager um){
    ssize_t nBytes;
    size_t n;
    char *line = NULL;
    int i, id;
    
    for (i = 0; (nBytes = getline (&line, &n, fp_queries)) != -1; i++){
        if (line[0] == '1'){
            id = atoi (line + 3);
            User u = search_user_by_id (id, um);
            if (u != NULL) //aka existe nos dados guardados
                print_user_info (u);
            else fprintf(stdout, "\n");
        }
      //  else fprintf(stdout, "\n");
    }
    free (line);
}
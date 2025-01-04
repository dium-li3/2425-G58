#include <ncurses.h>
#include <ctype.h>

#include "interativo.h"
#include "master_manager.h"
#include "queries.h"
#include "utils.h"
#include "output.h"
#include "parser.h"



void print_init() {
    initscr();
    start_color();

    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_GREEN, 0, 1000, 0);

    init_pair(WBa_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(RBa_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GBa_PAIR, COLOR_GREEN, COLOR_BLACK);

    bkgd(COLOR_PAIR(WBa_PAIR));

    char text[] = "=====PROGRAMA=INTERATIVO=====";
    int begin_x = (getmaxx(stdscr) / 2) - (strlen(text) / 2); //centrar o texto

    attron(A_BOLD);
    mvaddstr(0, begin_x, text);
    attroff(A_BOLD);
}


// Imprime o ecrã de saída.
int print_sair() {
    move(3, 0);
    clrtobot();
    attron(A_BLINK);
    curs_set(0);
    addstr("[Pressione qualquer tecla para fechar a janela]");
    refresh();
    return 2;
}


// Imprime a string passada e espera que o utilizador prima ENTER
void wait_for_enter(char text[]) {
    int key_buffer = -1;

    attron(A_BLINK);
    curs_set(0);
    noecho();
    
    addstr(text);
    
    while (key_buffer != ENTER) key_buffer = getch();
    
    attroff(A_BLINK);
    curs_set(1);
    echo();
    refresh();
}


void clear_to_bot_from_pos(int y, int x) {
    move(y, x);
    clrtobot();
    refresh();
}





// Imprime a mensagem de erro no carregamento dos dados.
void print_store_error() {
    attron(COLOR_PAIR(RBa_PAIR));
    addstr("\nErro no carregamento dos dados.\n\n");
    attroff(COLOR_PAIR(RBa_PAIR));

    wait_for_enter("[Prima ENTER para inserir um novo caminho]");
}


// Limpa o ecrã e lê o path introduzido do utilizador.
void get_input_path(char *buffer) {
    clear_to_bot_from_pos(3, 0);
    attron(A_BOLD);
    addstr("Introduza o caminho para os ficheiros de dados (digite 'sair' para fechar o modo interativo):\n\n");
    attroff(A_BOLD);
    getnstr(buffer, BUFSIZ-1);    //BUFSIZ-1 para poder guardar o \0
    addstr("\nA carregar dados...\n\n");
    refresh();
}


int store_data_until_correct(Master_Manager *mm, int interativo) {
    char input_buffer[BUFSIZ];
    
    get_input_path(input_buffer);
    *mm = create_master_manager();
    
    if(strcmp(input_buffer, "sair") == 0) return print_sair();
    
    char **entity_paths = pathEntities(input_buffer);
    int store = store_Entities(entity_paths, *mm, interativo);

    while(store != 0) {
        free_master_manager(*mm);
        freeEntityPaths(entity_paths);

        print_store_error();
        
        get_input_path(input_buffer);
        *mm = create_master_manager();

        if(strcmp(input_buffer, "sair") == 0) return print_sair();
        
        entity_paths = pathEntities(input_buffer);
        store = store_Entities(entity_paths, *mm, interativo);
    }
    
    freeEntityPaths(entity_paths);
    attron(COLOR_PAIR(GBa_PAIR));
    addstr("Dados carregados com sucesso.\n\n");
    attroff(COLOR_PAIR(GBa_PAIR));
    refresh();

    return store;
}





// Lê o tipo de query introduzido pelo utilizador
int input_query_type() {
    int key = -1;

    clear_to_bot_from_pos(3, 0);
    addstr("Introduza o número correspondente ao tipo da query (pressione 'f' para fechar o modo interativo).\n\n");
    noecho();

    while(!(key >= '1' && key <= '6') && key != 'f') key = getch();

    echo();
    return key;
}


// Lê se o utilizador quer mudar o separador
int input_separador() {
    int key = -1;

    printw("Deseja modificar o separador dos resultados?\n\n[s/n]");

    noecho();
    while(key != 's' && key != 'n') key = getch();
    echo();

    clear_to_bot_from_pos(7, 0);

    return (key == 's') ? 1 : 0;
}


// Verifica se uma string contém apenas números
int valid_number_as_string(char number[]) {
    int r = 1;

    if(number[0] == '\0') return 0;

    for(int i = 0; number[i] != '\0' && r; i++) r = isdigit(number[i]);

    return r;
}

void input_command_args_1(char line[]) {
    int key = -1, i;

    addstr("Deseja consultar um artista ou um utilizador?\n\n[a/u]");

    noecho();
    while(key != 'a' && key != 'u') key = getch();
    echo();

    char id_buffer[8]; //artistas ou utilizadores têm no máximo 7 dígitos no ID + 1 para o \0
    int valid_id = 0, id = -1;

    while(!valid_id) {
        clear_to_bot_from_pos(5, 0);
        
        addstr("Introduza o ID: ");
        
        for(i = 0; i < 8; i++) id_buffer[i] = '\0';
        getnstr(id_buffer, 7);
        
        valid_id = valid_number_as_string(id_buffer);
    }

    id = atoi(id_buffer);
    sprintf(line+strlen(line), " %c%07d", toupper(key), id);

    addch('\n');
}


void input_command_args_2(char line[]) {
    char buffer[86]; // 85 caracteres para o país com nome mais longo (Taumata) + 1 para o \0
    int valid_N = 0, i, key = -1;

    while(!valid_N) {
        clear_to_bot_from_pos(3, 0);

        addstr("Introduza o número de artistas que o top deve ter: ");

        for(i = 0; i < 86; i++) buffer[i] = '\0';
        getnstr(buffer, 5);

        valid_N = valid_number_as_string(buffer);
    }

    sprintf(line+strlen(line), " %s", buffer);

    addstr("\nDeseja incluir o filtro de país?\n\n[s/n]");
    noecho();
    while(key != 's' && key != 'n') key = getch();
    echo();

    clear_to_bot_from_pos(7, 0);

    if(key == 's') {
        addstr("Introduza o país: ");
        
        for(i = 0; i < 86; i++) buffer[i] = '\0';
        getnstr(buffer, 85);

        sprintf(line+strlen(line), " \"%s\"", buffer);

        addch('\n');
    }
}


void input_command_args_3(char line[]) {
    char age_buffer[4]; // cada idade tem no máximo 3 dígitos + 1 para o \0
    int valid_age = 0, i;

    while(!valid_age) {
        clear_to_bot_from_pos(3, 0);

        addstr("Introduza a idade mínima: ");

        for(i = 0; i < 4; i++) age_buffer[i] = '\0';
        getnstr(age_buffer, 3);

        valid_age = valid_number_as_string(age_buffer);
    }

    sprintf(line+strlen(line), " %s", age_buffer);

    valid_age = 0;

    while(!valid_age) {
        clear_to_bot_from_pos(5, 0);

        addstr("Introduza a idade máxima: ");

        for(i = 0; i < 4; i++) age_buffer[i] = '\0';
        getnstr(age_buffer, 3);

        valid_age = valid_number_as_string(age_buffer);
    }

    sprintf(line+strlen(line), " %s", age_buffer);

    addch('\n');
}


void input_command_args_4(char line[]) {
    int key = -1, i, valid = 0;

    addstr("Deseja incluir um intervalo?\n\n[s/n]");

    noecho();
    while(key != 's' && key != 'n') key = getch();
    echo();

    clear_to_bot_from_pos(5, 0);

    if(key == 's') {
        char date_buffer[11] = {'\0'}; // cada data deve ter 10 chars + 1 para o \0

        while(!valid) {
            clear_to_bot_from_pos(5, 0);
            
            addstr("Introduza o limite inferior do intervalo (data mais antiga): ");
            
            for(i = 0; i < 11; i++) date_buffer[i] = '\0';
            getnstr(date_buffer, 10);

            valid = valid_date(date_buffer);
        }

        sprintf(line+strlen(line), " %s", date_buffer);
        
        for(i = 0; i < 11; i++) date_buffer[i] = '\0';

        valid = 0;

        while(!valid) {
            clear_to_bot_from_pos(7, 0);

            addstr("Introduza o limite superior do intervalo (data mais recente): ");
            
            for(int i = 0; i < 11; i++) date_buffer[i] = '\0';
            getnstr(date_buffer, 10);

            valid = valid_date(date_buffer);
        }

        sprintf(line+strlen(line), " %s", date_buffer);

        addch('\n');
    }
}


void input_command_args_5(char line[]) {
    char buffer[8]; //utilizadores têm no máximo 7 dígitos no ID + 1 para o \0
    int valid = 0, id = -1, i;

    while(!valid) {
        clear_to_bot_from_pos(3, 0);
        
        addstr("Introduza o ID do utilizador alvo: ");
        
        for(i = 0; i < 8; i++) buffer[i] = '\0';
        getnstr(buffer, 7);
        
        valid = valid_number_as_string(buffer);
    }

    id = atoi(buffer);
    sprintf(line+strlen(line), " U%07d", id);

    valid = 0;

    while(!valid) {
        clear_to_bot_from_pos(5, 0);

        addstr("Introduza o número de utilizadores a recomendar: ");

        for(i = 0; i < 8; i++) buffer[i] = '\0';
        getnstr(buffer, 7);

        valid = valid_number_as_string(buffer);
    }

    sprintf(line+strlen(line), " %s", buffer);

    addch('\n');
}


void input_command_args_6(char line[]) {
    char buffer[8]; //utilizadores têm no máximo 7 dígitos no ID + 1 para o \0
    int valid = 0, id = -1, i, key = -1;

    while(!valid) {
        clear_to_bot_from_pos(3, 0);
        
        addstr("Introduza o ID do utilizador: ");
        
        for(i = 0; i < 8; i++) buffer[i] = '\0';
        getnstr(buffer, 7);
        
        valid = valid_number_as_string(buffer);
    }

    id = atoi(buffer);
    sprintf(line+strlen(line), " U%07d", id);

    valid = 0;

    while(!valid) {
        clear_to_bot_from_pos(5, 0);

        addstr("Introduza o ano: ");

        for(i = 0; i < 8; i++) buffer[i] = '\0';
        getnstr(buffer, 4);

        valid = valid_number_as_string(buffer) && atoi(buffer) <= 2024;
    }

    sprintf(line+strlen(line), " %s", buffer);

    printw("\nDeseja incluir o top de artistas mais ouvidos pelo utilizador em %s?\n\n[s/n]", buffer);
    noecho();
    while(key != 's' && key != 'n') key = getch();
    echo();

    clear_to_bot_from_pos(7, 0);

    if(key == 's') {
        valid = 0;
        
        while(!valid) {
            clear_to_bot_from_pos(7, 0);

            addstr("Introduza o tamanho do top: ");
            
            for(i = 0; i < 8; i++) buffer[i] = '\0';
            getnstr(buffer, 7);

            valid = valid_number_as_string(buffer);
        }

        sprintf(line+strlen(line), " %s", buffer);

        addch('\n');
    }
}


// Chama uma função que lê os argumentos de um comando, dependendo do tipo.
void input_command_args(int type, char line[]) {
    clear_to_bot_from_pos(3, 0);
    
    switch (type) {
        case '1':
            input_command_args_1(line);
            break;
        case '2':
            input_command_args_2(line);
            break;
        case '3':
            input_command_args_3(line);
            break;
        case '4':
            input_command_args_4(line);
            break;
        case '5':
            input_command_args_5(line);
            break;
        case '6':
            input_command_args_6(line);
            break;
    }
}

// Pergunta ao utilizador se quer ver o output no terminal
int input_display_terminal(int n_query) {
    int key = -1, current_y;

    printw("O resultado será escrito num ficheiro (command%d_output.txt).\n", n_query);
    addstr("Deseja visualizá-lo também no terminal? Resultados longos poderão não ser corretamente exibidos.\n\n[s/n]");

    noecho();
    while(key != 's' && key != 'n') key = getch();
    echo();

    current_y = getcury(stdscr);
    clear_to_bot_from_pos(current_y, 0);

    return (key == 's') ? 1 : 0;
}


int input_queries(Master_Manager mm, int interativo) {
    wait_for_enter("[Prima ENTER para prosseguir para as queries]");
    int query_type = -1, separador = -1, i, n_query = 0, terminal;
    char *query_line = malloc(sizeof(char) * BUFSIZ_QUERYLINE);
    Query q = NULL;
    Output out = NULL;
    Parser p = NULL;

    while(1) {
        for(i = 0; i < BUFSIZ_QUERYLINE; i++) query_line[i] = '\0';

        query_type = input_query_type(); //embora seja um int, o seu valor representa um char
        if(query_type == 'f') {
            free(query_line);
            remove(TEMP_FILE_PATH);
            return print_sair();
        }

        out = open_out(TEMP_FILE_PATH, ' ', 0);

        separador = input_separador();
        if(separador == 1) sprintf(query_line, "%cS", query_type);
        else query_line[0] = query_type;

        printw("Comando atual: %s\n\n", query_line);
        wait_for_enter("[Prima ENTER para inserir os argumentos do comando]");

        input_command_args(query_type, query_line);
        printw("Comando atual: %s\n\n", query_line);
        output_geral(&query_line, 1, out); n_query++;
        close_output(out);

        terminal = input_display_terminal(n_query);
        if(terminal != 0) set_terminal_true(out);  
        
        wait_for_enter("[Prima ENTER para obter o resultado]");
        clear_to_bot_from_pos(3, 0);
        
        q = create_query();
        p = open_parser(TEMP_FILE_PATH);
        read_query_line(p, q);
        answer_query(q, mm, n_query, NULL, terminal);

        wait_for_enter("\n[Pressione ENTER para prosseguir]");

        close_parser(p);
        free_query(q);
    }

    free(query_line);
    remove(TEMP_FILE_PATH);
    return 0;
}
// /home/marco/2ºAno/LI3/Projeto/Dados-fase2-small/dataset/com_erros/

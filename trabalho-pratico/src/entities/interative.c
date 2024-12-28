#include <ncurses.h>
#include <stdlib.h>

void load_dataset(char *path,int yMax, int xMax)
{
    int r = 0;
    while (!r)
    {
        WINDOW *win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);

        wprintw(win, "Introduza o caminho para o dataset:");

        curs_set(1);
        wgetnstr(win, path, 100);
        curs_set(0);

        FILE *file = fopen(path, "r");

        if (file != NULL)
        {
            r = 1;
            fclose(file);
        }
        else
        {
            r = 0;
            werase(win);
            wprintw(win, "Caminho inválido.");
        }

        free(path);
    }
}

int main()
{
    // começa o n_curses
    initscr();

    char *path = malloc(sizeof(char) * 100);

    noecho();
    cbreak();
    curs_set(0);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    load_dataset(path,yMax,xMax);

    // WINDOW *menuwin = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
    // box(menuwin, 0, 0);
    // refresh();
    // wrefresh(menuwin);

    // // permite que o menuwin receba setas
    // keypad(menuwin, true); 

    // char *queries[6] = {
    //     "Query 1",
    //     "Query 2",
    //     "Query 3",
    //     "Query 4",
    //     "Query 5",
    //     "Query 6",
    // };

    // int choice;
    // int highlight = 0;

    // while (1)
    // {
    //     for (int i = 0; i < 5; i++)
    //     {
    //         if (i == highlight)
    //             wattron(menuwin, A_REVERSE);
    //         mvwprintw(menuwin, i + 1, 1, "%s", queries[i]);
    //         wattroff(menuwin, A_REVERSE);
    //     }
    //     choice = wgetch(menuwin);

    //     switch (choice)
    //     {
    //     case KEY_UP:
    //         highlight--;
    //         if (highlight == -1)
    //             highlight = 0;
    //         break;
    //     case KEY_DOWN:
    //         highlight++;
    //         if (highlight == 6)
    //             highlight = 5;
    //         break;
    //     default:
    //         break;
    //     }

    //     if (choice == 10)
    //         break;
    // }

    getch();

    endwin();
}
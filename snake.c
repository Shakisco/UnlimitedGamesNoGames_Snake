#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void apple()
{
    int x = (rand() % 41) + 61;
    int y = (rand() % 19) + 1;
    mvprintw(y, x, "A");
    refresh();
}

void character()
{
    int x = 69;
    int y = 10;
    int prev_x = x;
    int prev_y = y;

    int dx = 1;
    int dy = 0;

    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    mvprintw(prev_y, prev_x, "@");
    mvprintw(10, 69, "                         ");
    mvprintw(9, 69, "                     "); 
    refresh();

    while(1){
        int input = getch();
        if(input != ERR)
        {
            switch(input)
            {
                case 'w': case 'W':
                    dx = 0, dy = -1;
                    usleep(160000);
                    break;
                case 's': case 'S':
                    dx = 0, dy = 1;
                    usleep(160000);
                    break;
                case 'a': case 'A':
                    dx = -1, dy = 0;
                    usleep(80000);
                    break;
                case 'd': case 'D':
                    dx = 1, dy = 0;
                    usleep(80000);
                    break;
                case 'q': case 'Q':
                    endwin();
                    exit(0);
            }
        }

        prev_x = x;
        prev_y = y;

        x += dx;
        y += dy;

        if(x <= 60 || x >= 102 || y <= 0 || y >= 20){
            mvprintw(prev_y, prev_x, " ");
            refresh();

            nodelay(stdscr, FALSE);
            start_color();
            init_pair(4, COLOR_YELLOW, COLOR_BLACK);
            attron(COLOR_PAIR(4));
            mvprintw(10, 66, "Game Over! Press any key to exit.");
            refresh();

            getch();
            endwin();
            exit(0);
        }

        mvprintw(prev_y, prev_x, " ");
        mvprintw(y, x, "@");
        refresh();

        usleep(80000);
    }
}

void map()
{
    clear();

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    
    int x, y, j, i;
    attron(COLOR_PAIR(2));
    for(j = 60; j <= 100; j++)
    {
        for(i = 0; i <= 20; i++)
        {
            mvprintw(i, j + 1, " ");
        }
    }
    attron(COLOR_PAIR(1));
    for(x = 60; x <= 100; x++)
    {
        mvprintw(0, x + 1, "-");
        mvprintw(20, x + 1, "-");
        usleep(60000);
        refresh();
    }
    for(y = 0; y <= 20; y++)
    {
        mvprintw(y, 60, "|");
        mvprintw(y, 102, "|");
        usleep(60000);
        refresh();
    }

    attron(COLOR_PAIR(3));
    mvprintw(9, 69, "Use WASD to move");
    mvprintw(10, 69, "Press any key to start... ");

    noecho();
    char start = getch();
    if(start)
    {
        apple();
        character();
    }
}

void startup()
{
    clear();
    mvprintw(5, 5, "Starting....");
    refresh();
    sleep(1);
    map();

}

int main(){

    initscr();
    clear();
    
    srand((unsigned int)time(NULL));
    startup();

    endwin();
    return 0;
}
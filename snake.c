#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void character()
{
    int x = 69;
    int y = 10;
    int prev_x;
    int prev_y;
    int snake_length = 5;

    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    mvprintw(9, 69, "Use WASD to move");
    mvprintw(y, x, "@");
    refresh();

    while(1){
        int input = getch();
        if(input != ERR){
            switch(input)
            {
                case 'w':
                case 'W':  
                    y--;
                    break;
                case 's':
                case 'S':
                    y++;
                    break;
                case 'a':
                case 'A':
                    x--;
                    break;
                case 'd':
                case 'D':
                    x++;
                    break;
                case 'q':
                case 'Q':
                    endwin();
                    exit(0);
            }
            
            mvprintw(prev_y, prev_x, " ");
            mvprintw(y, x, "@");
            refresh();
            prev_x = x;
            prev_y = y;
        }
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
        usleep(80000);
        refresh();
    }
    for(y = 0; y <= 20; y++)
    {
        mvprintw(y, 60, "|");
        mvprintw(y, 102, "|");
        usleep(80000);
        refresh();
    }

    attron(COLOR_PAIR(3));
    mvprintw(10, 69, "Press any key to start... ");
    
    char start = getch();
    if(start)
    {
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
    
    startup();

    endwin();
    return 0;
}
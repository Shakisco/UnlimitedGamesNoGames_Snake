#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void character()
{
    int x = 69;
    int y = 10;
    int snake_length = 5;
    mvprintw(10, 69, " ");
    mvprintw(9, 69, "Use WASD to move");
    mvprintw(10, 69, "@");
    while(1){
        char input = getch();
        switch(input)
        {
            case 'w':  
                y--;
                break;
            case 's':
                y++;
                break;
            case 'a':
                x--;
                break;
            case 'd':
                x++;
                break;
            case 'q':
                endwin();
                exit(0);
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
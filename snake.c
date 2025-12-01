#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define buffer 100
#define MAX_SNAKE 40

int apple_x = -1;
int apple_y = -1;
int counter = 0;

void winScreen()
{
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(10, 78, "You Win");

    attron(COLOR_PAIR(3));
    mvprintw(11, 71, "Press any key to exit");
    refresh();

    nodelay(stdscr, FALSE);
    getch();
    endwin();
    exit(0);
}

// Author: Matthew; Function to update and display the scoreboard
void scoreboard(int counter)
{
    mvprintw(0, 60, "Score to beat: 40");
    mvprintw(0, 94, "Score: %d", counter);
    if(counter == MAX_SNAKE)
    {
        winScreen();
    }
    refresh();
}

// Author: Matthew; Function to spawn an apple at a random position within the game boundaries
void apple()
{
    apple_x = (rand() % 41) + 61;
    apple_y = (rand() % 19) + 2;
    mvprintw(apple_y, apple_x, "A");

    refresh();
}

// Author: Matthew; Function to handle game over scenario
void fail(int seg_y[], int seg_x[])
{
    mvprintw(seg_y[0], seg_x[0], " ");
    refresh();

    nodelay(stdscr, FALSE);
    
    init_pair(4, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    mvprintw(10, 66, "Game Over! Press any key to exit.");
    refresh();

    getch();
    endwin();
    exit(0);
}

// Author: Shakil; Function to update the snake's position on the screen
void shift_snake(int seg_x[], int seg_y[], int *length, int dx, int dy)
{
    int new_x = seg_x[0] + dx;
    int new_y = seg_y[0] + dy;

    mvprintw(seg_y[*length - 1], seg_x[*length - 1], " ");

    for (int i = *length - 1; i > 0; --i) {
        seg_x[i] = seg_x[i - 1];
        seg_y[i] = seg_y[i - 1];
    }

    seg_x[0] = new_x;
    seg_y[0] = new_y;

    for (int i = 1; i < *length; ++i) {
        mvprintw(seg_y[i], seg_x[i], "o");
    }
    mvprintw(seg_y[0], seg_x[0], "O");
    refresh();
}

// Author: Shakil; Function to initialize and control the snake movement, input, and collision
void character()
{

    int x = 69;
    int y = 10;
    
    static int seg_x[buffer];
    static int seg_y[buffer];

    int length = 3;

    seg_x[0] = x;
    seg_y[0] = y;
    seg_x[1] = x - 1;
    seg_y[1] = y;
    seg_x[2] = x - 2;
    seg_y[2] = y;

    int dx = 1;
    int dy = 0;

    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    mvprintw(seg_y[2], seg_x[2], "o");
    mvprintw(seg_y[1], seg_x[1], "o");
    mvprintw(seg_y[0], seg_x[0], "O");

    mvprintw(10, 69, "                         ");
    mvprintw(9, 69, "                     "); 
    refresh();

    while(1){
        int input = getch();
        switch(input)
        {
            case 'w': case 'W':
                dx = 0, dy = -1;
                break;
            case 's': case 'S':
                dx = 0, dy = 1;
                break;
            case 'a': case 'A':
                dx = -1, dy = 0;
                break;
            case 'd': case 'D':
                dx = 1, dy = 0;
                break;
            case 'q': case 'Q':
                endwin();
                exit(0);
        }

        if(seg_x[0] == apple_x && seg_y[0] == apple_y)
        {
            apple();
            counter++;
            scoreboard(counter);
        }

        if(seg_x[0] + dx <= 60 || seg_x[0] + dx >= 102 || seg_y[0] + dy <= 1 || seg_y[0] + dy >= 21){
            fail(seg_y, seg_x);
        }

        shift_snake(seg_x, seg_y, &length, dx, dy);

        if(dy != 0)
        {
            usleep(160000);
        }
        else
        {
            usleep(100000);
        }
    }
}

// Author: Matthew; Function to draw the game map and start the game
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
        for(i = 1; i <= 21; i++)
        {
            mvprintw(i, j + 1, " ");
        }
    }
    attron(COLOR_PAIR(1));
    for(x = 60; x <= 100; x++)
    {
        mvprintw(1, x + 1, "-");
        mvprintw(21, x + 1, "-");
        usleep(60000);
        refresh();
    }
    for(y = 1; y <= 21; y++)
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
        scoreboard(counter);
        apple();
        character();
    }
}
// Author: Matthew; Function to display startup message and initialize the game
void startup()
{
    clear();
    mvprintw(5, 5, "Starting....");
    refresh();
    sleep(1);
    map();

}
// Author: Matthew; Main function to initialize curses and start the game
int main(){

    initscr();
    clear();
    
    srand((unsigned int)time(NULL));
    startup();

    endwin();
    return 0;
}
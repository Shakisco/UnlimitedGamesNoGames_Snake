#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define buffer 1000
#define MAX_SNAKE 200

int food_x, food_x1, food_x2;
int food_y, food_y1, food_y2;
int food1 = 1, food2 = 1, food3 = 1;
int counter = 0;
int length = 3;
int foodAmt = 0;

void startup();

// Author: Matthew; Function to display the win screen and exit the game
void winScreen()
{
    nodelay(stdscr, FALSE);

    attron(COLOR_PAIR(5));
    mvprintw(10, 78, "You Win");
    
    attron(COLOR_PAIR(3));
    mvprintw(11, 67, "Press R to restart or Q to quit.");
    refresh();
    
    int choice;
    while (1) {
        choice = getch();
        if (choice == 'r' || choice == 'R') {
            counter = 0;
            length = 3;
            food1 = food2 = food3 = 1;
            foodAmt = 0;
            startup();
        } 
        else if (choice == 'q' || choice == 'Q') {
            endwin();
            exit(0);
        }
    }
}

// Author: Matthew; Function to update and display the scoreboard
void scoreboard(int counter)
{
    attron(COLOR_PAIR(7));
    mvprintw(0, 60, "Score to beat: %d", MAX_SNAKE);
    mvprintw(0, 94, "Score: %d", counter);
    if(counter == MAX_SNAKE)
    {
        winScreen();
    }
    refresh();
}

// Author: Matthew; Function to spawn 3 foods at random positions within the game boundaries
void food(int *length, int seg_y[], int seg_x[])
{
    attron(COLOR_PAIR(5));
    while(foodAmt < 3)
    {
        if(food1)
        {
            food_x = (rand() % 41) + 61;
            food_y = (rand() % 19) + 2;
            food1 = 0;
            foodAmt++;
        }
        if(food2)
        {
            food_x1 = (rand() % 41) + 61;
            food_y1 = (rand() % 19) + 2;
            food2 = 0;
            foodAmt++;
        }
        if(food3)
        {
            food_x2 = (rand() % 41) + 61;
            food_y2 = (rand() % 19) + 2;
            food3 = 0;
            foodAmt++;
        }

        for(int i = 0; i < *length; ++i) {
        if(seg_y[i] == food_y && seg_x[i] == food_x )
        {
            food1 = 1;
            foodAmt--;
            food(length, seg_y, seg_x);
        }
        else if(seg_y[i] == food_y1 && seg_x[i] == food_x1)
        {
            food2 = 1;
            foodAmt--;
            food(length, seg_y, seg_x);
        }
        else if(seg_y[i] == food_y2 && seg_x[i] == food_x2)
        {
            food3 = 1;
            foodAmt--;
            food(length, seg_y, seg_x);
        }
        else
        {
            mvprintw(food_y, food_x, "@");
            mvprintw(food_y1, food_x1, "@");
            mvprintw(food_y2, food_x2, "@");
        }
        }
    }
    refresh();
}

// Author: Matthew; Function to handle game over scenario
void fail(int *length, int seg_y[], int seg_x[])
{
    for(int i = 0; i < *length; i++)
    {
        mvprintw(seg_y[i], seg_x[i], " ");
        refresh();
        usleep(200000);
    }

    nodelay(stdscr, FALSE);
    
    attron(COLOR_PAIR(4));
    mvprintw(10, 78, "Game Over!");
    mvprintw(11, 67, "Press R to restart or Q to quit.");
    sleep(1);
    refresh();

    int choice;
    while (1) {
        choice = getch();
        if (choice == 'r' || choice == 'R') {
            counter = 0;
            food1 = food2 = food3 = 1;
            foodAmt = 0;
            startup();
        } 
        else if (choice == 'q' || choice == 'Q') {
            endwin();
            exit(0);
        }
    }
}

// Author: Shakil; Function to check for self-collision of the snake
int self_collision(int seg_x[], int seg_y[], int length)
{
    for (int i = 1; i < length; i++){
        if (seg_x[0] == seg_x[i] && seg_y[0] == seg_y[i]){
            return 1;
        }
    }
    return 0;
}

// Author: Shakil; Function to update the snake's position on the screen
void shift_snake(int seg_x[], int seg_y[], int *length, int dx, int dy, int ate_food)
{
    int new_x = seg_x[0] + dx;
    int new_y = seg_y[0] + dy;

    attron(COLOR_PAIR(6));
    if(!ate_food)
    {
        mvprintw(seg_y[*length - 1], seg_x[*length - 1], " ");
    }

    for (int i = *length - 1; i > 0; --i) {
        seg_x[i] = seg_x[i - 1];
        seg_y[i] = seg_y[i - 1];
    }

    seg_x[0] = new_x;
    seg_y[0] = new_y;

    mvprintw(seg_y[1], seg_x[1], "o");
    
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

    food(&length, seg_y, seg_x);

    int ate_food = 0;

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

    attron(COLOR_PAIR(6));
    mvprintw(seg_y[2], seg_x[2], "o");
    mvprintw(seg_y[1], seg_x[1], "o");
    mvprintw(seg_y[0], seg_x[0], "O");

    mvprintw(10, 69, "                         ");
    mvprintw(9, 69, "                     "); 
    mvprintw(11, 69, "                         ");
    refresh();

    while(1){
        int input = getch();
        switch(input)
        {
            case 'w': case 'W':
                if(dy == 0)
                    dx = 0, dy = -1;
                break;
            case 's': case 'S':
                if(dy == 0)
                    dx = 0, dy = 1;
                break;
            case 'a': case 'A':
                if(dx == 0)
                    dx = -1, dy = 0;
                break;
            case 'd': case 'D':
                if(dx == 0)
                    dx = 1, dy = 0;
                break;
            case 'q': case 'Q':
                endwin();
                exit(0);
        }

        if(seg_x[0] == food_x && seg_y[0] == food_y)
        {
            foodAmt--;
            food1 = 1;
            food2 = 0;
            food3 = 0;
            food(&length, seg_y, seg_x);
            counter++;
            scoreboard(counter);
            ate_food = 1;
            length++;

        }
        else if(seg_x[0] == food_x1 && seg_y[0] == food_y1)
        {
            foodAmt--;
            food1 = 0;
            food2 = 1;
            food3 = 0;
            food(&length, seg_y, seg_x);
            counter++;
            scoreboard(counter);
            ate_food = 1;
            length++;
        }
        else if(seg_x[0] == food_x2 && seg_y[0] == food_y2)
        {
            foodAmt--;
            food1 = 0;
            food2 = 0;
            food3 = 1;
            food(&length, seg_y, seg_x);
            counter++;
            scoreboard(counter);
            ate_food = 1;
            length++;
        }

        if(seg_x[0] + dx <= 60 || seg_x[0] + dx >= 102 || seg_y[0] + dy <= 1 || seg_y[0] + dy >= 21){
            fail(&length, seg_y, seg_x);
        }

        shift_snake(seg_x, seg_y, &length, dx, dy, ate_food);
        if(self_collision(seg_x, seg_y, length)){
            fail(&length, seg_y, seg_x);
        }
        ate_food = 0;

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
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);
    
    int x, y, j, i;
    attron(COLOR_PAIR(2));
    for(j = 0; j <= 500; j++)
    {
        for(i = 0; i <= 100; i++)
        {
            mvprintw(i, j, " ");
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
    mvprintw(10, 69, "Press Q to quit");

    noecho();
    nodelay(stdscr, TRUE);
    while(1){
        mvprintw(11, 69, "Press any key to start... ");
        refresh();
        usleep(600000);
        mvprintw(11, 69, "                         ");
        refresh();
        usleep(600000);
 
        char start = getch();
        if(start == 'q')
        {
            endwin();
            exit(0);
        }
        if(start != ERR)
        {
            break;
        }
        
    }

    scoreboard(counter);
    character();
}

// Author: Matthew; Function to display startup message and initialize the game
void startup()
{
    clear();
    length = 3;
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
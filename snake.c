#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>


void map()
{
    clear();
    for(int i = 0; i <= 40; i++)
    {
        mvprintw(0, i + 1, "-");
        mvprintw(20, i + 1, "-");
        usleep(80000);
        refresh();
    }
    for(int i = 0; i <= 20; i++)
    {
        mvprintw(i, 0, "|");
        mvprintw(i, 42, "|");
        usleep(80000);
        refresh();
    }

}

void startup(int signum)
{
    mvprintw(5, 5, "Starting....");
    refresh();
    sleep(3);
    map();


}



int main(){

    initscr();
    clear();
    
    signal(SIGALRM, startup);

    alarm(1);

    getchar();
    endwin();

    return 0;
}
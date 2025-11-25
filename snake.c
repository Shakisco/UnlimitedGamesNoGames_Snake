#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>

void startup()
{
    mvprintw(5, 5, "Starting....");
    refresh();
    sleep(3);

}

int main(){

    initscr();
    clear();
    
    signal(SIGALRM, startup);

    alarm(1);

    return 0;
}
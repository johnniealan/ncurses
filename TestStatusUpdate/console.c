#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <curses.h>

#define TST_RUNNING  1
#define TST_PASSED   2
#define TST_FAILED   3
#define TST_TIMEOUT  4
#define TST_ERROR    5

typedef struct{
  WINDOW *mainwin;
  int yCordinate;
  int xCordinate;
  int tstIdx;
}sConsolePriv_t;

sConsolePriv_t sConsolePriv;
char *tstStats[] = {"","RUNNING","PASSED","FAILED","TIMEOUT","ERROR"};

int consoleInit(void)
{
    initscr();
    refresh();
    sConsolePriv.mainwin = newwin(LINES,COLS,0,0);
    noecho();
    nodelay(sConsolePriv.mainwin, true);
    scrollok(sConsolePriv.mainwin,true);
    use_default_colors();
    start_color();
    init_pair(TST_RUNNING, COLOR_BLACK, COLOR_WHITE);
    init_pair(TST_PASSED, COLOR_BLACK, COLOR_GREEN);
    init_pair(TST_FAILED, COLOR_WHITE, COLOR_RED);
    init_pair(TST_TIMEOUT, COLOR_WHITE, COLOR_YELLOW);
    init_pair(TST_ERROR, COLOR_WHITE, COLOR_RED);
    getmaxyx(sConsolePriv.mainwin, sConsolePriv.yCordinate, sConsolePriv.xCordinate);
    mvwprintw(sConsolePriv.mainwin,0,1,"TEST CASE");
    mvwprintw(sConsolePriv.mainwin,0,sConsolePriv.xCordinate/4,"TIME TAKEN");
    mvwprintw(sConsolePriv.mainwin,0,sConsolePriv.xCordinate/1.5,"TEST STATUS");
    mvwhline(sConsolePriv.mainwin,1,1,'-',(sConsolePriv.xCordinate/1.5 + 11)); 
    wrefresh(sConsolePriv.mainwin);
    wattron(sConsolePriv.mainwin,A_NORMAL);
    wrefresh(sConsolePriv.mainwin);
    sConsolePriv.tstIdx = 1;
}
int consoleExit(void)
{
    delwin(sConsolePriv.mainwin);
    endwin();    
}
int fnAddTestNameToScreen(char* tstName, int timeout, int status)
{
    int iRet;
    if ( status <= 0 ) return -1;

    iRet = ++sConsolePriv.tstIdx;
    mvwprintw(sConsolePriv.mainwin,sConsolePriv.tstIdx,1,"%s",tstName);
    mvwprintw(sConsolePriv.mainwin,sConsolePriv.tstIdx,sConsolePriv.xCordinate/4,"%f",timeout);
    wattron(sConsolePriv.mainwin,COLOR_PAIR(status));
    mvwprintw(sConsolePriv.mainwin,sConsolePriv.tstIdx,sConsolePriv.xCordinate/1.5,"%s",tstStats[status]);
    wattroff(sConsolePriv.mainwin,COLOR_PAIR(status));
    use_default_colors();
    wrefresh(sConsolePriv.mainwin);
    if ( sConsolePriv.tstIdx == sConsolePriv.yCordinate )
    {
          wmove(sConsolePriv.mainwin,0,0);
          wclear(sConsolePriv.mainwin);
          mvwprintw(sConsolePriv.mainwin,0,1,"TEST CASE");
          mvwprintw(sConsolePriv.mainwin,0,sConsolePriv.xCordinate/4,"TIME TAKEN");
          mvwprintw(sConsolePriv.mainwin,0,sConsolePriv.xCordinate/1.5,"TEST STATUS");
          mvwhline(sConsolePriv.mainwin,1,1,'-',(sConsolePriv.xCordinate/1.5 + 11)); 
          sConsolePriv.tstIdx = 1;
    }
   return iRet;
}
int fnUpdateTstStatusToScreen(int tstIdx, int timeout, int status)
{
    mvwhline(sConsolePriv.mainwin,tstIdx,sConsolePriv.xCordinate/4,' ',sConsolePriv.xCordinate-sConsolePriv.xCordinate/4);
    mvwprintw(sConsolePriv.mainwin,tstIdx,sConsolePriv.xCordinate/4,"%d",timeout);
    wattron(sConsolePriv.mainwin,COLOR_PAIR(status));
    mvwprintw(sConsolePriv.mainwin,tstIdx,sConsolePriv.xCordinate/1.5,"%s",tstStats[status]);
    wattroff(sConsolePriv.mainwin,COLOR_PAIR(status));
    use_default_colors();
    wrefresh(sConsolePriv.mainwin);
}
int main()
{
    int i = 0,stat=0;
    consoleInit();
    for ( i=0;i<100;i++){
   	stat = fnAddTestNameToScreen("Temp",i,TST_RUNNING);
        sleep(1);
        fnUpdateTstStatusToScreen(stat,i+200,i%5+1);
    }
    getch();
    consoleExit();
}

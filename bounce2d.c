#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#define MESSAGE "o"
#define BLANK " "
#define TICKS_PER_SEC 50

void move_msg(int);
int set_ticker(int);

int row;
int col;
int dir;
int dir2;
int xspd,yspd;

int main()
{
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	int delay;
	int ndelay;
	int c;
	void move_msg(int);

	initscr();
	crmode();
	noecho();
	clear();

	row = 10;
	col = 20;
	delay = 1000/TICKS_PER_SEC;
	dir = 1;
	dir2= 1;
	xspd= yspd = 1;
	move(row,col);
	addstr(MESSAGE);
	signal(SIGALRM, move_msg);
	set_ticker(delay);
	while(1)
	{
		c = getch();
		if( c== 'Q') break;
		if( c== 'f'){ xspd++;}
		if( c== 'F'){ yspd++;}
		if( c== 's' && xspd>0){ xspd--;}
		if( c== 'S' && yspd>0){ yspd--;}
	}
	endwin();
	return 0;
}

void move_msg(int signum)
{	

	signal(SIGALRM,move_msg);
	move(row,col);
	addstr(BLANK);

	for(int i=0; i<xspd; i++)
	col += dir;
	for(int i=0; i<yspd;i++)
	row += dir2;
	
	move(row,col);
	if(row>5|| row+strlen(MESSAGE)<20)
		if(col>10||col+strlen(MESSAGE)<70){
			addstr(MESSAGE);
		}
	move(LINES -1,COLS-1);
	refresh();
		
	
	if(dir == -1 && col<=10)
	{
		dir = 1;
	}
	else if (dir == 1 && col+strlen(MESSAGE) >=70)
	{	
		dir = -1;
	}
	if(dir2 == -1 && row<=5)
	{
		dir2 = 1;
	}
	else if (dir2 == 1&& row+strlen(MESSAGE) >=20)
	{
		dir2 = -1;
	}
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs/ 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset,NULL);
}

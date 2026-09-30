#include "global.h"

#define CON_OPENER ">"
#define CON_MAX_LEN 4096

#include <stdio.h>
#ifndef __linux
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
int getch()
{
    int ch;
    struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
#endif
/*
int consoleModule(void* cmdList) {
	//static char conBuffer[CON_MAX_LEN] = { };
	int conSym = 0;
	debug("\n"CON_OPENER" ");

	for (;;) {
		if (kbhit()) {
			conSym = getch();
			if (conSym == 27) break;
			else if (conSym == 32) break;
			else {
				debug("%c", conSym);
			}
		sleep(100);
		}
	}


	return 0;
}
*/

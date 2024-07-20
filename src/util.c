#include "include/util.h"
#include <stdarg.h>

// return the time elapsed in ms since startTime
double _timeDiff(clock_t startTime)
{
	return difftime(clock(), startTime);
}
//**************************************************************************************

// source: https://www.w3resource.com/c-programming/time/c-asctime.php
char *getCurrentTimeAndDate()
{
	static struct tm *new_time;
	time_t lctime;

	// Get the time in seconds
	time(&lctime);

	// Convert it to the structure tm
	new_time = localtime(&lctime);

	return asctime(new_time);
}
//**************************************************************************************

#ifdef _WIN32
void gotoxy(int y, int x)
{ 
	COORD coord = {0, 0}; // position cursor
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//****************************************************************************************

void showCursor(BOOL condition)
{ // hide or show cursor
	if (condition == FALSE)
	{
		CONSOLE_CURSOR_INFO cursor = {1, FALSE};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	}
	else
	{
		CONSOLE_CURSOR_INFO cursor = {1, TRUE};
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	}
}
//****************************************************************************************

#else // linux -> uses ncurses

void gotoxy(int y, int x)
{
	move(y, x);
}
//****************************************************************************************

void showCursor(BOOL condition)
{
	if (condition)
		curs_set(1); // show
	else
		curs_set(0); // hide
}
//****************************************************************************************

int kbhit(void)
{
	int ch = getch();

	if (ch != ERR)
	{
		ungetch(ch);
		return 1;
	}
	else
	{
		return 0;
	}
}
//****************************************************************************************

#endif

void printPrompt(char *message, int x, int y)
{
	gotoxy(x, y);
	_printf("%s", message);
	refresh();
	clrbuf();
}
//****************************************************************************************

void clear_write(int clear_size, int x, int y)
{
	gotoxy(x, y);
	for(int i = 0; i < clear_size; i++) _printf(" ");
	refresh();
	gotoxy(x, y);
}
//****************************************************************************************

void _printf(const char *format, ...)
{

	va_list args; // points to each unnamed arg in turn
	char buffer[BUFSIZ];

	va_start(args, format); // make ap point to 1st unnamed arg

	vsnprintf(buffer, sizeof buffer, format, args);

#ifdef _WIN32
	printf("%s", buffer);
#else
	printw("%s", buffer); // uses ncurses lib
	refresh();
#endif

	va_end(args);
}
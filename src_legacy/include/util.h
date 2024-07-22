#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE 100

//Keys
#define SPACE 32 //32 = space in ascii
#define ESC 27 //27 = esc in ascci

#ifdef _WIN32

#define UP 72 //72 = H in ascci = arrow up
#define DOWN 80 //80 = P in ascci = arrow down
#define ENTER 13 //13 = return in ASCII 
#define BACKSPACE 8
#define WINDOWS_OS 1

#else

#define UP 65
#define DOWN 66
#define ENTER 10 //return 
#define BACKSPACE 127 //delete
#define WINDOWS_OS 0

#endif



//https://www.youtube.com/watch?v=mYBr-Yb70Z4
//https://stackoverflow.com/questions/3521209/making-c-code-plot-a-graph-automatically

//-------------- WINDOWS --------------
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
//#define _sleep(a) Sleep(a) //ms
#define clrscr() system("cls")
#define clrbuf() fflush(stdin)
#define refresh()
#define timeDiff(a) _timeDiff(a)

#define close_serial_port(a) CloseHandle(a)

#define FILE_SEPARATOR "\\\\"
#define _NULL NULL

//--------------- LINUX ---------------
#else
#include <stdio_ext.h>
#include <ncurses.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/file.h>


#define _popen(a,b) popen(a,b)
#define _pclose(a) pclose(a)
#define close_serial_port(a) close(a)


typedef uint8_t BOOL;
typedef int HANDLE;
#define Sleep(a) for(int ii = 0; ii < 1000; ii++) {usleep(a);} //ms
#define clrscr() wclear(stdscr) //printf("\e[1;1H\e[2J")
#define clrbuf() __fpurge(stdin)
#define timeDiff(a) _timeDiff(a)/1000
#define FILE_SEPARATOR "/"


#define _NULL 0
int kbhit(void);

#endif

//-------------------------------------


double _timeDiff(clock_t startTime);
char* getCurrentTimeAndDate();
void gotoxy (int y, int x);
void showCursor(BOOL condition);
void printPrompt(char* message, int x, int y);
void _printf(const char *format, ...);
void clear_write(int clear_size, int x, int y);

#endif
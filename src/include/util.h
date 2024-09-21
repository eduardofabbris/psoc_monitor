#ifndef UTIL_H
#define UTIL_H

// ------------> INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

// ------------> DEFINES
// TODO: 100?? ->probably Im not using
#define MAX_BUFFER_SIZE 100

// Special ASCII keys
#define SPACE 32
#define ESC 27

//------------> PROTOTYPES
double time_diff(clock_t start_t);
char *get_timeinfo();
void gotoxy(int x, int y);
void hide_cursor(int state);

#ifdef _WIN32 // @windows

//------------> Windows INCLUDES
#include <windows.h>
#include <conio.h>

// ------------> Windows DEFINES
#define FILE_SEPARATOR "\\\\"
#define msleep(a) Sleep(a)
#define clrscr() system("cls")

#define clrbuf() fflush(stdin)
#define close_serial_port(a) CloseHandle(a)
#define _NULL NULL

// Special ASCII keys
#define UP 72
#define DOWN 80
#define ENTER 13
#define BACKSPACE 8

#define LINUX_EN 0 // Linux enabled flag
#define WINDOWS_EN 1 // Windows enabled flag

#else // @linux

//#include <fcntl.h> // Contains file controls like O_RDWR
//#include <errno.h> // Error integer and strerror() function
//#include <sys/file.h>

//------------> Linux INCLUDES
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
//#include <stdio_ext.h> // Add __fpurge

//------------> Linux DEFINES
#define FILE_SEPARATOR "/"
#define msleep(a) for(int ii = 0; ii < 1000; ii++) {usleep(a);}
//#define clrscr() printf("\e[1;1H\e[2J")
//#define clrscr() printf("\033[2J")
#define clrscr() system("clear") 

// Special ASCII keys
#define UP 65
#define DOWN 66
#define ENTER 10
#define BACKSPACE 127

#define LINUX_EN 1 // Linux enabled flag
#define WINDOWS_EN 0 // Windows enabled flag

//------------> Linux TYPEDEFS
typedef uint8_t BOOL;
typedef int HANDLE;

//------------> Linux PROTOTYPES
int kbhit();
void set_nonblock(int state);


#define clrbuf() __fpurge(stdin)
#define _NULL 0
#endif

#endif

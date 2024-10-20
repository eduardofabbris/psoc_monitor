/*******************************************************************************
* @filename: util.h
*
* @brief:
*
*
*
*
********************************************************************************/
#ifndef UTIL_H
#define UTIL_H

/**********************************************
 * Includes
 *********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

/**********************************************
 * Defines
 *********************************************/

// Special ASCII keys
#define SPACE 32
#define ESC 27

/**********************************************
 * Function Prototypes
 *********************************************/

// Time
long long get_clock();
double time_diff(uint64_t start_t);
char *get_timeinfo(time_t timestamp);

// Terminal
void gotoxy(int x, int y);
void hide_cursor(int state);
//void debug_print(const char *msg);

char get_char();

#ifdef _WIN32
// @windows

/**********************************************
 * WINDOWS Includes
 *********************************************/

#include <windows.h>
#include <conio.h>

/**********************************************
 * WINDOWS Defines
 *********************************************/

// Miscellaneous
#define FILE_SEPARATOR "\\\\"
#define msleep(a) Sleep(a)
#define clrscr() system("cls")
#define clrbuf() fflush(stdin)
#define close_serial_port(a) CloseHandle(a)
//#define _NULL NULL

// ASCII keys
#define UP 72
#define DOWN 80
#define ENTER 13
#define BACKSPACE 8

// Flags
#define LINUX_EN 0      // Linux enabled flag
#define WINDOWS_EN 1    // Windows enabled flag

#else
// @linux

/**********************************************
 * LINUX Includes
 *********************************************/

//#include <fcntl.h> // Contains file controls like O_RDWR
//#include <sys/file.h>

#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
//#include <stdio_ext.h> // Add __fpurge TODO: do i need?

/**********************************************
 * LINUX Defines
 *********************************************/

// Miscellaneous
#define INVALID_HANDLE_VALUE -1
#define FILE_SEPARATOR "/"
#define msleep(a) for(int ii = 0; ii < 1000; ii++) {usleep(a);}
#define close_serial_port(a) close(a)
#define clrscr() system("clear")
#define clrbuf() __fpurge(stdin)

// ASCII keys
#define UP 65
#define DOWN 66
#define ENTER 10
#define BACKSPACE 127

// Flags
#define LINUX_EN 1      // Linux enabled flag
#define WINDOWS_EN 0    // Windows enabled flag

/**********************************************
 * LINUX Typedefs
 *********************************************/

typedef uint8_t BOOL;
typedef int HANDLE;

/**********************************************
 * LINUX Function Prototypes
 *********************************************/

int kbhit();
void set_nonblock(int state);

#endif

#endif // UTIL_H

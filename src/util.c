/**
 ******************************************************************************
 * @file    util.h
 * @date    21-July-2024
 * @brief   Miscellaneous functions for PSoc Monitor
 *
 ******************************************************************************
 */
#include "include/util.h"

/*void debug_print(const char *msg)
{
    gotoxy(20, 0); 
    printf("Debug output: %s", msg); 
    fflush(stdout);
    char dummy_ch;
    //while(!read(STDIN_FILENO, &dummy_ch, 1));
}*/

/**
 * @brief  Computes the time difference
 * @param  start_t: initial time in microseconds
 * @retval The time elapsed in milliseconds since start_time
 * @note   clock() doesn't measure wall-clock time, i.e. can't be used with msleep in linux
 */
double time_diff(uint64_t start_t)
{
    //return (((double) (get_clock() - start_t)) / (1 + (CLOCKS_PER_SEC - 1)*WINDOWS_EN)) / 1000;
    return ((double) (get_clock() - start_t)) / 1000;
}
//**************************************************************************************

/**
 * @brief  Get time and date information
 * @param  None
 * @retval The system date and time as a string pointer
 * @note   Source code: https://www.w3resource.com/c-programming/time/c-asctime.php
 */
char *get_timeinfo(time_t timestamp)
{
    static struct tm *new_time;
    time_t lctime = timestamp;

    // Get the timestamp in seconds
    if (timestamp == 0)
    {
        time(&lctime);
    }

    // Convert it to the structure tm
    new_time = localtime(&lctime);

    return asctime(new_time);
}
//**************************************************************************************

#ifdef _WIN32 // @windows


/**
 * @brief  Get time in microseconds
 * @retval The CPU time in microseconds

long int get_clock()
{
    return (long int) ((double)(clock()) / CLOCKS_PER_SEC / 1000000);
}
 */
//**************************************************************************************

/**
 * @brief  Get time in microseconds
 * @retval The wall-clock time in microseconds
 */
long long get_clock()
{
    /*static long long frequency = 0;
    static LARGE_INTEGER te, ts;
    if (frequency == 0)
    {
        LARGE_INTEGER freq;
        // Get start program initial reference
        QueryPerformanceCounter(&ts);
        // Get core frequency
        QueryPerformanceFrequency(&freq);
        frequency = freq.QuadPart;
    }

    QueryPerformanceCounter(&te);
    return (long int) ((double)(te.QuadPart - ts.QuadPart) * 1000000.0 / frequency);*/

    
    //The epoch used by FILETIME starts from January 1, 1601
    FILETIME ft;
    ULARGE_INTEGER ui;

    // Get the current system time as a FILETIME
    GetSystemTimeAsFileTime(&ft);
    
    // Convert FILETIME to ULARGE_INTEGER for easier manipulation
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;

    // Convert to milliseconds (1 tick = 100 nanoseconds, so 10 ticks = 1 microsecond)
    return  (long long) (ui.QuadPart / 10);
    
}
//**************************************************************************************

/**
 * @brief  Move terminal cursor
 * @param  x: row number
 * @param  y: column number
 */
void gotoxy(int x, int y)
{
    COORD coord = {0, 0};
    coord.X = y;
    coord.Y = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//****************************************************************************************

/**
 * @brief  Hide terminal cursor
 * @param  State: disable or enable
 */
void hide_cursor(int state)
{
    CONSOLE_CURSOR_INFO cursor = {1, !state};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
//****************************************************************************************

/**
 * @brief  Read a pressed key from keyboard
 * @retval The key pressed
 */
char get_char()
{
    return getch();
}
//****************************************************************************************
#else // @linux

/**
 * @brief  Read a pressed key from keyboard
 * @retval The key pressed
 */
char get_char()
{
    char ch = 0;
    // read last character in case ANSI escape sequences
    while(read(STDIN_FILENO, &ch, 1) > 0);
    return ch;
}
//**************************************************************************************

/**
 * @brief  Get time in microseconds
 * @retval The wall-clock time in microseconds
 */
long int get_clock()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (long int) ((ts.tv_sec * 1000000LL) + (ts.tv_nsec / 1000));
}
//**************************************************************************************

/**
 * @brief  Move terminal cursor
 * @param  x: row number
 * @param  y: column number
 */
void gotoxy(int x, int y)
{
    printf("\033[%d;%dH",x+1, y+1);
}
//****************************************************************************************

/**
 * @brief  Hide terminal cursor
 * @param  State: disable or enable
 */
void hide_cursor(int state)
{
    if (state)
    {
        printf("\e[?25l");
    }
    else
    {
        printf("\e[?25h");
    }

}
//****************************************************************************************

/**
 * @brief  Verify keyboard input
 * @param  None
 * @retval True if a key was pressed or false otherwise
 */
int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
//****************************************************************************************

/**
 * @brief  Set non-blocking terminal input and disable echo mode
 * @param  state: enable or disable
 * @retval The system date and time as a string pointer
 * @note   Canonical mode: waits until ENTER is pressed to take input
 *         Echo mode     : prints all typed input
 */
void set_nonblock(int state)
{
    struct termios ttystate;

    // get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    if (state)
    {
        // turn off canonical and echo mode
        ttystate.c_lflag &= ~(ICANON | ECHO);
    }
    else
    {
        // turn on canonical and echo mode
        ttystate.c_lflag |= (ICANON | ECHO);
    }

    // minimum of number input read.
    ttystate.c_cc[VMIN] = 0;
    // timeout in deciseconds
    ttystate.c_cc[VTIME] = 0;

    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}
//****************************************************************************************
#endif

/**
 ******************************************************************************
 * @file    util.h
 * @date    21-July-2024
 * @brief   Miscellaneous functions for PSoc Monitor
 *
 ******************************************************************************
 */
#include "include/util.h"

/**
 * @brief  Time difference
 * @param  start_t: initial time
 * @retval The time elapsed in milliseconds since start_time
 * @note
 */
double time_diff(clock_t start_t)
{
    return (((double) (clock() - start_t)) / CLOCKS_PER_SEC) / 1000;
}
//**************************************************************************************

/**
 * @brief  Get time and date information
 * @param  None
 * @retval The system date and time as a string pointer
 * @note   Source code: https://www.w3resource.com/c-programming/time/c-asctime.php
 */
char *get_timeinfo()
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

#ifdef _WIN32 // @windows

/**
 * @brief  Move terminal cursor
 * @param  x: row number
 * @param  y: column number
 */
void gotoxy(int x, int y)
{
    COORD coord = {0, 0};
    coord.X = x;
    coord.Y = y;
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

#else // @linux

///**
// * @brief  Read the keyboard input  
// * @retval True if a key was pressed or false otherwise
// */
//char get_keyboard(void)
//{
//    int input_ch = 0;
//
//    // read last character in case ANSI escape sequences
//    while(read(STDIN_FILENO, &input_ch, 1) > 0);
//    return (char) input_ch;
//}
//****************************************************************************************

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

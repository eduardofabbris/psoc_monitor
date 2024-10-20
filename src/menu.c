/*******************************************************************************
* @filename: menu.c
*
* @brief:
*
*
*
*
********************************************************************************/

/*********************************************************
* Includes
*********************************************************/

#include "include/menu.h"

// _____   _____        _____        __       __  __             _ _
//|  __ \ / ____|      / ____|      / /      |  \/  |           (_) |
//| |__) | (___   ___ | |          / /_      | \  / | ___  _ __  _| |_ ___  _ __
//|  ___/ \___ \ / _ \| |         | '_ \     | |\/| |/ _ \| '_ \| | __/ _ \| '__|
//| |     ____) | (_) | |____     | (_) |    | |  | | (_) | | | | | || (_) | |
//|_|    |_____/ \___/ \_____|     \___/     |_|  |_|\___/|_| |_|_|\__\___/|_|
//
//⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⠀⣸⣏⠛⠻⠿⣿⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⠀⣿⣿⣿⣷⣦⣤⣈⠙⠛⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⣈⠙⠻⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀
//⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡉⠛⠻⢿⣿⣷⣶⣤⣀⠀⠀
//⠀⠀⠀⠉⠙⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⢻⣍⡉⠉⣿⠇⠀
//⠀⠀⠀⠀⠀⠀⠀⢹⡏⢹⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣰⣿⣿⣾⠏⠀⠀
//⠀⠀⠀⠀⠀⠀⠀⠘⣿⠈⣿⠸⣯⠉⠛⠿⢿⣿⣿⣿⣿⡏⠀⠻⠿⣿⠇⠀⠀⠀
//⠀⠀⠀⠀⠀⠀⠀⠀⢿⡆⢻⡄⣿⡀⠀⠀⠀⠈⠙⠛⠿⠿⠿⠿⠛⠋⠀⠀⠀⠀
//⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⠘⣇⢸⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⠀⠀⠀⠀⠀⣀⣀⣿⣴⣿⢾⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⣴⡶⠾⠟⠛⠋⢹⡏⠀⢹⡇⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⢠⣿⠀⠀⠀⠀⢀⣈⣿⣶⠿⠿⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⢸⣿⣴⠶⠞⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
//⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀

/*********************************************************
* Global Variables
*********************************************************/

// Templates
const char *main_menu_template =
{"\
+-----------------------------------------------------------------------------------+\
|                                                                                   |\
|   _____   _____        _____        __       __  __             _ _               |\
|  |  __ \\ / ____|      / ____|      / /      |  \\/  |           (_) |              |\
|  | |__) | (___   ___ | |          / /_      | \\  / | ___  _ __  _| |_ ___  _ __   |\
|  |  ___/ \\___ \\ / _ \\| |         | '_ \\     | |\\/| |/ _ \\| '_ \\| | __/ _ \\| '__|  |\
|  | |     ____) | (_) | |____     | (_) |    | |  | | (_) | | | | | || (_) | |     |\
|  |_|    |_____/ \\___/ \\_____|     \\___/     |_|  |_|\\___/|_| |_|_|\\__\\___/|_|     |\
|                                                                                   |\
|                                                                                   |\
|                              * Start Monitoring                                   |\
|                              * Exit                                               |\
|                                                                                   |\
|                                                                                   |\
|                                                                                   |\
|                                                                                   |\
|  Confirm: [ENTER]                                   Menu navigation: [w] and [s]  |\
+-----------------------------------------------------------------------------------+\
"};

const char *prompt_menu_template =
{"\
+-----------------------------------------------------------------------------------+\
|                                                                                   |\
|   _____   _____        _____        __       __  __             _ _               |\
|  |  __ \\ / ____|      / ____|      / /      |  \\/  |           (_) |              |\
|  | |__) | (___   ___ | |          / /_      | \\  / | ___  _ __  _| |_ ___  _ __   |\
|  |  ___/ \\___ \\ / _ \\| |         | '_ \\     | |\\/| |/ _ \\| '_ \\| | __/ _ \\| '__|  |\
|  | |     ____) | (_) | |____     | (_) |    | |  | | (_) | | | | | || (_) | |     |\
|  |_|    |_____/ \\___/ \\_____|     \\___/     |_|  |_|\\___/|_| |_|_|\\__\\___/|_|     |\
|                                                                                   |\
|  ******************    Enter the required information    ***********************  |\
|  *                                                                             *  |\
|  * PSoC 6 port:                                                                *  |\
|  * Monitoring device port:                                                     *  |\
|  * Additional log info:                                                        *  |\
|  *                                                                             *  |\
|  *******************************************************************************  |\
|  Confirm: [ENTER]                                                  Cancel: [ESC]  |\
+-----------------------------------------------------------------------------------+\
"};

const char *monitor_ascii_art[] = {
//"                                   ",
"⠀⠀⠀⣸⣏⠛⠻⠿⣿⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⣿⣿⣿⣷⣦⣤⣈⠙⠛⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⣈⠙⠻⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀",
"⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡉⠛⠻⢿⣿⣷⣶⣤⣀⠀",
"⠀⠀⠀⠉⠙⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⢻⣍⡉⠉⣿⠇",
"⠀⠀⠀⠀⠀⠀⠀⢹⡏⢹⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣰⣿⣿⣾⠏⠀",
"⠀⠀⠀⠀⠀⠀⠀⠘⣿⠈⣿⠸⣯⠉⠛⠿⢿⣿⣿⣿⣿⡏⠀⠻⠿⣿⠇⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⢿⡆⢻⡄⣿⡀⠀⠀⠀⠈⠙⠛⠿⠿⠿⠿⠛⠋⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⠘⣇⢸⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⣀⣀⣿⣴⣿⢾⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⣴⡶⠾⠟⠛⠋⢹⡏⠀⢹⡇⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⢠⣿⠀⠀⠀⠀⢀⣈⣿⣶⠿⠿⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⢸⣿⣴⠶⠞⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"\0",
};

const char *monitor_menu_template =
{"\
+-----------------------------------------------------------------------------------+\
|                                                                                   |\
|                                              P S o C  6   M o n i t o r           |\
|                                                                                   |\
|                                     Device Status                                 |\
|                                                                                   |\
|                                         - PSoc 6: Connected                       |\
|                                         - Watchdog: Connected                     |\
|                                                                                   |\
|                                     Info                                          |\
|                                                                                   |\
|                                         - Buffers:                                |\
|                                                                                   |\
|                                                                                   |\
|                                                                                   |\
|                                                                 Confirm: [ENTER]  |\
|  In progress                                                  Back/Exit: [ESC]    |\
+-----------------------------------------------------------------------------------+\
"};

const char *debug_menu_template =
{"\
+----------------------------------+------------------------------------------------+\
|                                  |                                                |\
|         Error Injection          |           P S o C  6   M o n i t o r (Debug)   |\
|  (Note: needs to be enabled on   |                                                |\
|         device)                  |  Device Status                                 |\
|  - Et: Time                      |                                                |\
|  - Eu: Single Upset              |      - PSoc 6: Connected                       |\
|  - El: Latch-up                  |      - Watchdog: Connected                     |\
|  - Eh: Core hang                 |                                                |\
|  - Em: Memory                    |  Info                                          |\
|  - Es: Stop UART                 |                                                |\
|                                  |      - Buffers:                                |\
|  - Elapsed min:                  |      - DUT response countdown:                 |\
|  - Received pckts:               |      - WD response countdown:                  |\
|                                  |      - Serial reset counter:                   |\
|                                  |      - Hang reset counter:                     |\
|  In progress                     |                               Back/Exit: [ESC] |\
+----------------------------------+------------------------------------------------+\
"};

/*********************************************************
* Function Definitions
*********************************************************/

/**
 * @brief  Clear screen and print a new one
 * @param  background: template to print as background
 * @param  input_layer: layer to print over the background
 * @retval None
 */
void update_screen(const char *background, char *input_layer, char *ascii_art[])
{
    static char clear_layer[TERM_N_COL*TERM_N_ROW] = {'\0'};
    static char *last_background = NULL;
    char bck_ch, input_ch, clr_ch, output_ch;

    //clrscr();
    for(int i = 0; i < TERM_N_ROW; i++)
    {
        for(int j = 0; j < TERM_N_COL; j++)
        {
            output_ch = '\0';
            clr_ch   = clear_layer[TERM_N_COL*i + j];
            input_ch = input_layer[TERM_N_COL*i + j];
            bck_ch   = background[TERM_N_COL*i + j];

            // Print the top layer of the scene
            if (input_ch != '\0')
            {
                output_ch = input_ch;
                clear_layer[TERM_N_COL*i + j] = '!';
            }
            // Clear last character printed in this position by printing the background
            else if (clr_ch != '\0')
            {
                output_ch = bck_ch;
                clear_layer[TERM_N_COL*i + j] = '\0';        
            }          
            // Force new background print if the scene has changed      
            else if(last_background != background)
            {
                output_ch = bck_ch;
            }
            // Output character
            if (output_ch != '\0')
            {
                gotoxy(i, j), printf("%c", output_ch);  
            }            
        }
    }

    last_background = (char *) background;
    //memset(clear_layer, '\0', TERM_N_ROW*TERM_N_COL);
    // Print ASCII art
    /*if (ascii_art != NULL)
    {
        gotoxy(1, 3);
        for (int i = 0; **(ascii_art + i) ; i++)
        {
            gotoxy(1 + i, 3);
            printf("%s", ascii_art[i]);
        }
    }*/

    // force terminal output update
    fflush(stdout);
    // Reset input layer
    memset(input_layer, '\0', TERM_N_ROW*TERM_N_COL);
}
//****************************************************************************************

/**
 * @brief  Handle arrow movement and get main menu option
 * @param  input_layer: screen layer to print over the background
 * @retval The option selected or -1 otherwise
 */
int get_menu_option(char *input_layer)
{
    static int menu_option = 0;
    // Set arrow position
    char *arrow_ptr = input_layer + TERM_N_COL*(ARROW_ROW_OFFSET + menu_option) + ARROW_COL_OFFSET;

    int ch = 0;
    if(kbhit()){
        // Read a key form keyboard
        ch = get_char();

        switch(ch)
        {
            case 'w': case 'k': case UP:
                if (menu_option > 0 ) menu_option -= 1;
                else menu_option = MENU_N_OPTIONS - 1;

                break;
            case 's': case 'j': case DOWN:
                if (menu_option < MENU_N_OPTIONS - 1) menu_option += 1;
                else menu_option = 0;

                break;
            case 'q': case ESC:
                // Considering the last option is exit
                return MENU_N_OPTIONS - 1;
            case ENTER:
                return menu_option;
            default:
                break;
        }
        // Update arrow position
        arrow_ptr = input_layer + TERM_N_COL*(ARROW_ROW_OFFSET + menu_option) + ARROW_COL_OFFSET;
    }

    memcpy(arrow_ptr, "->", 2);
    return -1;
}
//****************************************************************************************

/**
 * @brief  Get user string input from keyboard
 * @param  input_layer: screen layer to print over the background
 * @param  str_buffer: buffer to store the input string
 * @param  max_str_len: maximum input string length
 * @retval string length when input confirmed, -1 when input canceled and -2 otherwise
 */
int get_keyboard_str(char *input_layer, char *str_buffer, int max_str_len)
{
    static int str_len = 0, blink_latch = 0;
    static uint64_t blink_timer = 0;

    int ch = 0;
    if(kbhit()){
        // Read a key form keyboard
        ch = get_char();

        switch(ch)
        {
            // Cancel input
            case ESC:
                str_len = 0;
                return -1;

            // Finished typing
            case ENTER:
                int aux = str_len;
                str_len = 0;
                return aux;

            // Erase character
            case BACKSPACE:
                if (str_len > 0)
                    str_len -= 1;

                break;
            // Add new character
            default:
                if (str_len < max_str_len)
                {
                    if (str_buffer != NULL)
                    {
                        str_buffer[str_len] = ch;
                    }
                    str_len += 1;
                }
                break;
        }
    }

    if (input_layer != NULL && str_buffer != NULL)
    {
        memcpy(input_layer, str_buffer, str_len);
        if ( time_diff(blink_timer) >= 500 )
        {
            blink_timer = get_clock();
            blink_latch = !blink_latch;
        }
        if (blink_latch)
        {
            memcpy(input_layer + str_len, "_", 1);
        }
    }
    return -2;
}
//****************************************************************************************


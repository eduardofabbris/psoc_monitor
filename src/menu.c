#include "include/menu.h"

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
//⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⠇⠰⣤⣄⣀⠀⠉⠛⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⠀⠀⠀⠈⠙⠛⠷⣦⣤⣀⠀⠈⠉⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⠷⣦⣄⣀⠀⠈⠉⠛⠿⢿⣿⣿⣿⣿⣿⣿
//⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⢶⣤⣄⡀⠀⠈⠉⠛⠿⣿⣿
//⣿⣿⣿⣶⣦⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⡄⠲⢶⣶⠀⣸⣿
//⣿⣿⣿⣿⣿⣿⣿⡆⢰⡆⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠏⠀⠀⠁⣰⣿⣿
//⣿⣿⣿⣿⣿⣿⣿⣧⠀⣷⠀⣇⠐⣶⣤⣀⡀⠀⠀⠀⠀⢰⣿⣄⣀⠀⣸⣿⣿⣿
//⣿⣿⣿⣿⣿⣿⣿⣿⡀⢹⡄⢻⠀⢿⣿⣿⣿⣷⣦⣤⣀⣀⣀⣀⣤⣴⣿⣿⣿⣿
//⣿⣿⣿⣿⣿⣿⣿⣿⡇⠘⣧⠸⡇⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⣿⣿⣿⣿⠿⠿⠀⠋⠀⡁⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⠋⢉⣁⣠⣤⣴⡆⢰⣿⡆⢸⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⡟⠀⣿⣿⣿⣿⡿⠷⠀⠉⣀⣀⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⡇⠀⠋⣉⣡⣤⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿

// _____   _____        _____        __       __  __             _ _
//|  __ \ / ____|      / ____|      / /      |  \/  |           (_) |
//| |__) | (___   ___ | |          / /_      | \  / | ___  _ __  _| |_ ___  _ __
//|  ___/ \___ \ / _ \| |         | '_ \     | |\/| |/ _ \| '_ \| | __/ _ \| '__|
//| |     ____) | (_) | |____     | (_) |    | |  | | (_) | | | | | || (_) | |
//|_|    |_____/ \___/ \_____|     \___/     |_|  |_|\___/|_| |_|_|\__\___/|_|
//

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

const char *monitor_menu_template =
{"\
+-----------------------------------------------------------------------------------+\
|   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⠀⠀⣸⣏⠛⠻⠿⣿⣶⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀             P S o c  6   M o n i t o r           |\
|   ⠀⠀⠀⣿⣿⣿⣷⣦⣤⣈⠙⠛⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⣈⠙⠻⠿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀    Device Status                                 |\
|   ⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡉⠛⠻⢿⣿⣷⣶⣤⣀⠀⠀                                                  |\
|   ⠀⠀⠀⠉⠙⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⢻⣍⡉⠉⣿⠇⠀        - PSoc 6:                                 |\
|   ⠀⠀⠀⠀⠀⠀⠀⢹⡏⢹⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣰⣿⣿⣾⠏⠀⠀        - Watchdog:                               |\
|   ⠀⠀⠀⠀⠀⠀⠀⠘⣿⠈⣿⠸⣯⠉⠛⠿⢿⣿⣿⣿⣿⡏⠀⠻⠿⣿⠇⠀⠀⠀                                                  |\
|   ⠀⠀⠀⠀⠀⠀⠀⠀⢿⡆⢻⡄⣿⡀⠀⠀⠀⠈⠙⠛⠿⠿⠿⠿⠛⠋⠀⠀⠀⠀    Info                                          |\
|   ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⠘⣇⢸⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⠀⠀⠀⠀⠀⠀⣀⣀⣿⣴⣿⢾⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀        - Errors:                                 |\
|   ⠀⠀⣴⡶⠾⠟⠛⠋⢹⡏⠀⢹⡇⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⢠⣿⠀⠀⠀⠀⢀⣈⣿⣶⠿⠿⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⢸⣿⣴⠶⠞⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                                  |\
|   ⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                                   Exit: [ESC]    |\
|  In progress \%                                                 Confirm: [ENTER]  |\
+-----------------------------------------------------------------------------------+\
"};

//const char *monitor_menu_template[] =
//    {
//        "+-----------------------------------------------------------------------------+",
//        "|                          M o n i t o r   P S o c 6                          |",
//        "|                                                                             |",
//        "|  INFORMACOES                                                                |",
//        "|     - Alive serial recebido:                                                |",
//        "|     - Buffers recebidos:                                                    |",
//        "|     - Resets sinal alive:                                                   |",
//        "|     - Resets alive serial:                                                  |",
//                //TODO: remove debug signals
//        "|                                                                             |",
//        "|  STATUS                                                                     |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|  Confirma: [ENTER]                                           Voltar: [ESC]  |",
//        "+-----------------------------------------------------------------------------+"
//
//};

//const char *serial_menu_template[] =
//    {
//        "+-----------------------------------------------------------------------------+",
//        "|                          M o n i t o r   P S o c 6                          |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                  Selecione para escolher uma porta serial                   |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                   ( ) PSoC principal:                                       |",
//        "|                       Status:                                               |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                   ( ) PSoc controle:                                        |",
//        "|                       Status:                                               |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                                                                             |",
//        "|                       Apagar: [BACKSPACE]                                   |",
//        "|  Confirma: [ENTER]    Voltar/Cancelar: [ESC]       Navegar Menu: [W] e [S]  |",
//        "+-----------------------------------------------------------------------------+"
//
//};
//****************************************************************************************

/**
 * @brief  Clear screen and print a new one
 * @param  background: screen template to print as background
 * @param  input_layer: screen layer to print over the background
 * @retval None
 */
void update_screen(const char *background, char *input_layer)
{
    char bck_ch, input_ch;

    clrscr();
    for(int i = 0; i < TERM_N_ROW; i++)
    {
        for(int j = 0; j < TERM_N_COL; j++)
        {
            input_ch = input_layer[TERM_N_COL*i + j];
            if (input_ch != '\0')
                gotoxy(i, j), printf("%c", input_ch);
            else
            {
                bck_ch = background[TERM_N_COL*i + j];
                if (bck_ch != ' ' && bck_ch != '\0')
                    gotoxy(i, j), printf("%c", bck_ch);
            }
        }
    }
    // force terminal output update
    fflush(stdout);
    // clear input layer
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
        // read last character in case ANSI escape sequences
        // TODO: adapt for windows -> not sure if its going to work -> TEST IT
        while(read(STDIN_FILENO, &ch, 1) > 0);

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

///**
// * @brief  Handle arrow movement and get main menu option
// * @param  input_layer: screen layer to print over the background
// * @retval The option selected or -1 otherwise
// */
//int manage_main_menu(char *input_layer)
//
//{
//}
////****************************************************************************************

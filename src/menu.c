#include "include/menu.h"


//char input_layer[TERM_N_ROW*TERM_N_COL] = {};

const char *main_menu_template =
{"\
+-----------------------------------------------------------------------------+\
|                          P S o c  6   M o n i t o r                         |\
|                                                                             |\
|                                                                             |\
|  Device Status                                                              |\
|     - PSoc 6:                                                               |\
|     - Watchdog:                                                             |\
|                                                                             |\
|                                                                             |\
|                                                                             |\
|                          * Connect devices                                  |\
|                          * Begin                                            |\
|                          * Exit                                             |\
|                                                                             |\
|                                                                             |\
|                                                                             |\
|                                                                             |\
|                                                                             |\
|                                                                             |\
|  Confirm: [ENTER]                              Menu navigation: [W] and [S] |\
+-----------------------------------------------------------------------------+\
"};

const char *monitor_menu_template[] =
    {
        "+-----------------------------------------------------------------------------+",
        "|                          M o n i t o r   P S o c 6                          |",
        "|                                                                             |",
        "|  INFORMACOES                                                                |",
        "|     - Alive serial recebido:                                                |",
        "|     - Buffers recebidos:                                                    |",
        "|     - Resets sinal alive:                                                   |",
        "|     - Resets alive serial:                                                  |",
                //TODO: remove debug signals
        "|                                                                             |",
        "|  STATUS                                                                     |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|  Confirma: [ENTER]                                           Voltar: [ESC]  |",
        "+-----------------------------------------------------------------------------+"

};

const char *serial_menu_template[] =
    {
        "+-----------------------------------------------------------------------------+",
        "|                          M o n i t o r   P S o c 6                          |",
        "|                                                                             |",
        "|                                                                             |",
        "|                  Selecione para escolher uma porta serial                   |",
        "|                                                                             |",
        "|                                                                             |",
        "|                   ( ) PSoC principal:                                       |",
        "|                       Status:                                               |",
        "|                                                                             |",
        "|                                                                             |",
        "|                   ( ) PSoc controle:                                        |",
        "|                       Status:                                               |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                                                                             |",
        "|                       Apagar: [BACKSPACE]                                   |",
        "|  Confirma: [ENTER]    Voltar/Cancelar: [ESC]       Navegar Menu: [W] e [S]  |",
        "+-----------------------------------------------------------------------------+"

};
//****************************************************************************************

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
    fflush(stdout);
    memset(input_layer, '\0', TERM_N_ROW*TERM_N_COL);
}

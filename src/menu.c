#include "include/menu.h"


char input_layer[] = {};

const char *main_menu_template[] =
	{
		"+-----------------------------------------------------------------------------+",
		"|                          M o n i t o r   P S o c 6                          |",
		"|                                                                             |",
		"|                                                                             |",
		"|                                                                             |",
		"|  STATUS PORTA SERIAL                                                        |",
		"|     - PSoC principal:                                                       |",
		"|     - PSoC controle:                                                        |",
		"|                                                                             |",
		"|                                                                             |",
		"|                                                                             |",
		"|                          * Conectar serial                                  |",
		"|                          * Iniciar                                          |",
		"|                          * Plotar buffer (GNUPlot)                          |",
		"|                          * Sair                                             |",
		"|                                                                             |",
		"|                                                                             |",
		"|                                                                             |",
		"|                                                                             |",
		"|  Confirma: [ENTER]                                 Navegar Menu: [W] e [S]  |",
		"+-----------------------------------------------------------------------------+"

};

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


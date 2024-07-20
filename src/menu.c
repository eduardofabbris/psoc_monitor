#include "include/menu.h"

const char *mainMenuCanvas[] =
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

const char *monitorMenuCanvas[] =
	{
		"+-----------------------------------------------------------------------------+",
		"|                          M o n i t o r   P S o c 6                          |",
		"|                                                                             |",
		"|  INFORMACOES                                                                |",
		"|     - Alive serial recebido:                                                |",
		"|     - Buffers recebidos:                                                    |",
		"|     - Resets sinal alive:                                                   |",
		"|     - Resets alive serial:                                                  |",
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

const char *changeSerialPortMenuCanvas[] =
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

void printSymbolMenu(BOOL clean, int x, int y, enum symbolType symbol)
{

	switch (symbol)
	{
	case symbArrow:
	{
		char arrow[] = "->";
		int i;
		gotoxy(x, y);
		if (clean)
		{
			for (i = 0; i < ARROW_MENU_COLUMNS; i++)
			{
				_printf(" ");
			}
		}
		else
		{ // clean = false
			for (i = 0; i < ARROW_MENU_COLUMNS; i++)
			{
				_printf("%c", arrow[i]);
			}
		}
	}
	break;

	case symbX:
	{
		char symbolX = 'X';
		gotoxy(x, y);
		if (clean)
		{
			_printf(" ");
		}
		else
		{ // clean = false
			_printf("%c", symbolX);
		}
	}
	break;
	}
}
//****************************************************************************************

int 
symbolMenuMovement( int initialX, 	 int initialY,
					int upperLimitX, int bottomLimitX, 
					int leap, 
					enum symbolType symbol,
					BOOL blocking)
{
	char key = 0;
	static int last_initX = 0, 
			   last_initY = 0,
			   i = 0, j = 0;

	if(last_initX != initialX || last_initY != initialY)
	{
		// New menu init settings
		i = initialX, j = initialY;
		last_initX = i, last_initY = j;

		// Print symbol in initial position
		if (symbol == symbArrow)
			printSymbolMenu(FALSE, i, j, symbArrow);
		else if (symbol == symbX)
			printSymbolMenu(FALSE, i, j, symbX);

	}

	// menu movement
	Sleep(15); // limit CPU
	clrbuf();
	do
	{
		if (kbhit())
		{
			key = getch();
			
			// clean symbol
			if (symbol == symbArrow)
				printSymbolMenu(TRUE, i, j, symbArrow);
			else if (symbol == symbX)
				printSymbolMenu(TRUE, i, j, symbX);

			// update the coords
			switch (key)
			{
			case 'w': case 'W': case UP:
			{
				if (i > upperLimitX) // height limit
				{
					i -= leap; // up 1
				}
				else // i = upperLimitX
				{
					i = bottomLimitX;
				}
			}
			break;
			case 's': case 'S': case DOWN:
			{
				if (i < bottomLimitX) // bottom limit
				{
					i += leap; // down 1
				}
				else // i = bottomLimitX
				{
					i = upperLimitX;
				}
			}
			break;
			case ESC:
				if (!kbhit())
					return ESC;
			}
			// reprint symbol
			if (symbol == symbArrow)
				printSymbolMenu(FALSE, i, j, symbArrow);
			else if (symbol == symbX)
				printSymbolMenu(FALSE, i, j, symbX);
		}

	} while (key != ENTER && blocking);

	if(key == ENTER)
	{
		// Clean symbol in final position after the selection
		//if (symbol == symbArrow)
		//	printSymbolMenu(TRUE, i, j, symbArrow); // arrow

		last_initX = last_initY = -1;
		return ((i - upperLimitX) / leap); // return the option starting at 0
	}

	return -1;

}
//****************************************************************************************

void printBackground(const char *background[], int rows, int startRow, int startCol)
{

	clrscr();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < CANVAS_COLUMNS; j++)
		{
			if (background[i][j] != ' ')
			{
				gotoxy(i + startRow, startCol + j);
#ifdef _WIN32
				printf("%c", background[i][j]);
#else
				printw("%c", background[i][j]);
#endif
			}
		}
	}

	refresh();
}
//****************************************************************************************

void mainMenu(BUFFER_DATA *bufferPSoc, SERIAL_PORT *mainDevice, SERIAL_PORT *monitorDevice)
{
	//RUN_INFO runInfo;
	MONITORING_INFO monitoring_info;
	FILE_INFO file_info;
	const char *file_name = "psoc_buffer";
	clock_t firstTick;
	BOOL exitFlag = FALSE,
		 warningFlag = FALSE;
	uint8_t warningType = 0,
			connectionAttempts = 0;
	int8_t last_uart_status = -1;
	double dT = 0;
	int selectedOption = 0;

	MONITORING_INFO_Init(&monitoring_info);
	FILE_INFO_Init(&file_info, file_name);


	while (!exitFlag)
	{
		if(selectedOption != -1)
		{
			clrscr(), printBackground(mainMenuCanvas, CANVAS_ROWS, 0, 0);
		}
		

		// verify connection
		UART_verifyConnection(mainDevice), UART_verifyConnection(monitorDevice);
		if (last_uart_status != mainDevice->active + monitorDevice->active || selectedOption != -1)
		{
			printSerialStatus(mainDevice->active, MAIN_DEVICE_STATUS_X_POS, MAIN_DEVICE_STATUS_Y_POS);
			printSerialStatus(monitorDevice->active, MONITOR_DEVICE_STATUS_X_POS, MONITOR_DEVICE_STATUS_Y_POS);
			last_uart_status = mainDevice->active + monitorDevice->active;

		}
		
		

		if (warningFlag)
		{
			clear_write(53, 17, 3);
			switch (warningType)
			{
			case lostConnection:
				printPrompt("Aviso: A conexao com o(s) dispostivo(s) foi perdida!", 17, 3);
				break;
			case noDataInBuffer:
				printPrompt("Aviso: Nao ha dados no buffer!", 17, 3);
				break;
			case noGNUPlot:
				printPrompt("GNUPlot nao encontrado!", 17, 3);
				break;
			case connectBothDevices:
				printPrompt("Conecte os dois dispositivos antes de iniciar!", 17, 3);
				break;

			default:
			}

			warningFlag = 0;
			warningFlag = FALSE;
		}

		
		selectedOption = symbolMenuMovement(11, 24, 11, 14, 1, symbArrow, FALSE); 
		if (selectedOption != ESC && selectedOption != -1)
			selectedOption++;

		switch (selectedOption)
		{
		case PLOT_GRAPH:
			if (monitoring_info.buffer_n > 0)
			{
				if (!gnuPlotGraph())
				{
					warningType = noGNUPlot;
					warningFlag = 1;
				}
			}
			else
			{
				warningType = noDataInBuffer;
				warningFlag = 1;
			}

			break;
		//---------------------------------------------------------------------------
		case BEGIN:
			uint8_t menuRow = 11;
			
			if (!(mainDevice->active && monitorDevice->active))
			{
				warningFlag = TRUE;
				warningType = connectBothDevices;
				menuRow++;
			}
			else
			{
				printBackground(monitorMenuCanvas, CANVAS_ROWS, 0, 0);
				// tenta se comunicar com dispositivos
				printPrompt("Estabelecendo conexao com dispositivos...", menuRow, MENU_START_COLUMN);
				menuRow++;

				if (initPSocDevices(mainDevice, monitorDevice))
				{

					menuRow += 3;
					printPrompt("Aguardando buffer...", menuRow, MENU_START_COLUMN);

					MONITORING_INFO_Init(&monitoring_info);
					FILE_INFO_Init(&file_info, file_name);

					manageTxtFile(&file_info);

					bufferPSoc->dataIndex =
						bufferPSoc->SRfaultIndex =
							bufferPSoc->WRfaultIndex = 0;

					updateRunInfo(monitoring_info, TRUE);
					printPrompt("Pressione ENTER para SAIR   ", 17, 3);

					firstTick = clock();
					BOOL runExitFlag = FALSE;
					while (!exitRunPrompt() && !runExitFlag)
					{
#ifndef _WIN32
						usleep(5); // limit CPU linux
#endif
						// verify serial connection
						//connectSerialPort(mainDevice, FALSE), connectSerialPort(monitorDevice, FALSE);
						UART_verifyConnection(mainDevice), UART_verifyConnection(monitorDevice);
						if (!mainDevice->active || !monitorDevice->active)
						{
							if(connectionAttempts > 10){
								connectionAttempts = 0;
								runExitFlag = TRUE;
								warningFlag = TRUE;
								warningType = lostConnection;
								//gotoxy(0,0), _printf("error: Connection attempt %d", connectionAttempts);
								Sleep(1);
							}
							else{
								connectionAttempts++;
							}

						}
						monitorMainDevice(bufferPSoc, *mainDevice, &monitoring_info, &file_info);

						if (!monitoring_info.r_buffer_flag)
						{
							if (monitorControlDevice(*monitorDevice, &monitoring_info, &file_info))
							{
								Sleep(200);
								connectSerialPort(mainDevice, TRUE); // flush data
								monitoring_info.alv_serial_flag = FALSE;
								firstTick = clock();
							}
						}
						else
						{
							monitoring_info.alv_serial_flag = FALSE;
							firstTick = clock();
						}

						// serial alive signal monitoring
						dT = timeDiff(firstTick) / 1E3; // sec
						if (dT >= ALIVE_SIGNAL_TIMEOUT)
						{

							if (!monitoring_info.alv_serial_flag)
							{
								// send command to restart main device
								write_port(monitorDevice->ptr, (uint8_t *) "HR", 2);
								monitoring_info.alv_serial_rst_n++;
								file_info.alv_serial_rst_n++;
							}
							monitoring_info.alv_serial_flag = FALSE;
							firstTick = clock();
						}
						updateRunInfo(monitoring_info, FALSE);
					}

					updateFileHeader(&file_info);
					break;
				}
				printPrompt("Pressione qualquer tecla para sair... ", 17, MENU_START_COLUMN);
				clrbuf();
				while (!kbhit()) Sleep(33);
				getch();

				
			}


			break;
		//----------------------------------------------------------
		case CONNECT_SERIAL:
			connectSerialPortMenu(mainDevice, monitorDevice);
			break;

		case EXIT:
		case ESC:
			exitFlag = TRUE;
			break;
		default:
			break;
		}
	}
}
//****************************************************************************************

BOOL exitRunPrompt()
{
	static uint8_t state = 0;
	char key = 0;

	if (kbhit())
	{
		key = getch();

		clear_write(29, 17, MENU_START_COLUMN);
		printPrompt("Pressione ENTER para SAIR", 17, MENU_START_COLUMN);
		
		switch (state)
		{
		case 0:
			if (key == ENTER)
			{
				printPrompt("Tem certeza que deseja SAIR?", 17, MENU_START_COLUMN);
				state = 1;
			}
			break;

		case 1:
			if (key == ENTER)
			{
				state = 0;
				return 1;
			}
			else if (key == ESC)
			{
				state = 0;
			}

		default:
			break;
		}
	}

	return 0;
}
//****************************************************************************************

// @param device_type: choose 0 for monitor device or 1 for main device
void updatePortStatus(SERIAL_PORT device, uint8_t device_type)
{
	uint8_t status_x = device_type ? 8 : 12,
			status_y = 32;

	uint8_t port_name_x = device_type ? 7 : 11,
			port_name_y = device_type ? 40 : 39;

	clear_write(28, status_x, status_y);
	clear_write(10, port_name_x, port_name_y);
	if (!device.active)
	{
		if (!*device.port_name)
			printPrompt("Porta serial nao informada.", status_x, status_y);
		else
			printPrompt("Desconectada.", status_x, status_y);
	}
	else
	{
		printPrompt("Conectada.", status_x, status_y);
		printPrompt(device.port_name, port_name_x, port_name_y);
	}
}
//****************************************************************************************

void connectSerialPortMenu(SERIAL_PORT *mainDevice, SERIAL_PORT *monitorDevice)
{
	BOOL exitFlag = FALSE,
		 warningFlag = FALSE;
	int yPos = 7,
		selectedOption = -1;

	uint8_t actual_status = mainDevice->active + monitorDevice->active;

	printBackground(changeSerialPortMenuCanvas, CANVAS_ROWS, 0, 0);
	updatePortStatus(*monitorDevice, 0), updatePortStatus(*mainDevice, 1);

	while (!exitFlag)
	{
		//connectSerialPort(mainDevice, FALSE), connectSerialPort(monitorDevice, FALSE);
		UART_verifyConnection(mainDevice), UART_verifyConnection(monitorDevice);
		uint8_t aux = mainDevice->active + monitorDevice->active; 
		if(aux != actual_status )
		{
			actual_status = aux;
			updatePortStatus(*monitorDevice, 0), updatePortStatus(*mainDevice, 1);
		}
			
		selectedOption = symbolMenuMovement(yPos, 21, 7, 11, 4, symbX, FALSE);
		if (warningFlag && selectedOption != -1)
		{
			clear_write(50, 15, 3);
			warningFlag = FALSE;
		}
		switch (selectedOption)
		{

		case 0:
			yPos = 7;

			clear_write(10, 7, 40);
			if (chooseSerialPort(mainDevice, 7, 40) == 0)
			{
				printPrompt("Aviso: Nao foi possivel conectar a porta serial!", 15, 3);
				warningFlag = TRUE;
			}
			break;

		case 1:
			yPos = 11;

			clear_write(10, 11, 39);
			if (chooseSerialPort(monitorDevice, 11, 39) == 0)
			{
				printPrompt("Aviso: Nao foi possivel conectar a porta serial!", 15, 3);
				warningFlag = TRUE;
			}
			break;
		case ESC:
			exitFlag = TRUE;
			break;
		default:
			break;
		}

		if(selectedOption != -1) 
			updatePortStatus(*monitorDevice, 0), updatePortStatus(*mainDevice, 1);
	}
}
//****************************************************************************************

void updateRunInfo(MONITORING_INFO monitoring_info, BOOL printAll)
{
	char buffer[MAX_BUFFER_SIZE] = {0};
	/*static uint32_t lastBufferIndex = 0,
					lastALSerialRestartIndex = 0,
					lastALRestartIndex = 0;
	static BOOL lastAliveSignalReceived = FALSE;*/
	static MONITORING_INFO last = {0};
	

	if ((last.alv_serial_flag != monitoring_info.alv_serial_flag) || printAll)
	{
		if (monitoring_info.alv_serial_flag)
			printPrompt("Sim.", 4, 31);
		else
			printPrompt("Nao.", 4, 31);
		last.alv_serial_flag = monitoring_info.alv_serial_flag;
	}

	if ((last.buffer_n != monitoring_info.buffer_n) || printAll)
	{
		sprintf(buffer, "%u", monitoring_info.buffer_n);
		printPrompt(buffer, 5, 27);
		// memset(buffer, 0, sizeof(buffer));
		last.buffer_n = monitoring_info.buffer_n;
	}

	if ((last.alv_sig_rst_n != monitoring_info.alv_sig_rst_n) || printAll)
	{
		sprintf(buffer, "%u", monitoring_info.alv_sig_rst_n);
		printPrompt(buffer, 6, 28);
		// memset(buffer, 0, sizeof(buffer));
		last.alv_sig_rst_n = monitoring_info.alv_sig_rst_n;
	}

	if ((last.alv_serial_rst_n != monitoring_info.alv_serial_rst_n) || printAll)
	{
		sprintf(buffer, "%u", monitoring_info.alv_serial_rst_n);
		printPrompt(buffer, 7, 29);

		// memset(buffer, 0, sizeof(buffer));
		last.alv_serial_rst_n = monitoring_info.alv_serial_rst_n;
	}
}
//****************************************************************************************

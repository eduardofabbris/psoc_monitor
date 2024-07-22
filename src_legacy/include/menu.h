#pragma once


#include "util.h"
#include "fileManagement.h"
#include "serial.h"
#include "graph.h"


#define MENU_START_COLUMN 3
#define MAIN_MENU_NUMBER_OF_OPTIONS 4
#define CANVAS_COLUMNS 80
#define CANVAS_ROWS 21
#define ARROW_MENU_COLUMNS 2




enum menuWarnings{lostConnection = 1, noDataInBuffer, noGNUPlot, connectBothDevices};
enum menuOptions{CONNECT_SERIAL=1, BEGIN, PLOT_GRAPH, EXIT};

//symbol
    enum symbolType{ symbArrow, 
                    symbX };


void printBackground(const char *background[], int rows, int startRow, int startCol);
int  symbolMenuMovement(int initialX, int initialY, int upperLimitX, int bottomLimitX, int leap, enum symbolType symbol, BOOL blocking);
void printSymbolMenu(BOOL clean, int x, int y, enum symbolType symbol);
void mainMenu(BUFFER_DATA* bufferPSoc, SERIAL_PORT* mainDevice, SERIAL_PORT* monitorDevice);
BOOL exitRunPrompt();
void connectSerialPortMenu(SERIAL_PORT* mainDevice, SERIAL_PORT* monitorDevice);
void updatePortStatus(SERIAL_PORT device, uint8_t device_type);

void updateRunInfo(MONITORING_INFO monitoring_info, BOOL printAll);
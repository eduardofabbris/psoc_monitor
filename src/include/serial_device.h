#ifndef __SERIAL_DEVICE_H__
#define __SERIAL_DEVICE_H__

#include "util.h"
#include "serial.h"


#define SERIAL_PORT_NAME_BUFFER_LEN 10
#define MAX_LOADSTRING 100
#define ALIVE_SIGNAL_TIMEOUT 60 //sec

#define BAUD_RATE 115200

#define MAIN_DEVICE_STATUS_X_POS 6
#define MAIN_DEVICE_STATUS_Y_POS 24

#define MONITOR_DEVICE_STATUS_X_POS 7
#define MONITOR_DEVICE_STATUS_Y_POS 24


typedef enum {main_device_type, monitor_device_type} device_t;

typedef struct serial_port_str{
    HANDLE ptr;
    BOOL  active;
    BOOL verifying_connection_flag;
    clock_t verify_connection_tmr;
    device_t device_type;
    char port_name[SERIAL_PORT_NAME_BUFFER_LEN];
    //int portNumber;
} SERIAL_PORT;

BOOL connectSerialPort(SERIAL_PORT* device, BOOL flushData);
BOOL readProtocol(SERIAL_PORT* device, char* protocol, double timeout);
void printSerialStatus(BOOL active, int x, int y);
BOOL initPSocDevices(SERIAL_PORT* mainDevice, SERIAL_PORT* monitorDevice);
int chooseSerialPort(SERIAL_PORT* device, int x, int y);
BOOL UART_verifyConnection(SERIAL_PORT *device);
void SERIAL_PORT_Init(SERIAL_PORT *self, device_t device_type);
void *UART_verifyConnection_thread(void *vargp);

#endif
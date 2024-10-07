#ifndef SERIAL_H
#define SERIAL_H

#include "util.h"

//typedef struct serial_port_str{
//    HANDLE ptr;
//    BOOL  active;
//    BOOL verifying_connection_flag;
//    clock_t verify_connection_tmr;
//    device_t device_type;
//    char port_name[SERIAL_PORT_NAME_BUFFER_LEN];
//    //int portNumber;
//} SERIAL_PORT;

typedef struct serial_port_struct{
    char name[100];      // Port name
    int name_len;        // Port name length
    clock_t timeout_cnt; // Timeout connection counter
    BOOL status;         // Status, 1 active and 0 otherwise
    HANDLE device;       // Device descriptor
} serial_port_t;


//------------> DEFINES
#define DEBUG_SERIAL_EN 1
#define PSOC6_CONNECTION_TIMEOUT 8   // Serial connection timeout in seconds
#define MONITOR_CONNECTION_TIMEOUT 8 // Serial connection timeout in seconds




//static void print_error(const char * context);

#ifdef _WIN32 // @windows

//#include <stdint.h>

//------------> Windows DEFINES

// Debug error messages
#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 22
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 5

// Timeout (ms)
//#define READ_TIME_OUT_CONSTANT 100
//#define WRITE_TIME_OUT_CONSTANT 100


//------------> Windows PROTOTYPES
HANDLE open_serial_port(const char * device, uint32_t baud_rate);
int write_port(HANDLE port, uint8_t * buffer, size_t size);
SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size);

#else // @linux

//------------> Linux INCLUDES
#include <fcntl.h>

//------------> Linux DEFINES

// Debug error messages
#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 13
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 11
#define INVALID_HANDLE_VALUE -1

//------------> Linux PROTOTYPES
int _open_serial_port(const char * device, uint32_t baud_rate);
int open_serial_port(char * port_name, int name_len, uint32_t baud_rate);
int write_port(int fd, uint8_t * buffer, size_t size);
ssize_t read_port(int fd, uint8_t * buffer, size_t size);


#endif

#endif


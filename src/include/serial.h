/*******************************************************************************
* @filename: serial.h
* @brief: serial_windows and serial_linux.c header
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/
#ifndef SERIAL_H
#define SERIAL_H

/**********************************************
* Includes
**********************************************/

#include "util.h"

/**********************************************
* Typedefs
**********************************************/

typedef struct serial_port_struct{
    char name[100];                 // Port name
    int name_len;                   // Port name length
    uint64_t timeout_cnt;           // Timeout connection counter
    BOOL status;                    // Status, 1 active and 0 otherwise
    BOOL last_status;               // Last status for verifying status change
    HANDLE device;                  // Device descriptor
} serial_port_t;


/**********************************************
* Defines
**********************************************/

#define DEBUG_SERIAL_EN 0             // Serial port debug output enable
#define PSOC6_CONNECTION_TIMEOUT 10   // Serial connection timeout in seconds
#define MONITOR_CONNECTION_TIMEOUT 10 // Serial connection timeout in seconds

#ifdef _WIN32
// @windows

/**********************************************
* WINDOWS Defines
**********************************************/

// Debug error messages
#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 22
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 5

/**********************************************
* WINDOWS Prototypes
**********************************************/

HANDLE open_serial_port(const char * device, int name_len, uint32_t baud_rate);
HANDLE _open_serial_port(const char * device, uint32_t baud_rate);
int write_port(HANDLE port, uint8_t * buffer, size_t size);
SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size);

#else
// @linux

/**********************************************
* LINUX Includes
**********************************************/

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function


/**********************************************
* LINUX Defines
**********************************************/

// Debug error messages
#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 13
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 11
#define INVALID_HANDLE_VALUE -1

/**********************************************
* LINUX Prototypes
**********************************************/

int _open_serial_port(const char * device, uint32_t baud_rate);
int open_serial_port(char * port_name, int name_len, uint32_t baud_rate);
int write_port(int fd, uint8_t * buffer, size_t size);
ssize_t read_port(int fd, uint8_t * buffer, size_t size);


#endif

#endif // SERIAL_H


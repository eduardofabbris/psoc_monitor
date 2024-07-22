#ifndef SERIAL_H
#define SERIAL_H

#include "util.h"

//------------> DEFINES
#define DEBUG_SERIAL_EN 0

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
static void print_error(const char * context);
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
int open_serial_port(const char * device, uint32_t baud_rate);
int write_port(int fd, uint8_t * buffer, size_t size);
ssize_t read_port(int fd, uint8_t * buffer, size_t size);


#endif

#endif


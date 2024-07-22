#ifndef SERIAL_H
#define SERIAL_H

#include "util.h"


#ifdef _WIN32

#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 22
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 5
#define READ_TIME_OUT_CONSTANT 5//ms
#define WRITE_TIME_OUT_CONSTANT 1//ms


SSIZE_T read_port(HANDLE port, uint8_t* buffer, size_t size);
int write_port(HANDLE port, uint8_t* buffer, size_t size);
HANDLE open_serial_port(const char* device, uint32_t baud_rate, BOOL flushBuffers);
void print_error(const char* context);

#else

#define ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE 13
#define ERROR_CODE_CANNOT_FIND_DEVICE 2
#define ERROR_CODE_ACCESS_DENIED 11
#define INVALID_HANDLE_VALUE -1


ssize_t read_port(int fd, uint8_t * buffer, size_t size);
int write_port(int fd, uint8_t * buffer, size_t size);
int open_serial_port(const char * device, uint32_t baud_rate, BOOL flushData);

#endif
// linux and windows


#endif
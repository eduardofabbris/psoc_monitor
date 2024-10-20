// Source code: https://www.pololu.com/docs/0J73/15.6
// Uses Windows API serial port functions to send and receive data

#include "include/serial.h"

#ifdef _WIN32 // @windows

static void print_error(const char * context)
{
  if (DEBUG_SERIAL_EN)
  {
      char buffer[256];
      DWORD error_code = GetLastError();
      DWORD size = FormatMessageA
          (
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
            NULL, error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            buffer, sizeof(buffer), NULL
          );
      if (size == 0) { buffer[0] = 0; }
      gotoxy(0, 0);
      fprintf(stderr, "%s: %s\n", context, buffer);
      fflush(stdout);
      getch();
  }
  else
  {
    (void) context;
  }
}

// COM ports higher than COM9 need the \\.\ prefix 
// which tells Windows to treat it as a device name rather than as a regular string, and is written as
// "\\\\.\\" in C because we need to escape the backslashes.
HANDLE open_serial_port(const char * device, int name_len, uint32_t baud_rate)
{
  char name_buf[100] = {0};
  const char *port_prefix = "\\\\.\\";

  strcpy(name_buf, port_prefix);
  strcpy(name_buf + strlen(port_prefix), device);

  return _open_serial_port(name_buf, baud_rate);
}

// Opens the specified serial port, configures its timeouts, and sets its
// baud rate.  Returns a handle on success, or INVALID_HANDLE_VALUE on failure.
HANDLE _open_serial_port(const char * device, uint32_t baud_rate)
{
  HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (port == INVALID_HANDLE_VALUE)
  {
    print_error(device);
    return INVALID_HANDLE_VALUE;
  }

  // Flush away any bytes previously read or written.
  BOOL success = FlushFileBuffers(port);
  if (!success)
  {
    print_error("Failed to flush serial port");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }

  // Configure read and write operations to time out after 1 ms.
  COMMTIMEOUTS timeouts = {0};
  timeouts.ReadIntervalTimeout = 0;
  timeouts.ReadTotalTimeoutConstant = 1;
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant = 1;
  timeouts.WriteTotalTimeoutMultiplier = 0;

  success = SetCommTimeouts(port, &timeouts);
  if (!success)
  {
    print_error("Failed to set serial timeouts");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }

  // Set the baud rate and other options.
  DCB state = {0};
  state.DCBlength = sizeof(DCB);
  state.BaudRate = baud_rate;
  state.ByteSize = 8;
  state.Parity = NOPARITY;
  state.StopBits = ONESTOPBIT;
  success = SetCommState(port, &state);
  if (!success)
  {
    print_error("Failed to set serial settings");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }

  return port;
}

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(HANDLE port, uint8_t * buffer, size_t size)
{
  DWORD written;
  BOOL success = WriteFile(port, buffer, size, &written, NULL);
  if (!success)
  {
    print_error("Failed to write to port");
    return -1;
  }
  if (written != size)
  {
    print_error("Failed to write all bytes to port");
    return -1;
  }
  return 0;
}

// Reads bytes from the serial port.
// Returns after all the desired bytes have been read, or if there is a
// timeout or other error.
// Returns the number of bytes successfully read into the buffer, or -1 if
// there was an error reading.
SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size)
{
  DWORD received;
  BOOL success = ReadFile(port, buffer, size, &received, NULL);
  if (!success)
  {
    print_error("Failed to read from port");
    return -1;
  }
  return received;
}

#endif

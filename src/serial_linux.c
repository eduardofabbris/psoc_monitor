/*******************************************************************************
* @filename: serial_linux.c
* @brief: Uses POSIX serial port functions to send and receive data
*
* Source code: https://www.pololu.com/docs/0J73/15.5
*
********************************************************************************/
#include "include/serial.h"

#ifndef _WIN32 // @linux

static void print_error(const char *msg)
{
  if (DEBUG_SERIAL_EN)
  {
    int error_code = errno;

    printf("(%d)%s: %s", error_code, strerror(error_code), msg);
    fflush(stdout);
    char dummy_ch;
    while(!read(STDIN_FILENO, &dummy_ch, 1));
  }
  else
  {
    (void) msg;
  }
}
//**************************************************************************************

// Replace '/' character with "//" and calls _open_serial_port
int open_serial_port(char * port_name, int name_len, uint32_t baud_rate)
{
    char name_buf[100] = {0};
    int j = 0;

    for (int i = 0; i < name_len; i++)
    {
        if ( port_name[i] == '/' )
        {
            name_buf[j]   = port_name[i];
            name_buf[j+1] = port_name[i];
            j += 2;

        }
        else
        {
            name_buf[j] = port_name[i];
            j++;
        }

    }
    return _open_serial_port(name_buf, baud_rate);
}
//**************************************************************************************

// Opens the specified serial port, sets it up for binary communication,
// configures its read timeouts, and sets its baud rate.
// Returns a non-negative file descriptor on success, or -1 on failure.
int _open_serial_port(const char * device, uint32_t baud_rate)
{
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd == -1)
  {
    print_error(device);
    return -1;
  }

  // Flush away any bytes previously read or written.
  int result = tcflush(fd, TCIOFLUSH);
  if (result)
  {
    print_error("tcflush failed");  // just a warning, not a fatal error
  }

  // Get the current configuration of the serial port.
  struct termios options;
  result = tcgetattr(fd, &options);
  if (result)
  {
    print_error("tcgetattr failed");
    close(fd);
    return -1;
  }

  // Turn off any options that might interfere with our ability to send and
  // receive raw binary bytes.
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

  // timeout in deciseconds
  options.c_cc[VTIME] = 0;
  //// minimum of number input read.
  options.c_cc[VMIN] = 0;

  // This code only supports certain standard baud rates. Supporting
  // non-standard baud rates should be possible but takes more work.
  switch (baud_rate)
  {
  case 4800:   cfsetospeed(&options, B4800);   break;
  case 9600:   cfsetospeed(&options, B9600);   break;
  case 19200:  cfsetospeed(&options, B19200);  break;
  case 38400:  cfsetospeed(&options, B38400);  break;
  case 115200: cfsetospeed(&options, B115200); break;
  default:
    print_error("warning: baud rate is not supported, using 9600.");
    cfsetospeed(&options, B9600);
    break;
  }
  cfsetispeed(&options, cfgetospeed(&options));

  result = tcsetattr(fd, TCSANOW, &options);
  if (result)
  {
    print_error("tcsetattr failed");
    close(fd);
    return -1;
  }

  return fd;
}
//**************************************************************************************

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(int fd, uint8_t * buffer, size_t size)
{
  ssize_t result = write(fd, buffer, size);
  if (result != (ssize_t)size)
  {
    print_error("failed to write to port");
    return -1;
  }
  return 0;
}
//**************************************************************************************

// Reads bytes from the serial port.
// Returns after all the desired bytes have been read, or if there is a
// timeout or other error.
// Returns the number of bytes successfully read into the buffer, or -1 if
// there was an error reading.
ssize_t read_port(int fd, uint8_t * buffer, size_t size)
{
  size_t received = 0;
  while (received < size)
  {
    ssize_t r = read(fd, buffer + received, size - received);
    if (r < 0)
    {
      print_error("failed to read from port");
      return -1;
    }
    if (r == 0)
    {
      // Timeout
      break;
    }
    received += r;
  }
  return received;
}
//**************************************************************************************

#endif

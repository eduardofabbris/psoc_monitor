/* Code source:
 * https://www.pololu.com/docs/0J73/15.6
 * https://www.pololu.com/docs/0J73/15.5
 * https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 * https://linux.die.net/man/2/flock
 *
*/
#include "include/serial.h"

#ifdef _WIN32 // @windows

// Opens the specified serial port, configures its timeouts, and sets its
// baud rate.  Returns a handle on success, or INVALID_HANDLE_VALUE on failure.
HANDLE open_serial_port(const char *device, uint32_t baud_rate, BOOL flushBuffers)
{
    BOOL success;
    HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (port == INVALID_HANDLE_VALUE)
    {
        print_error(device);
        return INVALID_HANDLE_VALUE;
    }

    // Flush away any bytes previously read or written.
    if (flushBuffers)
    {
        success = FlushFileBuffers(port);
        if (!success)
        {
            print_error("Failed to flush serial port\n");
            CloseHandle(port);
            return INVALID_HANDLE_VALUE;
        }
    }

    // Configure read and write operations to time out after TIME_OUT_CONSTANT ms.
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = READ_TIME_OUT_CONSTANT;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = WRITE_TIME_OUT_CONSTANT;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    success = SetCommTimeouts(port, &timeouts);
    if (!success)
    {
        print_error("Failed to set serial timeouts\n");
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
        print_error("Failed to set serial settings\n");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    return port;
}
//**************************************************************************************

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(HANDLE port, uint8_t *buffer, size_t size)
{
    DWORD written;
    BOOL success = WriteFile(port, buffer, size, &written, NULL);
    if (!success)
    {
        print_error("Failed to write to port\n");
        return -1;
    }
    if (written != size)
    {
        print_error("Failed to write all bytes to port\n");
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
SSIZE_T read_port(HANDLE port, uint8_t *buffer, size_t size)
{
    DWORD received = 0;
    BOOL success = ReadFile(port, buffer, size, &received, NULL);
    if (!success)
    {
        print_error("Failed to read from port\n");
        return -1;
    }
    return received;
}
//**************************************************************************************

void print_error(const char *context)
{
    DWORD error_code = GetLastError();
    char buffer[256];
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL, error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buffer, sizeof(buffer), NULL);
    if (size == 0)
    {
        buffer[0] = 0;
    }
    (void )context;
    // gotoxy(0,0);
    // fprintf(stderr, "%s: %s\n", context, buffer);

    /*
    printf("Error code: %lu\n", error_code);
    printf("Pess ENTER to continue... ");
    clrbuf();
    getchar();*/
}
//**************************************************************************************
#else // @linux

// Opens the specified serial port, sets it up for binary communication,
// configures its read timeouts, and sets its baud rate.
// Returns a non-negative file descriptor on success, or -1 on failure.
int open_serial_port(const char *device, uint32_t baud_rate, BOOL flushData)
{
    int result = 0;
    int fd = open(device, O_RDWR | O_NOCTTY);

    if (fd == -1)
    {
         //perror(device);
        return -1;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) == -1)
    {
        // Already opened
        close(fd);

        return -1;
    }

    // Flush away any bytes previously read or written.
    if (flushData)
    {
        result = tcflush(fd, TCIOFLUSH);
        if (result)
        {
            // perror("tcflush failed");  // just a warning, not a fatal error
        }
    }

    // Get the current configuration of the serial port.
    struct termios options;
    result = tcgetattr(fd, &options);
    if (result)
    {
        // perror("tcgetattr failed");
        close(fd);
        return -1;
    }

    // Turn off any options that might interfere with our ability to send and
    // receive raw binary bytes.
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    // Set up timeouts: Calls to read() will return as soon as there is
    // at least one byte available or when 0 ms has passed.
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;

    // This code only supports certain standard baud rates. Supporting
    // non-standard baud rates should be possible but takes more work.
    switch (baud_rate)
    {
    case 4800:
        cfsetospeed(&options, B4800);
        break;
    case 9600:
        cfsetospeed(&options, B9600);
        break;
    case 19200:
        cfsetospeed(&options, B19200);
        break;
    case 38400:
        cfsetospeed(&options, B38400);
        break;
    case 115200:
        cfsetospeed(&options, B115200);
        break;
    default:

        fprintf(stderr, "warning: baud rate %u is not supported, using 9600.\n",
                baud_rate);
        cfsetospeed(&options, B9600);
        break;
    }
    cfsetispeed(&options, cfgetospeed(&options));

    result = tcsetattr(fd, TCSANOW, &options);
    if (result)
    {
        // perror("tcsetattr failed");
        close(fd);
        return -1;
    }

    return fd;
}
//**************************************************************************************

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(int fd, uint8_t *buffer, size_t size)
{
    ssize_t result = write(fd, buffer, size);
    if (result != (ssize_t)size)
    {
        // perror("failed to write to port");
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
ssize_t read_port(int fd, uint8_t *buffer, size_t size)
{
    size_t received = 0;
    while (received < size)
    {
        ssize_t r = read(fd, buffer + received, size - received);
        if (r < 0)
        {
            // perror("failed to read from port");
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

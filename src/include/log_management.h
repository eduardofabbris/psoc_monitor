/*******************************************************************************
* @filename: log_management.h
*
* @brief:
*
*
*
*
********************************************************************************/
#pragma once

/*********************************************************
* Includes
*********************************************************/

#include "util.h"
#include "serial.h"

/*********************************************************
* Defines
*********************************************************/

// Packet
#define PCKT_BYTE_LEN   708 // Number of bytes per packet
#define BUFFER_PER_PCKT 100 // Buffer chunk per packet

// Buffer
#define MAX_BUFFER_LEN 1000 // Maximum buffer length

// File
#define FILE_HEADER_N_COL 80    // Header number of columns
#define FILE_HEADER_N_ROW 6     // Header number of rows
#define MAX_BUFFERS_PER_FILE 10 // Maximum number of buffers per file

/*********************************************************
* Typedefs
*********************************************************/

// Error descriptor
// bit 0 -> dx fault: written and read mismatch  fault, i.e. (DAC_out - ADC_in) > TOLERANCE
// bit 1 -> slope fault: slew-rate fault, i.e. (ADC_in[m + 1] - ADC_in[m]) / dt not bounded by slew-rate limits
// bit 2 ->
// bit 3 ->
// bit 4 -> dt fault:
// bit 5 -> backup time
// bit 6 -> backup DAC_out
// bit 7 -> backup ADC_in

typedef struct log_struct {

    // DUT information
    struct {
        struct {
            uint8_t error_descriptor;
            uint16_t ADC_in,                // ADC input or read sample
                     DAC_out,               // DAC output or written sample
                     dt;                    // Time difference between each sample in microseconds

        } received_buffer[MAX_BUFFER_LEN];
        uint32_t timestamp;                 // DUT timestamp indicating monitoring time
        uint8_t rx_packet_cnt;              // Received packets per buffer, should be 10
        uint8_t checksum_error,             // Checksum is different than zero
                timeout_error;              // Timeout receiving buffer packets
    } psoc;

    // Session information to be stored in file as well
    struct {
        uint64_t init_timestamp;      // Session beginning timestamp
        uint64_t end_timestamp;       // Session end timestamp
        uint64_t buffer_timestamp;    // Last buffer timestamp for date retrieve
        uint32_t buffer_cnt;          // Number of buffers received since the beginning of session
        uint32_t con_rst_cnt;         // Serial connection reset counter (alive)
        uint32_t hang_rst_cnt;        // Core hang reset counter (alive)
        uint32_t checksum_error_cnt;  // Number of checksum errors
        uint32_t packet_num;          // Total number of received packets since the beginning of session 
    } session;

    // File information
    struct {
        char name[100];       // Active file name
        uint16_t cnt;         // Number of generated files for the current session
        uint32_t buffer_cnt;  // Number of buffers in active file

    } file;

} log_info_t;

/*********************************************************
* Function Prototypes
*********************************************************/

int listen_psoc(serial_port_t *psoc_port, log_info_t *log);
void dut_rst(serial_port_t *monitor_port);

void create_new_file(log_info_t *log);
void clear_psoc_log(log_info_t *log);
void clear_session_log(log_info_t *log);
void clear_file_log(log_info_t *log);

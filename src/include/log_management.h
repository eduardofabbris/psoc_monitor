#pragma once

#include "util.h"
#include "serial.h"

// buffer_info struct -> add CRC
// File descriptor
// file_name
// file_cnt
// irst_cnt     // internal reset counter
// erst_cnt     // external reset counter
// buffer_cnt   // number of buffers in file

//typedef struct {
//
//    char file_name[20];
//    uint16_t file_n;               // number of files
//    uint32_t buffer_n;             // number of buffers in file
//    uint32_t alv_sig_rst_n;
//    uint32_t alv_serial_rst_n;
//
//} FILE_INFO;

//typedef struct {
//    struct {
//        uint16_t dtime;
//        uint16_t read, written;
//        BOOL WR_fault;
//        BOOL SR_fault;
//    } data[MAX_BUFFER_DATA];
//
//    uint16_t dataIndex;
//    uint16_t SRfaultIndex, WRfaultIndex;
//} BUFFER_DATA;
//
//
//typedef struct run_info_str {
//
//    uint32_t buffer_n;              // number of buffers received
//    uint32_t alv_sig_rst_n;         // number of device's resets due to alive signal
//    uint32_t alv_serial_rst_n;      // number of device's resets due to serial alive signal
//    BOOL alv_serial_flag;           // alive serial received flag
//    BOOL r_buffer_flag;             // reading buffer flags
//
//} MONITORING_INFO;
//
#define PCKT_BYTE_LEN   708
#define BUFFER_PER_PCKT 100

#define MAX_BUFFER_LEN 1000
typedef struct log_struct {

    // DUT information
    struct {
        struct {
            uint8_t error_descriptor;
            uint16_t ADC_in,    // ADC input or read sample
                     DAC_out,   // DAC output or written sample
                     dt;        // Time difference between each sample in microseconds
            //BOOL dx_fault;      // written and read mismatch  fault, i.e. (DAC_out - ADC_in) > TOLERANCE
            //BOOL slope_fault;   // slew-rate fault, i.e. (ADC_in[m + 1] - ADC_in[m]) / dt not bounded by slew-rate limits
            //TODO: misc_fault CRC erro during serial read or not identified 

        } received_buffer[MAX_BUFFER_LEN];
        uint16_t buffer_index;  // received buffer index during transmission, TODO:remove, not needed just for debug
        uint32_t timestamp;     // DUT timestamp indicating monitoring time
        uint32_t checksum_error_cnt;     // DUT timestamp indicating monitoring time

        // TODO: error types -> timeout, incorrect CRC( UART ),  
        // uart_timeout, uart_crc, sw_timeout
        //uint32_t buffer_cnt;  // number of buffers received since the begining of session
        //uint32_t error_cnt;   // Total number of received buffers
    } psoc;

    // Session information to be stored in file as well
    struct {
        uint64_t init_timestamp;
        uint64_t end_timestamp;
        uint64_t buffer_timestamp; // Last buffer timestamp for date retrieve
        uint32_t buffer_cnt;     // number of buffers received since the begining of session
        uint32_t con_rst_cnt;    // Serial connection reset counter (alive)
        uint32_t hang_rst_cnt;   // Core hang reset counter (alive)
    } session;

    // File information
    struct {
        char name[100];       // active file name
        uint16_t cnt;         // number of files
        uint32_t buffer_cnt;  // number of buffers in file
        //uint32_t irst_cnt;    // internal reset counter
        //uint32_t erst_cnt;    // external reset counter
        //uint32_t buffer_cnt;  // number of buffers in file

    } file;

} log_info_t;

int listen_psoc(serial_port_t *psoc_port, log_info_t *log);

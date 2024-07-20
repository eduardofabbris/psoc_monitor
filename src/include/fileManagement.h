#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include "util.h"
//#include "serial.h"
#include "serial_device.h"

#define MAX_BUFFER_DATA 1000

#define MAX_BUFFER_INDEX_IN_FILE 1000u //number of buffers in each file
#define SERIAL_BUFFER_SIZE 7


typedef struct {
    struct {
        uint16_t dtime;
        uint16_t read, written; 
        BOOL WR_fault;
        BOOL SR_fault;
    } data[MAX_BUFFER_DATA];
    
    uint16_t dataIndex;
    uint16_t SRfaultIndex, WRfaultIndex; 
} BUFFER_DATA;


typedef struct run_info_str {
 
    uint32_t buffer_n;              // number of buffers received
    uint32_t alv_sig_rst_n;         // number of device's resets due to alive signal
    uint32_t alv_serial_rst_n;      // number of device's resets due to serial alive signal
    BOOL alv_serial_flag;           // alive serial received flag
    BOOL r_buffer_flag;             // reading buffer flags 

} MONITORING_INFO;

typedef struct {

    char file_name[20];
    uint16_t file_n;               // number of files
    uint32_t buffer_n;             // number of buffers in file
    uint32_t alv_sig_rst_n;    
    uint32_t alv_serial_rst_n;  

} FILE_INFO;

void FILE_INFO_Init(FILE_INFO *self, const char *file_name);
void MONITORING_INFO_Init(MONITORING_INFO *self);

void monitorMainDevice(BUFFER_DATA* bufferPSoc, SERIAL_PORT mainDevice, MONITORING_INFO* monitoring_info, FILE_INFO *file_info);

void storeDeviceBuffer(BUFFER_DATA* bufferPSoc, uint8_t *readData);
void writeFileNewBuffer(BUFFER_DATA deviceBuffer, FILE_INFO file_info);
uint32_t writeFileHeader(char* fileName);
int32_t getHeaderData(char* fileName, uint16_t posInFile);
void manageTxtFile(FILE_INFO *file_info);
void addNewBufferInFile(BUFFER_DATA* bufferPSoc, MONITORING_INFO* monitoring_info, FILE_INFO *file_info);

BOOL monitorControlDevice(SERIAL_PORT device, MONITORING_INFO* monitoring_info, FILE_INFO *file_info);

void updateFileHeader(FILE_INFO *file_info);
#endif

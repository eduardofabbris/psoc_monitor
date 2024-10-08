/*******************************************************************************
* @filename: log_management.c
*
* @brief:
*
*
*
*
********************************************************************************/

/*********************************************************
* Includes
*********************************************************/

#include "include/log_management.h"

// listen_monitor_device

/*********************************************************
* Global Variables
*********************************************************/

// Templates
const char *log_file_header_template =
{"\
********************************************************************************\n\
* @filename:                                                                   *\n\
* @brief: This file is intended to store information about PSoc6 radiation     *\n\
*         tests                                                                *\n\
* @info:                                                                       *\n\
*                                                                              *\n\
********************************************************************************\n\
"};

// User header info
char user_header_info[100] = {0};

// Debug
uint8_t rx_byte = 0;
int psoc6_listening_fsm  = 0;

/*********************************************************
* Function Definitions
*********************************************************/

/**
* @brief   Asks monitoring device to trigger DUT reset
* @param  *monitor_port  : monitor device serial port descriptor
* @retval None
*/
void dut_rst(serial_port_t *monitor_port)
{
    write_port(monitor_port->device, (uint8_t *)"WR", 2);
    msleep(1);
}

/**
* @brief  Clear file log information
* @param  log  : log information from current session
* @retval None
*/
void clear_file_log(log_info_t *log)
{
    memset(log->file.name, '\0', 100);
    log->file.cnt        = 0;
    log->file.buffer_cnt = 0;
}
//****************************************************************************************

/**
* @brief  Clear session log information
* @param  log  : log information from current session
* @retval None
*/
void clear_session_log(log_info_t *log)
{
    log->session.init_timestamp      =
    log->session.end_timestamp       =
    log->session.buffer_timestamp    = 0;
    log->session.buffer_cnt          =
    log->session.con_rst_cnt         =
    log->session.hang_rst_cnt        =
    log->session.checksum_error_cnt  =
    log->session.packet_num          =
    log->session.con_lost_monitor    = 0;
}
//****************************************************************************************

/**
* @brief  Clear psoc log information
* @param  log  : log information from current session
* @retval None
*/
void clear_psoc_log(log_info_t *log)
{
    // Status
    log->psoc.timestamp      = 0;
    log->psoc.rx_packet_cnt  =
    log->psoc.checksum_error =
    log->psoc.timeout_error  = 0;

    // Payload
    for(int i=0; i < MAX_BUFFER_LEN; i++)
    {
        log->psoc.received_buffer[i].ADC_in           =
        log->psoc.received_buffer[i].DAC_out          =
        log->psoc.received_buffer[i].dt               =
        log->psoc.received_buffer[i].error_descriptor = 0;
    }
}
//****************************************************************************************


/**
* @brief  Convert a sequence of bytes in a integer word
* @param  data     : bytes sequence big-endian
* @param  byte_len : number of bytes to represent data
* @return A integer word
*/
static uint32_t bytes2int(uint8_t *buf_ptr, uint8_t byte_len)
{
    uint32_t result = 0;
    for (int j = 0; j < byte_len; j++)
    {
        result += buf_ptr[j] << ((byte_len-1)-j)*8;
    }
    return result;
}
//****************************************************************************************

/**
* @brief  Decode packet into buffer format
* @param  *pckt    : packet pointer
* @param  log      : log information from current session
* @return True if received a complete new buffer
*/
static uint8_t process_new_pckt(uint8_t *pckt, log_info_t *log)
{
    static uint16_t last_index = 0;
    uint16_t sequence = 0;
    uint8_t *pckt_ptr = pckt;
    uint8_t new_buffer_flag = 0;

    // Get buffer offset
    sequence = bytes2int(pckt_ptr, 2);
    pckt_ptr += 2;

    // Get DUT timestamp
    log->psoc.timestamp = bytes2int(pckt_ptr, 4);
    pckt_ptr += 4;

    // Get payload
    int i;
    for (i = 0; i < BUFFER_PER_PCKT; i++)
    {
        log->psoc.received_buffer[sequence + i].error_descriptor = *pckt_ptr;
        pckt_ptr++;
        log->psoc.received_buffer[sequence + i].dt = bytes2int(pckt_ptr, 2);
        pckt_ptr += 2;
        log->psoc.received_buffer[sequence + i].ADC_in = bytes2int(pckt_ptr, 2);
        pckt_ptr += 2;
        log->psoc.received_buffer[sequence + i].DAC_out = bytes2int(pckt_ptr, 2);
        pckt_ptr += 2;

    }
    // TODO: identify duplicate packets

    // New buffer
    if ( (sequence + i == MAX_BUFFER_LEN) && ( sequence + i != last_index) )
    {
        log->session.buffer_cnt++;
        new_buffer_flag = 1;
    }
    last_index = sequence + i;

    return new_buffer_flag;
}
//****************************************************************************************


/**
* @brief  Creates a new file
* @param  log   : log information from current session
* @retval None
*/
void create_new_file(log_info_t *log)
{
    FILE *ptr = NULL;
    char name_buffer[100] = {0};
    char *name_ptr;
    char file_header[FILE_HEADER_N_COL*FILE_HEADER_N_ROW] = {0};

    if(WINDOWS_EN)
    {
        system("if not exist \"log\" mkdir \"log\"");
    }
    else
    {
        system("mkdir -p log");
    }

    // Creates new file name
    sprintf(name_buffer, "log%slog_%lu_%u.txt", FILE_SEPARATOR, log->session.init_timestamp, log->file.cnt);
    memcpy(log->file.name, name_buffer, strlen(name_buffer));

    // Creates header
    memcpy(file_header, log_file_header_template  , strlen(log_file_header_template ));

    // Copy file name to header
    name_ptr = name_buffer + 3 + strlen(FILE_SEPARATOR);
    memcpy(file_header + FILE_HEADER_N_COL*1 + 14, name_ptr, strlen(name_ptr));

    // Copy additional user info to header
    memcpy(file_header + FILE_HEADER_N_COL*4 + 13, user_header_info, strlen(user_header_info));

    ptr = fopen(name_buffer, "w");
    if(ptr != NULL)
    {
        fprintf(ptr, "%s\n", file_header);

        fclose(ptr);
    }

    // Clear user input
    memset(user_header_info, '\0', sizeof(user_header_info));

    // Increment file counter
    log->file.cnt += 1;

}
//****************************************************************************************

/**
* @brief  Append new buffer information to active file
* @param  log   : log information from current session
* @retval None
*/
static void append_psoc_log(log_info_t log)
{
    FILE *ptr;

    ptr = fopen(log.file.name, "a");
    if(ptr != NULL)
    {
        const char *dateAndTime = get_timeinfo(log.session.buffer_timestamp);
        // Header
        fprintf
            (
                ptr,
                "@B%u, %u - received at (%lu) %s",
                log.file.buffer_cnt,
                log.psoc.timestamp,
                log.session.buffer_timestamp,
                dateAndTime
            );
        // Buffer serial reception information
        fprintf
            (
                ptr,
                "received packets: %d, checksum error: %d, timeout error: %d\n",
                log.psoc.rx_packet_cnt,
                log.psoc.checksum_error,
                log.psoc.timeout_error
            );

        // Payload
        for(int i=0; i < MAX_BUFFER_LEN; i++)
        {

            fprintf(ptr, "%-6d\t%-6d\t%-6d\t%-6d\n",
                log.psoc.received_buffer[i].ADC_in,
                log.psoc.received_buffer[i].DAC_out,
                log.psoc.received_buffer[i].dt,
                log.psoc.received_buffer[i].error_descriptor
            );

        }
        fclose(ptr);
    }

}
//****************************************************************************************

/**
* @brief  Append session information to active file
* @param  log   : log information from current session
* @retval None
*/
void append_session_log(log_info_t log)
{
    FILE *ptr;

    ptr = fopen(log.file.name, "a");
    if(ptr != NULL)
    {
        // Session status
        fprintf
            (
                ptr,
                "@S init tm: %lu, end tm: %lu, tot bufs: %u, con rsts: %u, hang rsts: %u, chsum errors: %u, monit con: %u\n",
                log.session.init_timestamp,
                log.session.end_timestamp,
                log.session.buffer_cnt,
                log.session.con_rst_cnt,
                log.session.hang_rst_cnt,
                log.session.checksum_error_cnt,
                log.session.con_lost_monitor
            );

        fclose(ptr);
    }
}
//****************************************************************************************

/**
* @brief  Listen to DUT via serial
* @param  *psco_port  : DUT serial port descriptor
* @param  *log        : log information from current session
* @retval None
*/
int listen_psoc(serial_port_t *psoc_port, log_info_t *log)
{
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_OP_SEL_ST,
            FSM_READ_PCKT_ST
        };

    static int fsm_st = FSM_IDLE_ST;
    static uint32_t checksum = 0;
    static uint8_t rx_pckt[1024] = {0};
    static int byte_cnt = 0;


    static clock_t packet_timer;

    uint8_t proc_pckt_flag  = 0,
            new_buf_flag    = 0,
            append_buf_flag = 0;

    uint8_t read_buffer[4096] = {0};
    uint8_t *read_ptr = read_buffer;
    int buf_len = 0;


    // Try to fill input buffer
    buf_len = read_port(psoc_port->device, read_buffer, sizeof(read_buffer));

    // Process input buffer
    while(buf_len > 0)
    {
        switch (fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:
                // Reset variables
                proc_pckt_flag = 0;
                byte_cnt = 0;
                memset(rx_pckt, '\0', sizeof(rx_pckt));

                // 'DUT' device ID
                if (*read_ptr == 'D')
                {
                    fsm_st = FSM_OP_SEL_ST;
                }

                break;
            // Select operation
            case FSM_OP_SEL_ST:

                // Read new data packet
                if (*read_ptr == 'P')
                {
                    new_buf_flag = 1;
                    log->session.packet_num++;
                    checksum = 0x4450;
                    psoc_port->status = 1;
                    psoc_port->timeout_cnt = get_clock();
                    fsm_st = FSM_READ_PCKT_ST;
                }
                // Alive signal received
                else if (*read_ptr == 'A')
                {
                    psoc_port->status = 1;
                    psoc_port->timeout_cnt = get_clock();
                    fsm_st = FSM_IDLE_ST;

                }
                else
                {
                    fsm_st = FSM_IDLE_ST;
                }

                break;

            // Receive new packet
            case FSM_READ_PCKT_ST:
                packet_timer = get_clock();

                // Build new packet
                checksum += (byte_cnt & 1) ? *read_ptr : (*read_ptr << 8);
                rx_pckt[byte_cnt] = *read_ptr;
                if (byte_cnt < PCKT_BYTE_LEN - 1)
                {
                    byte_cnt++;
                }
                else
                {
                    // Add carry bits
                    checksum += (checksum >> 16) & 0xF;
                    // One's complement
                    checksum = ~checksum;
                    // Get only 16 LSB
                    checksum = checksum & 0xFFFF;

                    if (checksum != 0)
                    {
                        log->psoc.checksum_error = 1;
                        log->session.checksum_error_cnt += 1;
                        // Send acknowledge
                        //write_port(psoc_port->device, (uint8_t *)"HR", 2);
                    }

                    proc_pckt_flag = 1;
                    log->psoc.rx_packet_cnt += 1;

                    fsm_st = FSM_IDLE_ST;
                }
                break;

            // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
                break;

        }
        read_ptr++;
        buf_len--;

        // Process new packet
        if (proc_pckt_flag)
        {
            // End of buffer
            if( process_new_pckt(rx_pckt, log) > 0 )
            {
                log->session.buffer_timestamp = time(NULL);
                append_buf_flag = 1;
                new_buf_flag = 0;
            }

        }
        // Packet timeout - buffer bad formation
        if ( new_buf_flag && time_diff(packet_timer) > 250 )
        {
            log->psoc.timeout_error = 1;
            append_buf_flag = 1;
            new_buf_flag = 0;
        }

        // Append buffer
        if ( append_buf_flag )
        {
            append_buf_flag = 0;

            // Verify file size
            if ( log->file.buffer_cnt < MAX_BUFFERS_PER_FILE )
            {
                append_psoc_log(*log);
                log->file.buffer_cnt += 1;
            }
            else
            {
                create_new_file(log);
                log->file.buffer_cnt = 0;
            }

            // Clear buffer
            clear_psoc_log(log);

        }

    }
    psoc6_listening_fsm = fsm_st;
    // process data flag

    return 0;
}
//****************************************************************************************

/**
* @brief  Listen to Monitor Device via serial
* @param  *monitor_port  : monitor serial port descriptor
* @retval True if DUT device needs to be restarted
*/
uint8_t listen_monitor_device(serial_port_t *monitor_port)
{
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_OP_SEL_ST
        };

    static int fsm_st = FSM_IDLE_ST;

    uint8_t need_rst = 0;
    uint8_t read_buffer[4096] = {0};
    uint8_t *read_ptr = read_buffer;
    int buf_len = 0;


    // Try to fill input buffer
    buf_len = read_port(monitor_port->device, read_buffer, sizeof(read_buffer));

    // Process input buffer
    while(buf_len > 0)
    {
        switch (fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:

                // External Watchdog ID
                if (*read_ptr == 'W')
                {
                    fsm_st = FSM_OP_SEL_ST;
                }

                break;
            // Select operation
            case FSM_OP_SEL_ST:

                // Alive signal received
                if (*read_ptr == 'A')
                {
                    monitor_port->status = 1;
                    monitor_port->timeout_cnt = get_clock();
                }
                // Core hang timeout indication
                else if (*read_ptr == 'T')
                {
                    monitor_port->status = 1;
                    monitor_port->timeout_cnt = get_clock();

                    need_rst = 1;

                }
                fsm_st = FSM_IDLE_ST;

                break;

            // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
                break;

        }
        read_ptr++;
        buf_len--;

    }

    return need_rst;
}
//****************************************************************************************

/**
* @brief  Listen to serial device command
* @param  *device_port  : device's serial port descriptor
* @param  *device_cmd   : the command to listen to
* @retval True if the desired command was read
*/
uint8_t attempt_connection(serial_port_t *device_port, const char *device_cmd)
{
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_OP_SEL_ST
        };

    int fsm_st = FSM_IDLE_ST;
    int buf_len = 0;
    uint8_t received_cmd = 0;
    uint8_t read_buffer[4096] = {0};
    uint8_t *read_ptr = read_buffer;


    // Try to fill input buffer
    buf_len = read_port(device_port->device, read_buffer, sizeof(read_buffer));

    // Process input buffer
    while(buf_len > 0)
    {
        switch (fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:

                // Device ID
                if (*read_ptr == device_cmd[0])
                {
                    fsm_st = FSM_OP_SEL_ST;
                }

                break;
            // Select operation
            case FSM_OP_SEL_ST:

                // Device operation
                if (*read_ptr == device_cmd[1])
                {
                    received_cmd = 1;
                }
                fsm_st = FSM_IDLE_ST;

                break;

            // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
                break;

        }
        read_ptr++;
        buf_len--;

    }

    return received_cmd;
}
//****************************************************************************************

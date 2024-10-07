#include "include/log_management.h"

// create_new_file
// append_log
// listen_psoc
// listen_monitor_device

uint8_t rx_byte = 0;
int psoc6_listening_fsm  = 0;
int pckt_num = 0;
uint32_t global_checksum = 0;


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

        log->psoc.buffer_index = sequence + i;
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


int listen_psoc(serial_port_t *psoc_port, log_info_t *log)
{
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_OP_SEL_ST,
            FSM_READ_PCKT_ST,
            FSM_EXIT_PROMT_ST
        };

    static int fsm_st = FSM_IDLE_ST;
    static uint32_t checksum = 0;
    static uint8_t rx_pckt[1024] = {0};
    static int byte_cnt = 0;


    //uint16_t sequence = 0;
    //uint8_t *pckt_ptr;
    uint8_t process_pckt = 0;

    uint8_t read_buffer[1024] = {0};
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
                process_pckt = 0;
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
                    pckt_num++;
                    checksum = 0x4450;
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
                // TODO: add timeout?

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

                    // Send acknowledge
                    if (checksum == 0)
                    {
                        write_port(psoc_port->device, (uint8_t *)"HR", 2);
                        process_pckt = 1;
                        // TODO: store error otherwise
                    }

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
        if (process_pckt)
        {
            process_new_pckt(rx_pckt, log);

        }


    }
    psoc6_listening_fsm = fsm_st;
    global_checksum = checksum;
    // process data flag

    return 0;
}


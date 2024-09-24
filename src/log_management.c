#include "include/log_management.h"

// create_new_file
// append_log 
// listen_psoc
// listen_monitor_device

int listen_psoc(serial_port_t *psoc_port, log_info_t *log)
{
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_OP_SEL_ST,
            FSM_READ_PCKT_ST,
            FSM_EXIT_PROMT_ST
        };

    static int fsm_st = FSM_IDLE_ST;

    uint8_t read_buffer[1024] = {0};
    uint8_t *read_ptr = read_buffer;

    int buf_len = 0;
    // Try to fill buffer
    buf_len = read_port(psoc_port->device, read_buffer, sizeof(read_buffer));

    // Process buffer
    while(buf_len > 0)
    {
        switch (fsm_st)
        {
            // Idle state 
            case FSM_IDLE_ST:

                // 'Main' device ID
                // TODO: change ID
                if (*read_ptr == 'M')
                {
                    fsm_st = FSM_OP_SEL_ST;
                }

                break;

            // Select operation 
            case FSM_OP_SEL_ST:

                // Read new data packet
                if (*read_ptr == 'D')
                {
                    fsm_st = FSM_READ_PCKT_ST;
                }
                // Alive signal received TODO: status package
                else if (*read_ptr == 'A')
                {
                    psoc_port->status = 1;
                    psoc_port->timeout_cnt = get_clock(); 
                }
                // Waiting start TODO: remove 
                else if (*read_ptr == 'W')
                {
                    psoc_port->status = 1;
                    psoc_port->timeout_cnt = get_clock(); 
                }
                // Invalid operation
                else
                {
                    fsm_st = FSM_IDLE_ST;
                }

                break;

            // Exit prompt
            //case FSM_EXIT_PROMT_ST:

            //    if (user_input == ENTER)
            //    {
            //        exit_flag = 1;
            //    }
            //    else
            //    {
            //        status_prompt = NULL;
            //        fsm_st = FSM_IDLE_ST;
            //    }

            //    break;

            // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
                break;

        }

        buf_len -= 1;
        read_ptr += 1;
    }

    return 0;
}


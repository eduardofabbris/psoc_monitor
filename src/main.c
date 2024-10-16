/*******************************************************************************
* @filename: main.c
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

#include "include/util.h"
#include "include/menu.h"
#include "include/serial.h"
#include "include/log_management.h"


/*********************************************************
* Function Prototypes
*********************************************************/

int manage_monitor_menu(serial_port_t *psoc_port, serial_port_t *monitor_port);

/*********************************************************
* Global Variables
*********************************************************/

// Debug
extern int psoc6_listening_fsm;
extern uint16_t global_cooldown_lvl;
extern clock_t global_rst_cooldown_timer;

// User additional information
extern char user_header_info[100];

// Templates
extern char *main_menu_template;
extern char *monitor_ascii_art[];
extern char *monitor_menu_template;
extern char *debug_menu_template;
extern char *prompt_menu_template;

const char *connected_status_template        =  "Connected";
const char *error_status_template            =  "Error connecting to port!";
const char *exit_prompt_template             =  "Are you sure you want to Exit?";
const char *progrss_spinner_template         = "|/-\\";
const char *attempting_status_template       =  "Attempting connection...";
const char *lost_connection_status_template  =  "Lost connection...";
const char *session_abort_template           =  "Session aborted: psoc not answering...";


/*********************************************************
* Function Definitions
*********************************************************/

/**
* @brief  Initial terminal configuration
* @retval None
*/
static void init_terminal()
{
    if(WINDOWS_EN)
    {
        //system("MODE con cols=86 lines=18");
    }
    else
    {
        set_nonblock(1);
        hide_cursor(1);

        system("echo -ne '\e[8;18;86t'");
    }
}
//**************************************************************************************

/**
* @brief  Reset default terminal configurations
* @retval None
*/
static void reset_terminal()
{
    if(WINDOWS_EN)
    {
        //system("MODE con cols=80 lines=22");
        //system("MODE con cols=120 lines=30");
    }
    else
    {
        set_nonblock(0);
        hide_cursor(0);

        //system("echo -ne '\e[8;22;80t'");
        system("echo -ne '\e[8;24;80t'");

    }
    clrscr();
}
//**************************************************************************************

/**
* @brief  Main menu or code entry
* @return Zero
*/
int main(){

    serial_port_t psoc_port, monitor_port;
    enum menu_st
    {
        FSM_IDLE_ST,
        FSM_GET_PSOC_PORT_ST,
        FSM_ATTMP_PSOC_CONNECT_ST,
        FSM_GET_MONITOR_DEVICE_PORT_ST,
        FSM_ATTMP_MONITOR_CONNECT_ST,
        FSM_GET_LOG_INFO_ST
    };

    // Timer and counters
    clock_t attempt_connection_timer;
    uint8_t attempt_status = 0;

    // Screen
    int menu_option = 0, temp = 0, str_len = 0;
    char input_layer[TERM_N_ROW*TERM_N_COL] = {0};
    char user_input[100] = {0};
    char *background    = NULL,
         *status_prompt = NULL;

    // FSM
    int fsm_st;

    // Configure terminal
    init_terminal();

    // Manage Main Menu
    while(menu_option != MENU_N_OPTIONS - 1)
    {
        switch (fsm_st)
        {
            // Listen to menu option
            case FSM_IDLE_ST:
                // Print status
                if (status_prompt != NULL)
                {
                    temp = strlen(status_prompt);
                    memcpy(input_layer + TERM_N_COL*13 + 22, status_prompt, temp);
                }

                menu_option = get_menu_option(input_layer);

                if (menu_option == 0)
                {
                    background = prompt_menu_template;

                    // Reset variables
                    psoc_port.last_status = 1;
                    monitor_port.last_status = 1;
					memset(user_header_info, '\0', sizeof(user_header_info));
                    memset(psoc_port.name, '\0', sizeof(psoc_port.name));
                    memset(monitor_port.name, '\0', sizeof(monitor_port.name));
                    status_prompt = NULL;
                    fsm_st = FSM_GET_PSOC_PORT_ST;
                }
                else
                {
                    background = main_menu_template;
                }

                break;

            // Connect PSoC 6 Serial port
            case FSM_GET_PSOC_PORT_ST:
                str_len = get_keyboard_str(input_layer + PSOC_PROMPT_OFFSET, user_input, 20);

                if (status_prompt != NULL)
                {
                    temp = strlen(status_prompt);
                    memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET+1) - 5 - temp, status_prompt, temp);
                }

                // Cancel
                if (str_len == -1)
                {
                    fsm_st = FSM_IDLE_ST;
                    status_prompt = NULL;
                }
                // Verify serial port
                else if (str_len > 0)
                {
                    memcpy(psoc_port.name, user_input, str_len);
                    psoc_port.name_len = str_len;

                    // Attempt connection
                    psoc_port.device = open_serial_port(psoc_port.name, psoc_port.name_len, 115200);

                    // Verify port
                    //if (psoc_port.device != INVALID_HANDLE_VALUE)
                    if (psoc_port.device > 0)
                    {
                        // Verify device
                        attempt_status = 0;
                        attempt_connection_timer = get_clock();
                        fsm_st = FSM_ATTMP_PSOC_CONNECT_ST;
                    }
                    else
                    {
                        status_prompt = (char *) error_status_template;
                    }
                }

                break;

            // Attempt PSoc 6 Serial Port Connection
            case FSM_ATTMP_PSOC_CONNECT_ST:
                str_len = get_keyboard_str(NULL, NULL, 1);

                // Print selected ports
                memcpy(input_layer + PSOC_PROMPT_OFFSET,    psoc_port.name, psoc_port.name_len);

                // Print status
                temp = strlen(attempting_status_template);
                memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET + 1) - 5 - temp, attempting_status_template, temp);

                if (str_len == -1 )
                {
                    // Close open ports
                    close_serial_port(psoc_port.device);
                    status_prompt = NULL;
                    fsm_st = FSM_GET_PSOC_PORT_ST;
                }
                else
                {
                    if ( attempt_status == 0 && time_diff(attempt_connection_timer) < PSOC6_CONNECTION_TIMEOUT*1000 )
                    {
                        attempt_status = attempt_connection(&psoc_port, "DA");
                    }
                    else if (attempt_status)
                    {
                        fsm_st = FSM_GET_MONITOR_DEVICE_PORT_ST;
                        status_prompt = NULL;
                    }
                    else
                    {
                        fsm_st = FSM_GET_PSOC_PORT_ST;
                        status_prompt = (char *) error_status_template;
                    }
                }

                break;

            // Connect Monitoring Device Serial Port
            case FSM_GET_MONITOR_DEVICE_PORT_ST:
                str_len = get_keyboard_str(input_layer + MONITOR_PROMPT_OFFSET, user_input, 20);

                // Print selected port
                memcpy(input_layer + PSOC_PROMPT_OFFSET, psoc_port.name, psoc_port.name_len);

                // Print status
                temp = strlen(connected_status_template);
                memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET + 1) - 5 - temp, connected_status_template, temp);

                if (status_prompt != NULL)
                {
                    temp = strlen(status_prompt);
                    memcpy(input_layer + TERM_N_COL*(MONITOR_PROMPT_ROW_OFFSET + 1) - 5 - temp, status_prompt, temp);
                }

                // Cancel
                if (str_len == -1)
                {
                    // Close open ports
                    close_serial_port(psoc_port.device);

                    status_prompt = NULL;
                    fsm_st = FSM_IDLE_ST;
                }
                // Verify port
                else if (str_len > 0)
                {
                    memcpy(monitor_port.name, user_input, str_len);
                    monitor_port.name_len = str_len;

                    if (strcmp(monitor_port.name, psoc_port.name) != 0)
                    {
                        // Attempt connection
                        monitor_port.device = open_serial_port(monitor_port.name, monitor_port.name_len, 115200);

                        //if (monitor_port.device != INVALID_HANDLE_VALUE)
                        if (monitor_port.device > 0)
                        {
                            // Verify device
                            attempt_status = 0;
                            attempt_connection_timer = get_clock();
                            fsm_st = FSM_ATTMP_MONITOR_CONNECT_ST;
                        }
                        else
                        {
                            status_prompt = (char *) error_status_template;
                        }
                    }
                    else
                    {
                        status_prompt = (char *) error_status_template;
                    }
                }

                break;

            // Attempt Monitoring Device Serial Port Connection
            case FSM_ATTMP_MONITOR_CONNECT_ST:
                str_len = get_keyboard_str(NULL, NULL, 1);

                // Print selected ports
                memcpy(input_layer + PSOC_PROMPT_OFFSET,    psoc_port.name, psoc_port.name_len);
                memcpy(input_layer + MONITOR_PROMPT_OFFSET, monitor_port.name, monitor_port.name_len);

                // Print status
                temp = strlen(connected_status_template);
                memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET + 1)    - 5 - temp, connected_status_template, temp);
                temp = strlen(attempting_status_template);
                memcpy(input_layer + TERM_N_COL*(MONITOR_PROMPT_ROW_OFFSET + 1) - 5 - temp, attempting_status_template, temp);

                if (str_len == -1 )
                {
                    // Close open ports
                    close_serial_port(monitor_port.device);
                    status_prompt = NULL;
                    fsm_st = FSM_GET_MONITOR_DEVICE_PORT_ST;
                }
                else
                {
                    if ( attempt_status == 0 && time_diff(attempt_connection_timer) < MONITOR_CONNECTION_TIMEOUT*1000 )
                    {
                        // TODO: if cable not connected in DUT, it only sends WT
                        attempt_status = attempt_connection(&monitor_port, "WA");
                    }
                    else if (attempt_status)
                    {
                        fsm_st = FSM_GET_LOG_INFO_ST;
                    }
                    else
                    {
                        fsm_st = FSM_GET_MONITOR_DEVICE_PORT_ST;
                        status_prompt = (char *) error_status_template;
                    }
                }

                break;

            // Get addition info to append to log file
            case FSM_GET_LOG_INFO_ST:
                str_len = get_keyboard_str(input_layer + LOG_PROMPT_OFFSET, user_input, 50);

                // Print selected ports
                memcpy(input_layer + PSOC_PROMPT_OFFSET,    psoc_port.name, psoc_port.name_len);
                memcpy(input_layer + MONITOR_PROMPT_OFFSET, monitor_port.name, monitor_port.name_len);

                // Print status
                temp = strlen(connected_status_template);
                memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET + 1)    - 5 - temp, connected_status_template, temp);
                memcpy(input_layer + TERM_N_COL*(MONITOR_PROMPT_ROW_OFFSET + 1) - 5 - temp, connected_status_template, temp);

                // Cancel
                if (str_len == -1)
                {
                    // Close open ports
                    close_serial_port(psoc_port.device);
                    close_serial_port(monitor_port.device);

                    status_prompt = NULL;
                    fsm_st = FSM_IDLE_ST;
                }
                // Creates file with timestamp and begin monitoring
                else if (str_len >= 0)
                {
                    // Copy port info and check port
                    memcpy(user_header_info, user_input, str_len);

                    // Rest port timeouts
                    psoc_port.timeout_cnt = get_clock();
                    monitor_port.timeout_cnt = get_clock();

                    // Monitoring management
                    if (manage_monitor_menu(&psoc_port, &monitor_port) == 0)
                    {
                        status_prompt = (char *)session_abort_template;
                    }
                    else
                    {
                        status_prompt = NULL;
                    }

                    // Close open ports
                    close_serial_port(psoc_port.device);
                    close_serial_port(monitor_port.device);

                    fsm_st = FSM_IDLE_ST;
                }

                break;
            default:
                fsm_st = FSM_IDLE_ST;
                break;
        }

        if (background != NULL)
        {
            update_screen(background, input_layer, NULL);
            //while(1);
            msleep(10);
        }
    }

    // Return terminal to default configuration
    reset_terminal();

    return 0;
}
//**************************************************************************************

/**
* @brief  Monitor menu management
* @param  *psoc_port    : DUT serial port descriptor
* @param  *monitor_port : Monitor device serial port descriptor
* @return Zero
*/
int manage_monitor_menu(serial_port_t *psoc_port, serial_port_t *monitor_port)
{
    log_info_t monitoring_info;
    enum monitor_menu_st
    {
        FSM_IDLE_ST,
        FSM_TOGGLE_DEBUG_ST,
        FSM_INJECT_FAULT_ST,
        FSM_EXIT_PROMT_ST
    };

    // Screen
    char input_layer[TERM_N_ROW*TERM_N_COL] = {0};
    char user_input = 0;
    char *status_prompt = NULL;
    char info_buffer[100] = {0};
    int  spinner_cnt = 0, temp;

    // Timers and counters
    double  psoc_timer,
            monitor_timer;
    time_t  temp_timestamp   = 0;
    clock_t spinner_timer    = 0,
            new_buffer_timer = 0;

    // Flags
    uint8_t new_buf_flag  = 0,
            dut_hang_flag = 0,
            dut_hang_reg  = 0,
            response      = 1;
    uint8_t reset_request = 0,
            rst_ctrl_dsc  = 0;
    // FSM
    int fsm_st      = FSM_IDLE_ST,
        exit_flag   = 0,
        debug_flag  = 0;

    // Send reset command to reset DUT timestamp
    dut_rst(monitor_port);

    // Clear variables
    clear_psoc_log(&monitoring_info);
    clear_session_log(&monitoring_info);
    clear_file_log(&monitoring_info);

    // Get initial timestamp
    monitoring_info.session.init_timestamp = time(NULL);

    // Creates new file
    create_new_file(&monitoring_info);

    while(!exit_flag)
    {
        // Listen to devices
        new_buf_flag  = listen_psoc(psoc_port, &monitoring_info);
        dut_hang_flag = listen_monitor_device(monitor_port);

        // New buffer reset cooldown
        if ( dut_hang_flag != dut_hang_reg )
        {
            // Rising edge
            if (dut_hang_flag == 1)
            {
                new_buffer_timer = get_clock();
            }
            dut_hang_reg = dut_hang_flag;
        }

        // DUT core hang timeout reset request
		// Before resetting device, verify if possible hang was due to new buffer transmission
        if ( dut_hang_flag && new_buf_flag == 0 && time_diff(new_buffer_timer) > 600 )
        {
            reset_request |= (1 << 7);
        }
        else
        {
            reset_request &= ~(1 << 7);
        }

        // Verify PSoC UART timeout
        psoc_timer = time_diff(psoc_port->timeout_cnt);
        if ( psoc_timer > PSOC6_CONNECTION_TIMEOUT*1000 )
        {
            psoc_port->status = 0;
            // Update screen device status
            temp = strlen(lost_connection_status_template);
            memcpy(input_layer + TERM_N_COL*6 + 52, lost_connection_status_template, temp);
        }
        // Serial port timeout reset request
        if (psoc_port->status == 0)
        {
            reset_request |= (1 << 0);
        }
        else
        {
            reset_request &= ~(1 << 0);
        }
        status_checker("psoc6", psoc_port, &monitoring_info);

        // Verify Monitoring Device UART timeout
        monitor_timer = time_diff(monitor_port->timeout_cnt);
        if (monitor_timer > MONITOR_CONNECTION_TIMEOUT*1000)
        {
            monitor_port->status = 0;
            // Update screen device status
            temp = strlen(lost_connection_status_template);
            memcpy(input_layer + TERM_N_COL*7 + 54, lost_connection_status_template, temp);
            monitoring_info.session.con_lost_monitor = 1;
        }
        status_checker("watchdog", monitor_port, &monitoring_info);

        // Handle reset requests
        rst_ctrl_dsc = rst_controller(reset_request, &monitoring_info);

        // Reset DUT device
        if (rst_ctrl_dsc & 1)
        {
            dut_rst(monitor_port);
            // Clear request
            reset_request = 0;
        }

        // Reset attempts didn't work, terminate session
        if ((rst_ctrl_dsc >> 7) & 1)
        {
            temp_timestamp = time(NULL);
            sprintf(info_buffer, "@a (%lu) session abort - psoc device unmanageable", temp_timestamp);
            append_msg_log(info_buffer, monitoring_info);
            response  = 0;
            exit_flag = 1;
        }

        if(kbhit())
        {
            // Read a key form keyboard
            user_input = get_char();

            switch (fsm_st)
            {
                // Listen to menu option
                case FSM_IDLE_ST:
                    exit_flag = 0;

                    if (user_input == ESC)
                    {
                        status_prompt = (char *) exit_prompt_template;
                        fsm_st = FSM_EXIT_PROMT_ST;
                    }
                    else if (user_input == 'D')
                    {
                        fsm_st = FSM_TOGGLE_DEBUG_ST;
                    }
                    else if (user_input == 'E')
                    {
                        fsm_st = FSM_INJECT_FAULT_ST;
                    }

                    break;

                // Toggle debug mode
                case FSM_TOGGLE_DEBUG_ST:

                    if (user_input == 'M')
                    {
                        debug_flag = !debug_flag;
                    }
                    fsm_st = FSM_IDLE_ST;

                    break;

                // Emulate errors for Debug
                // @note: This debug feature needs to be enabled in DUT device
                case FSM_INJECT_FAULT_ST:

                    // Single bit flip or upset
                    if (user_input == 'u')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Eu", 2);
                    }
                    // Time hold
                    else if (user_input == 't')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Et", 2);
                    }
                    // Stuck in a sample or DAC latch
                    else if (user_input == 'l')
                    {
                        write_port(psoc_port->device, (uint8_t *)"El", 2);
                    }
                    // Core hang in a infinite loop
                    else if (user_input == 'h')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Eh", 2);
                    }
                    // Redundant memory buffer mismatch
                    else if (user_input == 'm')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Em", 2);
                    }
                    // Stop serial alive messages
                    else if (user_input == 's')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Es", 2);
                    }
                    fsm_st = FSM_IDLE_ST;

                    break;

                // Exit prompt
                case FSM_EXIT_PROMT_ST:

                    if (user_input == ENTER)
                    {
                        exit_flag = 1;
                    }
                    else
                    {
                        status_prompt = NULL;
                        fsm_st = FSM_IDLE_ST;
                    }

                    break;

                default:
                    fsm_st = FSM_IDLE_ST;
                    break;

            }
        }

        // Change default prompt
        if (status_prompt != NULL)
        {
            temp = strlen(status_prompt);
            memcpy(input_layer + STATUS_PROMPT_OFFSET , status_prompt, temp);
        }
        // Update spinner animation
        else
        {
            input_layer[STATUS_PROMPT_OFFSET + DEFAULT_STATUS_PROMPT_LEN + 1] = progrss_spinner_template[spinner_cnt];
            if ( time_diff(spinner_timer) >= 100)
            {
                spinner_cnt = spinner_cnt < SPINNER_ANIMATION_LEN ? spinner_cnt + 1 : 0;
                spinner_timer = get_clock();
            }
        }

        // Number of received buffers info
        sprintf(info_buffer, "%d",  monitoring_info.session.buffer_cnt);
        temp = strlen(info_buffer);
        memcpy(input_layer + TERM_N_COL*11 + 53 , info_buffer, temp);

        // Debug menu enabled
        if (debug_flag)
        {

            // Listening PSoC FSM state
            double temp_timer = global_cooldown_lvl - time_diff(global_rst_cooldown_timer)/1000;
            sprintf(info_buffer, "FSM sate: %d, cooldown level: %d sec, cooldown: %.2f", psoc6_listening_fsm, global_cooldown_lvl, temp_timer);
            temp = strlen(info_buffer);
            memcpy(input_layer + 2*TERM_N_COL - 2 - temp, info_buffer, temp);

            // DUT connection timeout info
            sprintf(info_buffer, "%.2f",  PSOC6_CONNECTION_TIMEOUT - psoc_timer/1000);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*12 + 68 , info_buffer, temp);

            // Watchdog connection timeout info
            sprintf(info_buffer, "%.2f",  MONITOR_CONNECTION_TIMEOUT - monitor_timer/1000);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*13 + 67 , info_buffer, temp);

            // DUT connection timeout resets (alive signal from serial)
            sprintf(info_buffer, "%d",  monitoring_info.session.con_rst_cnt);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*14 + 66 , info_buffer, temp);

            // Core hang timeout resets (alive signal)
            sprintf(info_buffer, "%d",  monitoring_info.session.hang_rst_cnt);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*15 + 64 , info_buffer, temp);

            // Elapsed minutes since session start
            time_t progress_timestamp = time(NULL);
            sprintf(info_buffer, "%.2f",  (double)(progress_timestamp - monitoring_info.session.init_timestamp) / 60);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*12 + 18 , info_buffer, temp);

            // Total received packets since the start of session
            sprintf(info_buffer, "%u",  monitoring_info.session.packet_num);
            temp = strlen(info_buffer);
            memcpy(input_layer + TERM_N_COL*13 + 21 , info_buffer, temp);

            update_screen(debug_menu_template, input_layer, NULL);
        }
        else
        {
            update_screen(monitor_menu_template, input_layer, monitor_ascii_art);
        }
        msleep(10);
    }

    // Get end of session timestamp
    monitoring_info.session.end_timestamp = time(NULL);

    // Append to file session info
    append_session_log(monitoring_info);

    return response;
}
//**************************************************************************************


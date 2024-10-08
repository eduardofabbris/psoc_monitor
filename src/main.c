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
extern uint8_t rx_byte;
extern int psoc6_listening_fsm;

extern char user_header_info[100];

// Templates
extern char *main_menu_template;
extern char *monitor_ascii_art[];
extern char *monitor_menu_template;
extern char *debug_menu_template;
extern char *prompt_menu_template;
const char *connected_status_template       =  "Connected";
const char *error_status_template           =  "Error connecting to port!";
const char *exit_prompt_template            =  "Are you sure you want to Exit?";
const char *progrss_spinner_template        = "|/-\\";
//const char *attempting_status_template       =  "Attempting connection...";
const char *lost_connection_status_template  =  "Lost connection...";


/*********************************************************
* Function Definitions
*********************************************************/

void init_terminal()
{
    if(WINDOWS_EN)
    {
        system("MODE con cols=80 lines=22");
        system("if not exist \"temp\" mkdir \"temp\"");
        system("if not exist \"log\" mkdir \"log\"");
    }
    else
    {
        set_nonblock(1);
        hide_cursor(1);

        //mkdir -p
        //system("mkdir -p temp");
        //system("mkdir -p log");
        //resizeterm(22,80);
        //system("echo -ne '\e[8;22;80t'");
    }
}
int main(){
    char input_layer[TERM_N_ROW*TERM_N_COL] = {0};
    init_terminal();
    //char *arrow = "->";

    //BUFFER_DATA bufferPSoc;
    //SERIAL_PORT mainDevice, monitorDevice;
    //
    //

    //SERIAL_PORT_Init(&mainDevice, main_device_type);
    //SERIAL_PORT_Init(&monitorDevice, monitor_device_type);

    //memset(bufferPSoc.data, 0, sizeof(bufferPSoc.data));
    //bufferPSoc.dataIndex =
    //bufferPSoc.SRfaultIndex =
    //bufferPSoc.WRfaultIndex = 0;
    //

    //showCursor(FALSE);
    //
    //mainMenu(&bufferPSoc, &mainDevice, &monitorDevice);

    //if(mainDevice.active) close_serial_port(mainDevice.ptr);
    //if(monitorDevice.active) close_serial_port(monitorDevice.ptr);
    //showCursor(TRUE);



    enum menu_st {
            FSM_IDLE_ST,
            FSM_GET_PSOC_PORT_ST,
            FSM_GET_MONITOR_DEVICE_PORT_ST,
            FSM_GET_LOG_INFO_ST
        };

    serial_port_t psoc_port, monitor_port;
    int fsm_st, str_len;
    int menu_option = 0, temp = 0;
    char user_input[100] = {0};
    char *background    = NULL,
         *status_prompt = NULL;

    // Manage Main Menu
    while(menu_option != MENU_N_OPTIONS - 1)
    {
        switch (fsm_st)
        {
            // Listen to menu option
            case FSM_IDLE_ST:
                menu_option = get_menu_option(input_layer);

                if (menu_option == 0)
                {
                    background = prompt_menu_template;

                    // Reset variables
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
                    fsm_st = FSM_IDLE_ST;
                // Verify serial port
                else if (str_len > 0)
                {
                    memcpy(psoc_port.name, user_input, str_len);
                    psoc_port.name_len = str_len;

                    // Attempt connection
                    psoc_port.device = open_serial_port(psoc_port.name, psoc_port.name_len, 115200);
                    if (psoc_port.device > 0)
                    {
                        // Verify device
                        // TODO: listen to port and change active status
                        status_prompt = NULL;
                        fsm_st = FSM_GET_MONITOR_DEVICE_PORT_ST;

                    }
					else
					{
                        status_prompt = (char *) error_status_template;

					}

                }

                break;

            // Connect Monitoring Device Serial Port
            case FSM_GET_MONITOR_DEVICE_PORT_ST:
                str_len = get_keyboard_str(input_layer + MONITOR_PROMPT_OFFSET, user_input, 20);

                // Print selected port
                memcpy(input_layer + PSOC_PROMPT_OFFSET, psoc_port.name, psoc_port.name_len);

                // TODO: check active status
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
                    close(psoc_port.device);

                    fsm_st = FSM_IDLE_ST;
                }
                // Verify port
                else if (str_len > 0)
                {
                    memcpy(monitor_port.name, user_input, str_len);
                    monitor_port.name_len = str_len;

                    // Attempt connection
                    monitor_port.device = open_serial_port(monitor_port.name, monitor_port.name_len, 115200);
                    if (monitor_port.device > 0)
                    {
                        // TODO: flush serial port?
                        // Verify device
                        // TODO: listen to port and change active status
                        memset(user_input, '\0', sizeof(user_input));
                        fsm_st = FSM_GET_LOG_INFO_ST;

                    }
					else
					{
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
                    close(psoc_port.device);
                    close(monitor_port.device);

                    fsm_st = FSM_IDLE_ST;
                }
                // Creates file with timestamp and begin monitoring
                else if (str_len >= 0)
                {
                    memcpy(user_header_info, user_input, strlen(user_input));
                    // Copy port info and check port
                    // Monitoring management
                    manage_monitor_menu(&psoc_port, &monitor_port);
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
			msleep(10);
		}
    }
    hide_cursor(0);
    set_nonblock(0);

    //#ifdef _WIN32
    //    system("MODE con cols=120 lines=30");
    //    system("if exist \"temp\" rmdir /S /Q temp");
    //#else
    //    //resizeterm(24,80);
    //    //system("echo -ne '\e[8;24;80t'");
    //    //system("rm -fr temp");
    //    //endwin();
    //#endif
    //clrscr();

    return 0;
}

int manage_monitor_menu(serial_port_t *psoc_port, serial_port_t *monitor_port)
{
    log_info_t monitoring_info;
    enum monitor_menu_st {
            FSM_IDLE_ST,
            FSM_TOGGLE_DEBUG_ST,
            FSM_INJECT_FAULT_ST,
            FSM_EXIT_PROMT_ST
        };

    char input_layer[TERM_N_ROW*TERM_N_COL] = {0};
    char user_input = 0;
    char *status_prompt = NULL;

    char info_buffer[100] = {0};

    double psoc_timer,
           monitor_timer;
    clock_t spinner_timer = 0;

    int spinner_cnt = 0, temp;
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

        // TODO: create listen functions
        listen_psoc(psoc_port, &monitoring_info);

        // Verify PSoC UART timeout
        psoc_timer = time_diff(psoc_port->timeout_cnt);
        if (psoc_timer > PSOC6_CONNECTION_TIMEOUT*1000)
        {
            temp = strlen(lost_connection_status_template);
            memcpy(input_layer + TERM_N_COL*6 + 52, lost_connection_status_template, temp);
            psoc_port->status = 0;

            // Restart DUT device
            dut_rst(monitor_port);
            monitoring_info.session.con_rst_cnt += 1;
        }

        // Verify Monitoring Device UART timeout
        monitor_timer = time_diff(monitor_port->timeout_cnt);
        if (monitor_timer > MONITOR_CONNECTION_TIMEOUT*1000)
        {
            temp = strlen(lost_connection_status_template);
            memcpy(input_layer + TERM_N_COL*7 + 54, lost_connection_status_template, temp);
            monitor_port->status = 0;
            monitoring_info.session.con_lost_monitor += 1;
        }

        if(kbhit())
        {
            // read last character in case ANSI escape sequences
            while(read(STDIN_FILENO, &user_input, 1) > 0){}

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

                    // Single bit flip
                    if (user_input == 's')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Es", 2);
                    }
                    // Time hold
                    else if (user_input == 't')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Et", 2);
                    }
                    // Stuck in a sample
                    else if (user_input == 'a')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Ea", 2);
                    }
                    // Core hang in a infinite loop
                    else if (user_input == 'h')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Eh", 2);
                    }
                    // Redundant buffer mismatch
                    else if (user_input == 'b')
                    {
                        write_port(psoc_port->device, (uint8_t *)"Eb", 2);
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



        sprintf(info_buffer, "Last char: %d, FSM sate: %d, pckt_num: %u", rx_byte, psoc6_listening_fsm, monitoring_info.session.packet_num);
        temp = strlen(info_buffer);
        memcpy(input_layer + 2*TERM_N_COL - 2 - temp, info_buffer, temp);

        // Debug menu enabled
        if (debug_flag)
        {
            update_screen(debug_menu_template, input_layer, NULL);

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

    return 0;
}


//**************************************************************************************


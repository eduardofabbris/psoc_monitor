#include "include/util.h"
#include "include/menu.h"
#include "include/serial.h"
//#include "include/serial.h"
//#include "include/fileManagement.h"

// buffer_info struct -> add CRC
// File descriptor
// file_name
// file_cnt
// irst_cnt     // internal reset counter
// erst_cnt     // external reset counter
// buffer_cnt   // number of buffers in file

//=========== GLOBAL VARIABLES ===========

extern char *main_menu_template;
extern char *prompt_menu_template;
const char *connected_status_template        =  "Connected";
const char *attempting_status_template       =  "Attempting connection...";
const char *error_status_template            =  "Error connecting to port!";
const char *lost_connection_status_template  =  "Lost connection...";

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
    char *background = NULL,
         *status_prompt = NULL;

    // Manage Main Menu
    while(menu_option != MENU_N_OPTIONS - 1) {
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

                // Print status
                temp = strlen(connected_status_template);
                memcpy(input_layer + TERM_N_COL*(PSOC_PROMPT_ROW_OFFSET + 1) - 5 - temp, connected_status_template, temp);

                if (status_prompt != NULL)
                {
                    temp = strlen(status_prompt);
                    memcpy(input_layer + TERM_N_COL*(MONITOR_PROMPT_ROW_OFFSET+1) - 5 - temp, status_prompt, temp);
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
                        // Verify device
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
                    // Copy port info and check port

                }

                break;
            default:
                fsm_st = FSM_IDLE_ST;
                break;

        }

		if (background != NULL)
		{
			update_screen(background, input_layer);
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



//**************************************************************************************


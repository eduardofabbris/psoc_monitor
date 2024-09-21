#include "include/util.h"
#include "include/menu.h"
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
    char input_layer[TERM_N_ROW*TERM_N_COL] = {};
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

    int fsm_st, str_status;
    int menu_option = 0;
    char user_input[100] = {0};
    char *background;

    // manage main menu
    while(menu_option != MENU_N_OPTIONS - 1) {
        switch (fsm_st)
        {
            case FSM_IDLE_ST:
                menu_option = get_menu_option(input_layer);

                if (menu_option == 0)
                {
                    background = prompt_menu_template;
                    fsm_st = FSM_GET_PSOC_PORT_ST;
                }
                else
                {
                    background = main_menu_template;
                }

                break;

            // Connect PSoC 6 Serial port
            case FSM_GET_PSOC_PORT_ST:
                str_status = get_keyboard_str(input_layer + TERM_N_COL*11 + 18, user_input, 20);

                // Cancel
                if (str_status == 0)
                    fsm_st = FSM_IDLE_ST;
                // Verify serial port
                else if (str_status == 1)
                {
                    fsm_st = FSM_GET_MONITOR_DEVICE_PORT_ST;

                }

                // Monitoring Device Serial Port


                break;

            // Connect Monitoring Device Serial Port
            case FSM_GET_MONITOR_DEVICE_PORT_ST:
                str_status = get_keyboard_str(input_layer + TERM_N_COL*12 + 29, user_input, 20);

                // Cancel
                if (str_status == 0)
                {
                    // TODO: close serial port open
                    fsm_st = FSM_IDLE_ST;
                }
                // Verify port
                else if (str_status == 1)
                {
                    // Copy port info and check port
                    fsm_st = FSM_GET_LOG_INFO_ST;

                }

                break;

            // Get addition info to append to log file
            case FSM_GET_LOG_INFO_ST:
                str_status = get_keyboard_str(input_layer + TERM_N_COL*13 + 26, user_input, 50);

                // Cancel
                if (str_status == 0)
                {
                    // TODO: close both serial ports open
                    fsm_st = FSM_IDLE_ST;
                }
                // Creates file with timestamp and begin monitoring
                else if (str_status == 1)
                {
                    // Copy port info and check port

                }

                break;
            default:
                fsm_st = FSM_IDLE_ST;

        }

        update_screen(background, input_layer);
        msleep(10);
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


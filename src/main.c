#include "include/util.h"
#include "include/menu.h"
//#include "include/serial.h"
//#include "include/fileManagement.h"


//=========== GLOBAL VARIABLES ===========

extern const char *main_menu_template;

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

    int menu_option = 0;
    char *arrow_ptr = input_layer + TERM_N_COL*ARROW_ROW_OFFSET + ARROW_COL_OFFSET;
    memcpy(arrow_ptr, "->", 2);
   

    int ch = 0;
    while(ch != ESC && ch != 'q') {
        if(kbhit()){
            // read last character in case ANSI escape sequences
            while(read(STDIN_FILENO, &ch, 1) > 0);

            //gotoxy(1,1), printf("Read character: %c", ch);
            switch(ch)
            {
                case 'w': case 'k': case UP:
                    if (menu_option > 0 ) menu_option -= 1;
                    else menu_option = MENU_N_OPTIONS - 1;

                    break;
                case 's': case 'j': case DOWN:
                    if (menu_option < MENU_N_OPTIONS - 1) menu_option += 1;
                    else menu_option = 0;
                    break;
                case ENTER:
                    gotoxy(2,1), printf("Option %d", menu_option);
                    break;
                default:
                    break;
            }
            arrow_ptr = input_layer + TERM_N_COL*(ARROW_ROW_OFFSET + menu_option) + ARROW_COL_OFFSET;

        }
        //system("clear");
        //clrscr();
        memcpy(arrow_ptr, "->", 2);
        update_screen(main_menu_template, input_layer);
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


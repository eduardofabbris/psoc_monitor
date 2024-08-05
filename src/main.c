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
    while(menu_option != MENU_N_OPTIONS - 1) {
        menu_option = get_menu_option(input_layer);
        //system("clear");
        //clrscr();
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


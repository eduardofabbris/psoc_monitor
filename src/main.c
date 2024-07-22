#include "include/util.h"
//#include "include/serial.h"
//#include "include/menu.h"
//#include "include/fileManagement.h"


//=========== GLOBAL VARIABLES ===========


int main(){

    #ifdef _WIN32
        system("MODE con cols=80 lines=22");
        system("if not exist \"temp\" mkdir \"temp\"");
        system("if not exist \"log\" mkdir \"log\"");
    #else
        initscr();
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        //scrollok(stdscr, TRUE);

        //mkdir -p
        system("mkdir -p temp");
        system("mkdir -p log");
        //resizeterm(22,80);
        system("echo -ne '\e[8;22;80t'");
    #endif
    BUFFER_DATA bufferPSoc;    
    SERIAL_PORT mainDevice, monitorDevice;
    
    

    SERIAL_PORT_Init(&mainDevice, main_device_type);
    SERIAL_PORT_Init(&monitorDevice, monitor_device_type);

    memset(bufferPSoc.data, 0, sizeof(bufferPSoc.data));
    bufferPSoc.dataIndex = 
    bufferPSoc.SRfaultIndex = 
    bufferPSoc.WRfaultIndex = 0;
    

    showCursor(FALSE);
    
    mainMenu(&bufferPSoc, &mainDevice, &monitorDevice);

    if(mainDevice.active) close_serial_port(mainDevice.ptr);
    if(monitorDevice.active) close_serial_port(monitorDevice.ptr);
    showCursor(TRUE);
    #ifdef _WIN32
        system("MODE con cols=120 lines=30");
        system("if exist \"temp\" rmdir /S /Q temp");
    #else
        //resizeterm(24,80);
        system("echo -ne '\e[8;24;80t'");
        system("rm -fr temp");
        endwin();
    #endif
    clrscr();
    
    return 0;
}



//**************************************************************************************


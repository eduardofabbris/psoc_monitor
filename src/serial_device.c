#include "include/serial_device.h"

void SERIAL_PORT_Init(SERIAL_PORT *self, device_t device_type)
{
    self->ptr = _NULL;
    self->active = FALSE;
    self->device_type = device_type;
    self->verifying_connection_flag = FALSE;
    self->verify_connection_tmr = 0;
    memset(self->port_name, '\0', sizeof(self->port_name));
}
//****************************************************************************************


// return 1 if successful, also verify if port is still open and flushes buffer
BOOL connectSerialPort(SERIAL_PORT *device, BOOL flushData)
{
    char deviceName[MAX_LOADSTRING] = {0};
    static uint32_t baudRate = BAUD_RATE;

    if(! *device->port_name)
        return 0;

    if(WINDOWS_OS)
        sprintf(deviceName, "\\\\.\\%s", device->port_name);
    else
        sprintf(deviceName, "/dev/tty%s", device->port_name);
    

    HANDLE ptr = open_serial_port(deviceName, baudRate, flushData);

    if (ptr == INVALID_HANDLE_VALUE)
    {
#ifdef _WIN32
        int error_code = GetLastError();
#else
        int error_code = errno;
        // gotoxy(1,0);
        // printw("%s", strerror(error_code));
#endif
        // gotoxy(0,0);
        // printw("Error code: %i\n", error_code);

        refresh();

        if (error_code == ERROR_CODE_CANNOT_FIND_DEVICE ||
            error_code == ERROR_CODE_DOES_NOT_RECOGNIZE_DEVICE)
        {
            if (device->active)
                close_serial_port(device->ptr);
            device->active = FALSE;
        }
        else if (error_code == ERROR_CODE_ACCESS_DENIED)
        {
            if (flushData)
            {
#ifdef _WIN32
                close_serial_port(device->ptr);
                device->ptr = open_serial_port(deviceName, baudRate, flushData);
#else
                tcflush(device->ptr, TCIOFLUSH);
#endif
            }
        }
    }
    else
    {
        device->ptr = ptr;
        device->active = TRUE;
        return 1;
    }

    return 0;
}
//****************************************************************************************

// return 1 if success, 0 if not success, -1 cancel
int chooseSerialPort(SERIAL_PORT *device, int x, int y)
{
    SERIAL_PORT aux;
    char key = 0, 
         read_buffer[10] = {0};
    
    uint8_t j = 0,
            my_y = y;
            //port_number = 0;

    aux.ptr = _NULL;
    aux.active = FALSE;
    //aux.portNumber = 0;

    if(WINDOWS_OS)
    {
        printPrompt("COM", x, my_y);
        my_y += 3;
    }

    showCursor(TRUE);
    clrbuf();
    // scanf(" %d", &aux.portNumber);
    while (key != ENTER && key != ESC)
    {
        Sleep(15);
        if (kbhit())
        {
            key = getch();
            // gotoxy(0,0);
            // printw("%i   ", key);
            // refresh();
            if (  (key >= '0' && key <= '9') || 
                ( (key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') & !WINDOWS_OS))
            {
                if (j < 6)
                {
                    gotoxy(x, my_y + j);
                    _printf("%c", key);
                    read_buffer[j] = key;

                    j++;
                    gotoxy(x, my_y + j); // advance cursor
                }
            }
            else if (key == BACKSPACE && j > 0)
            {

                read_buffer[j] = '\0';
                j--;
                gotoxy(x, my_y + j);
                _printf(" ");

                gotoxy(x, my_y + j); // return cursor to starting pos
            }
        }
    }
    showCursor(FALSE);

    if (key == ESC)
    {
        return -1;
    }
    else
    {
        if(WINDOWS_OS)
        {
            //port_number = atoi(read_buffer);
            sprintf(aux.port_name, "COM%s", read_buffer);
        } 
        else
        {
            strncpy(aux.port_name, read_buffer, sizeof(read_buffer));
        }            

        if (connectSerialPort(&aux, TRUE))
        {
            if (device->active)
            {
                close_serial_port(device->ptr);
            }
            device->active = TRUE;
            strncpy(device->port_name, aux.port_name, sizeof(aux.port_name));
            device->ptr = aux.ptr;

            return 1;
        }
    }

    return 0;
}
//****************************************************************************************

// return 1 if successful, or 0 if not
BOOL readProtocol(SERIAL_PORT *device, char *protocol, double timeout)
{
    double dTime = 0;
    clock_t firstTick;

    uint8_t state = 0;
    uint8_t readByte = 0;

    connectSerialPort(device, TRUE);
    if (device->active)
    {

        firstTick = clock();
        do
        {
            readByte = 0;
            if (read_port(device->ptr, &readByte, 1) > 0)
            {
                // printf("%c\n", readByte);
                switch (state)
                {
                    case 0:
                        if (readByte == protocol[0])
                            state = 1;
                        break;
                    case 1:
                        if (readByte == protocol[1])
                            return 1;
                        else
                            state = 0;
                        break;
                    default:
                        break;
                }
            }

            connectSerialPort(device, FALSE);
            dTime = timeDiff(firstTick);
            // printf("Time-%lf\n", dTime);
        } while ((dTime < timeout) && device->active);
    }

    return 0;
}
//****************************************************************************************

void printSerialStatus(BOOL active, int x, int y)
{
    gotoxy(x, y);
    if (active)
        _printf("Conectada.   ");
    else
        _printf("Desconectada.");
}
//***************************************************************************************

BOOL initPSocDevices(SERIAL_PORT *mainDevice, SERIAL_PORT *monitorDevice)
{
    uint8_t response = 0,
            rowPos = 13,
            colPos = 3;
    response += readProtocol(mainDevice, "MW", 1E3);
    response += readProtocol(monitorDevice, "CW", 1E3) << 1;

    if (!(response & 1) || !(response & 2))
    {
        gotoxy(rowPos, colPos);
        rowPos++;
        if (!(response & 1) && !(response & 2))
            _printf("Nao foi possivel estabelecer conexao com ambos dispositivos!");
        else if (!(response & 1))
            _printf("Nao foi possivel estabelecer conexao com PSoc6 PRINCIPAL!");
        else
            _printf("Nao foi possivel estabelecer conexao com PSoc6 CONTROLE!");

        gotoxy(rowPos, colPos);
        rowPos++;
        _printf("Por favor, reinicie manualmente o(s) dispositivo(s).");

        return 0;
    }
    else
    {
        // printHeader();
        uint8_t buffer[] = {'H', 'S'};

        gotoxy(rowPos, colPos);
        rowPos++;
        _printf("Iniciando PSoc6 PRINCIPAL...");

        write_port(mainDevice->ptr, buffer, sizeof(buffer)); // initialize main device
        // read MS
        if (!readProtocol(mainDevice, "MS", 1E3))
        {
            _printf("   -> Sem resposta de PSoc6 PRINCIPAL!");
        }
        //----------------------------------------------------------------------------------
        gotoxy(rowPos, colPos);
        rowPos++;
        _printf("Iniciando PSoc6 CONTROLE...");

        write_port(monitorDevice->ptr, buffer, sizeof(buffer)); // initialize control device
        // read CS
        if (!readProtocol(monitorDevice, "CS", 1E3))
        {
            _printf("   -> Sem resposta de PSoc6 CONTROLE!");
        }
    }

    return 1;
}
//***************************************************************************************
// windows: https://superuser.com/questions/835848/how-to-view-serial-com-ports-but-not-through-device-manager
BOOL UART_verifyConnection(SERIAL_PORT *device)
{  
    pthread_t thread_id;

    // If device is active, verify each 1000 ms second
    if(timeDiff(device->verify_connection_tmr) <= 1000 || !device->active || device->verifying_connection_flag)    
        return 1;

    device->verifying_connection_flag = TRUE;
    pthread_create(&thread_id, NULL, UART_verifyConnection_thread, device);
    //pthread_exit(NULL);
    //pthread_detach(thread_id);
    //pthread_join(thread_id, NULL); 

    
    return device->active;

}
//****************************************************************************************

void *UART_verifyConnection_thread(void *vargp) 
{ 
   SERIAL_PORT *device = (SERIAL_PORT *)vargp; 
    FILE *ptr = NULL;
    int success = FALSE;

    char buffer[MAX_LOADSTRING] = {0}, 
         file_path[50] = {0};
    
        
    // Update time
    device->verify_connection_tmr = clock();     

    // Verify if port is still connected
    sprintf(file_path, "temp%s%s_status.tmp", FILE_SEPARATOR, device->port_name);
#ifdef _WIN32
    //sprintf(cmp_output, "Device %s is not currently available.", device->port_name);
    sprintf(buffer, "wmic path Win32_SerialPort | find /C \"%s\" > %s", device->port_name, file_path);    
#else
    //sprintf(cmp_output, "/dev/tty%s", device->port_name);
    sprintf(buffer, "find /dev/tty* | grep -c %s > %s", device->port_name, file_path);    
#endif

    system(buffer);
    ptr = fopen(file_path, "r");
    if(ptr == NULL)
        return 0;

    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, sizeof(buffer), ptr);

    //success = strncmp(buffer, cmp_output, strlen(cmp_output)) == 0;
    success = atoi(buffer) > 0;
    fclose(ptr);

    device->active = success; 

    device->verifying_connection_flag = FALSE;
    return NULL;  

} 


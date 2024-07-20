#include "include/fileManagement.h"

//extern BUFFER_DATA bufferPSoc;
extern BUFFER_DATA buffer_copy;
// =================== GLOBAL VARIABLES ===================
uint16_t BUFFERES_IN_FILE_OFFSET = 0,
         ALIVE_SIGNAL_RESTART_IN_FILE_OFFSET = 0,
         ALIVE_SIGNAL_SERIAL_RESTART_IN_FILE_OFFSET = 0;

void FILE_INFO_Init(FILE_INFO *self, const char *file_name)
{
    self->alv_sig_rst_n = 0;
    self->alv_serial_rst_n = 0;
    self->buffer_n = 0;
    self->file_n = 0;

    memset(self->file_name, '\0', sizeof(self->file_name));
    if(file_name != NULL)
        strncpy(self->file_name, file_name, strlen(file_name));
        
}
//**************************************************************************


void MONITORING_INFO_Init(MONITORING_INFO *self)
{
    self->alv_serial_flag = FALSE;
    self->r_buffer_flag = FALSE;
    self->alv_sig_rst_n = 0;
    self->alv_serial_rst_n = 0;
    self->buffer_n = 0;
}
//**************************************************************************

uint32_t writeFileHeader(char* fileName) 
{
    FILE *ptr = NULL;
    char buffer[MAX_BUFFER_SIZE] = {0};

    uint32_t aux = 0;

    if(fileName != NULL)
    {
        sprintf(buffer, "log%s%s.txt", FILE_SEPARATOR, fileName);
    }
    else
    {
        sprintf(buffer, "temp%sheader.txt", FILE_SEPARATOR);
    }



    ptr = fopen(buffer, "w");

    
    fprintf(ptr, "# INFO FORMAT: dTime(us) | Read | Written | SlewRate Fault | WRdiff Fault \n");

    fprintf(ptr, "# File buffers: ");
    BUFFERES_IN_FILE_OFFSET = ftell(ptr);
    fprintf(ptr, "           \n");
    
    fprintf(ptr, "# File alive signal restart: ");
    ALIVE_SIGNAL_RESTART_IN_FILE_OFFSET = ftell(ptr);
    fprintf(ptr, "           \n");

    fprintf(ptr, "# File alive signal serial restart: ");
    ALIVE_SIGNAL_SERIAL_RESTART_IN_FILE_OFFSET = ftell(ptr);
    fprintf(ptr, "           \n");

    
    fprintf(ptr, "\n");
    aux = ftell(ptr); // return the number os bytes written


    fclose(ptr);
    

    return aux;

}
//**************************************************************************

//return the number of buffers or 0 if the file doesent exit
int32_t getHeaderData(char* fileName, uint16_t posInFile) {
    int32_t aux = 0;
    char buffer[MAX_BUFFER_SIZE] = {0};

    sprintf(buffer, "log%s%s.txt", FILE_SEPARATOR, fileName);

    FILE *ptr = fopen(buffer, "r");
    if(ptr == NULL)
        return 0;
    //else
    memset(buffer, 0, sizeof(buffer));

    fseek(ptr, posInFile, SEEK_SET);    
    fgets(buffer, 10, ptr);
    
    aux += atoi(buffer);
    //gotoxy(0,0);
    //_printf("AUX = %s",buffer);
    //getchar();

    fclose(ptr);

    return aux;
}
//**************************************************************************

void updateFileHeader(FILE_INFO *file_info)
{
    FILE *ptr = NULL;
    char buffer[MAX_BUFFER_SIZE] = {0};

    if(file_info->file_n > 0)
        sprintf(buffer, "log%s%s_%u.txt", FILE_SEPARATOR, file_info->file_name, file_info->file_n);
    else
        sprintf(buffer, "log%s%s.txt", FILE_SEPARATOR, file_info->file_name);   



    // Change header info
    ptr = fopen(buffer, "r+");
    if(ptr != NULL)
    {
        fseek(ptr, BUFFERES_IN_FILE_OFFSET, SEEK_SET);
        fprintf(ptr, "%u", file_info->buffer_n);

        fseek(ptr, ALIVE_SIGNAL_RESTART_IN_FILE_OFFSET, SEEK_SET);
        fprintf(ptr, "%u", file_info->alv_sig_rst_n);

        fseek(ptr, ALIVE_SIGNAL_SERIAL_RESTART_IN_FILE_OFFSET, SEEK_SET);
        fprintf(ptr, "%u", file_info->alv_serial_rst_n);

        fclose(ptr);        
    }


}
//**************************************************************************

void writeFileNewBuffer(BUFFER_DATA deviceBuffer, FILE_INFO file_info)
{
    FILE *ptr;
    //uint32_t timeSum = 0;
    char buffer[MAX_BUFFER_SIZE] = {0};

    if(file_info.file_n > 0)
        sprintf(buffer, "log%s%s_%u.txt", FILE_SEPARATOR, file_info.file_name, file_info.file_n);
    else
        sprintf(buffer, "log%s%s.txt", FILE_SEPARATOR, file_info.file_name);        
        

    ptr = fopen(buffer, "a");
    if(ptr != NULL)
    {
        const char *dateAndTime = getCurrentTimeAndDate();
        fprintf(ptr, "\n# Buffer: %u - %s", file_info.buffer_n, dateAndTime);
        fprintf(ptr, "# Faults: SlewRate -> %d, WRdiff -> %d\n", deviceBuffer.SRfaultIndex, deviceBuffer.WRfaultIndex);
        for(int i=0; i < MAX_BUFFER_DATA; i++){
            char srFault = ' ';
            char wrFault = ' ';
            if(deviceBuffer.data[i].SR_fault) srFault = 'X';
            if(deviceBuffer.data[i].WR_fault) wrFault = 'X';

            fprintf(ptr, "%-6d\t%-6d\t%-6d\t%-6c\t%-6c\n", 
                deviceBuffer.data[i].dtime,
                deviceBuffer.data[i].read,            
                deviceBuffer.data[i].written,
                srFault,
                wrFault
            );

            //timeSum += deviceBuffer.data[i].dtime;

        }
        fclose(ptr);
    }


    //updateFileHeader(deviceBuffer, file_info);

}
//**************************************************************************

void 
monitorMainDevice(BUFFER_DATA* bufferPSoc, SERIAL_PORT mainDevice, MONITORING_INFO* monitoring_info, FILE_INFO *file_info)
{
    static uint8_t readData[10];
    static uint8_t state = 0;
    static uint8_t dataCount = 0;
    uint8_t readByte = 0;

    if( read_port(mainDevice.ptr, &readByte, 1) > 0 ){
        switch (state) {
            case 0:
                if(readByte == 'M') //main device
                    state = 1;
                break;
            case 1:
                if(readByte == 'D')//device has data to trasmit
                    state = 2;
                else if(readByte == 'A')//device alive signal
                    state = 3;
                else if(readByte == 'W')//device is wating to start
                    state = 4;
                else
                    state = 0;
                break;
            case 2:
                monitoring_info->r_buffer_flag = TRUE;
                readData[dataCount] = readByte;
                dataCount++;
                
                if(dataCount >= SERIAL_BUFFER_SIZE){
                    dataCount = 0;
                    
                    storeDeviceBuffer(bufferPSoc, readData);
                    bufferPSoc->dataIndex++;                    
                    if(bufferPSoc->dataIndex >= MAX_BUFFER_DATA)
                    {
                        //gotoxy(0,0), _printf("norst - %04d", bufferPSoc->dataIndex);
                        addNewBufferInFile(bufferPSoc, monitoring_info, file_info);
                    }
                        

                    state = 0;
                }                                                    
                break;
            case 3:
                monitoring_info->alv_serial_flag =  TRUE;
                state = 0;
                break;
            case 4: //starting main device  
                
                /*if(monitoring_info->r_buffer_flag)
                {
                    gotoxy(0,0), _printf("Buffer flag on");
                    break;
                }*/
                if(bufferPSoc->dataIndex > 0) { //error serial with last buffer **?
                    //gotoxy(0,0), _printf("restart - %04d", bufferPSoc->dataIndex);
                    if(bufferPSoc->SRfaultIndex || bufferPSoc->WRfaultIndex)
                    {
                        bufferPSoc->data[MAX_BUFFER_DATA-1].dtime = 
                        bufferPSoc->data[MAX_BUFFER_DATA-1].read = 
                        bufferPSoc->data[MAX_BUFFER_DATA-1].written = 0;
                        addNewBufferInFile(bufferPSoc, monitoring_info, file_info);
                    }
                    else{
                        //gotoxy(0,0), _printf("Error, faul==NULL");
                        memcpy(&buffer_copy.data, bufferPSoc->data, sizeof(bufferPSoc->data));
                        memset(bufferPSoc->data, 0, sizeof(bufferPSoc->data)); 
                    }

                }
                write_port(mainDevice.ptr, (uint8_t *)"HS", 2);//start command
                state = 0;
                break;

            default:
                break;
        }
    }                    

}
//**************************************************************************

void storeDeviceBuffer(BUFFER_DATA* bufferPSoc, uint8_t *readData){
    uint16_t i = bufferPSoc->dataIndex;

    bufferPSoc->data[i].read      = readData[0] + (readData[1] << 8);
    bufferPSoc->data[i].dtime     = readData[2] + (readData[3] << 8);
    bufferPSoc->data[i].written   = readData[4] + (readData[5] << 8);
    bufferPSoc->data[i].WR_fault  = ~readData[6] & 1 ;
    bufferPSoc->data[i].SR_fault  = (~readData[6] >> 1) & 1;

    if(bufferPSoc->data[i].WR_fault) bufferPSoc->WRfaultIndex++;
    if(bufferPSoc->data[i].SR_fault) bufferPSoc->SRfaultIndex++;

    /*printf(
        "%d-\t%d,\t%d,\t%d,\t%d,%d",
        i, 
        bufferPSoc->data[i].read,                                            
        bufferPSoc->data[i].dtime,
        bufferPSoc->data[i].written,
        bufferPSoc->data[i].WR_fault,
        bufferPSoc->data[i].SR_fault
    );
    if(~readData[6] & 0xFF) printf(" <- Erro");
    printf("\n");*/
}
//**************************************************************************

BOOL 
monitorControlDevice(SERIAL_PORT device, MONITORING_INFO* monitoring_info, FILE_INFO *file_info)
{
    static uint8_t state = 0;
    uint8_t readByte = 0;

    if( read_port(device.ptr, &readByte, 1) > 0 ){
        switch (state) {
            case 0:
                if(readByte == 'C')
                    state = 1;
                break;
            
            case 1:
                state = 0;
                if(readByte == 'R'){
                    monitoring_info->alv_sig_rst_n++;
                    file_info->alv_sig_rst_n++;
                    return 1;
                }
                break;

            default:
                break;
        }
    }

    return 0;
}
//**************************************************************************

//before writing file header or append another buffer
//verify if there is already a buffer file and the quantity of buffers
//if it's greater than a value, write in new file (file_name_n.txt)
void manageTxtFile(FILE_INFO *file_info) 
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    
    writeFileHeader(NULL); // get positions
    uint32_t aux = getHeaderData(file_info->file_name, BUFFERES_IN_FILE_OFFSET);
    if(aux > 0){ //the file already exists
        if(aux < MAX_BUFFER_INDEX_IN_FILE){
            file_info->buffer_n = aux;	
            file_info->alv_sig_rst_n = getHeaderData(file_info->file_name, ALIVE_SIGNAL_RESTART_IN_FILE_OFFSET);
            file_info->alv_serial_rst_n = getHeaderData(file_info->file_name, ALIVE_SIGNAL_SERIAL_RESTART_IN_FILE_OFFSET);
                            
        }
        else {
            do {
                memset(buffer, 0, sizeof(buffer));
                
                file_info->file_n++;
                sprintf(buffer, "%s_%u", file_info->file_name, file_info->file_n);
                aux = getHeaderData(buffer, BUFFERES_IN_FILE_OFFSET);

            } while(aux >= MAX_BUFFER_INDEX_IN_FILE);

            if(aux == 0){
                writeFileHeader(buffer);
                //runInfo->bufferIndexInFile = 
                //runInfo->aliveSigRestartIndexInFile =
                //runInfo->aliveSigSerialRestartIndexInFile = 0;
            }
             //if there is already a file although with indexbuffer < maxindexbuffer   
            else{
                
                file_info->buffer_n = aux;
                file_info->alv_sig_rst_n = getHeaderData(buffer, ALIVE_SIGNAL_RESTART_IN_FILE_OFFSET);
                file_info->alv_serial_rst_n = getHeaderData(buffer, ALIVE_SIGNAL_SERIAL_RESTART_IN_FILE_OFFSET);
            }

        } 
    }
    else{
        writeFileHeader(file_info->file_name);
    }
}
//**************************************************************************

void 
addNewBufferInFile(BUFFER_DATA* bufferPSoc, MONITORING_INFO *monitoring_info, FILE_INFO *file_info) 
{
    static uint32_t last_alv_sig_n = 0,
                    last_alv_serial_n = 0;
    
    monitoring_info->r_buffer_flag = FALSE;
    monitoring_info->buffer_n++;
    file_info->buffer_n++;

    
    if(file_info->buffer_n > MAX_BUFFER_INDEX_IN_FILE){
        manageTxtFile(file_info);

        file_info->buffer_n = 1;
        file_info->alv_sig_rst_n = file_info->alv_sig_rst_n - last_alv_sig_n;
        file_info->alv_serial_rst_n = file_info->alv_serial_rst_n - last_alv_serial_n;  
    }
    //write data in txt file
    writeFileNewBuffer(*bufferPSoc, *file_info);

    bufferPSoc->dataIndex = 0;
    bufferPSoc->SRfaultIndex = 0;
    bufferPSoc->WRfaultIndex = 0; 
    
    memcpy(&buffer_copy.data, bufferPSoc->data, sizeof(bufferPSoc->data));
    memset(bufferPSoc->data, 0, sizeof(bufferPSoc->data));

    updateFileHeader(file_info);
    
    last_alv_sig_n = file_info->alv_sig_rst_n;
    last_alv_serial_n = file_info->alv_serial_rst_n;
}

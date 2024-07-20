#include "include/graph.h"

BUFFER_DATA buffer_copy;

BOOL gnuPlotGraph() {
    FILE *gnupipe = NULL;
    FILE *ptr[3] = {NULL};
    BOOL success = TRUE;

    const char *command[] =
    {
        "set style line 1 lc rgb '#0000FF' lw 1.5 lt 7 ps 0.5",
        "set style line 2 lc rgb '#006400' lw 1.5 lt 7 ps 0.5",
        "set style line 3 lc rgb '#FF0000' lw 1.5 lt 7 ps 0.8",
        "set tics nomirror scale 1.5",
        "set xlabel 'Samples'",
        "set grid",
        "set ylabel 'Amplitude'",
        "set key box opaque",
        "cd 'temp'",
        "plot 'systOutput.tmp' ls 2 title 'System Output', 'adcOutput.tmp' ls 1 title 'ADC' with lp, 'fault.tmp' ls 3 title 'Fault'",
        "\0"
    };

    #ifdef _WIN32
    
        const char *fileNames[] =
        {
            "temp\\\\systOutput.tmp",
            "temp\\\\adcOutput.tmp",
            "temp\\\\fault.tmp"
        };
    #else
        const char *fileNames[] =
        {
            "temp/systOutput.tmp",
            "temp/adcOutput.tmp",
            "temp/fault.tmp"
        };

    #endif
    
    for(int i=0; i < 3; i++){
        ptr[i] = fopen(fileNames[i], "w");    
    }

    for(int i=0; i < MAX_BUFFER_DATA; i++){
        fprintf(ptr[0], "%d, %d\n", i, buffer_copy.data[i].written);
        fprintf(ptr[1], "%d, %d\n", i, buffer_copy.data[i].read);
        

        if(buffer_copy.data[i].SR_fault || buffer_copy.data[i].WR_fault){
            if(!buffer_copy.data[i].WR_fault){
                if((i+1) < 1000 ) fprintf(ptr[2], "%d, %d\n", i+1, buffer_copy.data[i+1].read);
            }
            else{
                fprintf(ptr[2], "%d, %d\n", i, buffer_copy.data[i].read);  
            }
              
        }
        
    }
    for(int i=0; i < 3; i++){
        fclose(ptr[i]);
    }
    gnupipe = _popen("gnuplot -persistent", "w"); 

    if(gnupipe != NULL){
        for(int i=0;  *command[i] != 0; i++){
            //printf("%d,", i);
            fprintf(gnupipe, "%s\n", command[i]);
        }


        fflush(gnupipe);
        _pclose(gnupipe);       
    }
    else{        
        success = FALSE;
    }


    //remove files
    /*for(int i=0; i < 3; i++){
        remove(fileNames[i]);
    }*/

    return success;
}
//***************************************************************************************
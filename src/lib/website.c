#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "website.h"

#define MAX_LINE_LEN 50
#define NUM_COLUMNS 3

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Roll a dice between 0 and 100 */
    int roll = rand() % 100;

    /* Loop over websites */
    int curr_range = 0;
    for (unsigned int i = 0; i < num_websites; i++) {
        if (roll >= curr_range && roll <= websites[i].influence + curr_range
            && websites[i].id != previous_website_id) {
            return &websites[i];
        }   
        else {
            curr_range += websites[i].influence;
        }
    }

    return NULL;
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    char *file_addr = "input/websites_data.csv";
    FILE *file_pointer = fopen(file_addr, "r");
    char *line_buffer;
    int i, j;
    
    /* Opens the input file to read it */
    
    
    if (file_pointer == NULL) {
        printf("ERROR: Can't open file\n");
        exit(1);
    }

    /* Reads text until newline is encountered */
    for (i = -1; i < sim_input->num_websites; i++) {
        fscanf(file_pointer, " %s", line_buffer);
        
        if (i != -1) {
            websites[i].id = i;
            sscanf(line_buffer, " %hu,%f,%f", websites[i].avg_duration, websites[i].pages_per_visit, websites[i].influence);        
            /* ERROR WITH SSCANF IN GDB!!!
            Reading symbols from c:\Users\jonas\OneDrive - Aalborg Universitet\1. semester\P1-Sustainable-Internet\a.exe...done.
            (gdb) run
            Starting program: c:\Users\jonas\OneDrive - Aalborg Universitet\1. semester\P1-Sustainable-Internet/a.exe
            [New Thread 19528.0x3c18]
            [New Thread 19528.0x4b8]

            Program received signal SIGSEGV, Segmentation fault.
            0x76ab17da in ungetwc () from C:\WINDOWS\SysWOW64\msvcrt.dll
            (gdb) bt
            #0  0x76ab17da in ungetwc () from C:\WINDOWS\SysWOW64\msvcrt.dll
            #1  0x00000001 in ?? ()
            #2  0x00000000 in ?? ()*/
        }
    }
        
    fclose(file_pointer);
}
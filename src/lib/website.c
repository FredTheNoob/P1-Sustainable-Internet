#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "website.h"

#define MAX_LINE_LEN 100
#define NUM_COLUMNS 3

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Roll a dice between 0 and 100 */
    int roll = rand() % 100;

    /* Loop over websites */
    int curr_range = 0;
    for (int i = 0; i < num_websites; i++) {
        if (roll >= curr_range && roll <= websites[i].influence + curr_range
            && websites[i].id != previous_website_id) {
            return &websites[i];
        }   
        else {
            curr_range += websites[i].influence;
        }
    }
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    char *file_addr = "input/websites_data.csv";
    FILE *file_pointer = fopen(file_addr, "r");
    char line_buffer[MAX_LINE_LEN];
    char *tokens = NULL;
    int i = -1, j;
    
    /* Opens the input file to read it */
    
    
    if (file_pointer == NULL) {
        printf("ERROR: Can't open file\n");
        exit(1);
    }

    /* Reads text until newline is encountered */
    while (fgets(line_buffer, MAX_LINE_LEN, file_pointer) && i < sim_input->num_websites) {
        if (i != -1) {
            /* Splitting the data into a series of tokens */
            tokens = strtok(line_buffer, ",");

            /* Assigning a unique ID for every website */
            websites[i].id = i;

            /*  */
            for (j = 0; j < NUM_COLUMNS; j++) {
                switch (j) {
                    case 0: websites[i].avg_duration = atoi(&tokens[j]); break;
                    case 1: websites[i].pages_per_visit = atof(&tokens[j]); break;
                    case 2: websites[i].influence = atof(&tokens[j]); break;
                    default: break;
                }
            }
        }
        
        i++;
        for (int x = 0; x < MAX_LINE_LEN; x++) {
            printf("%c ", line_buffer[x]);
        }
        printf("\n");
    }
    
    printf("I'm here now! :-D");
    
    fclose(file_pointer);
}
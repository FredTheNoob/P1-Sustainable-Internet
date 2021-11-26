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
    for (int i = 0; i < num_websites; i++) {
        if (roll >= curr_range && roll <= websites[i].influence + curr_range
            && websites[i].id != previous_website_id) {
            return &websites[i];
        }   
        else {
            curr_range += websites[i].influence;
        }
    }

    printf("Failed to get website in website.c");
    exit(1);
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    char *file_addr = "input/websites_data.csv";
    FILE *file_pointer = fopen(file_addr, "r");
    char line_buffer[MAX_LINE_LEN];
    int i;
    
    /* Opens the input file to read it */
    
    
    if (file_pointer == NULL) {
        printf("ERROR: Can't open file\n");
        exit(1);
    }

    /* Skip over first line of csv file */
    fscanf(file_pointer, " %s", line_buffer);
    
    /* Read file line by line */
    for (i = 0; i < sim_input->num_websites; i++) {
        fscanf(file_pointer, " %s", line_buffer);
        
        websites[i].id = i;
        sscanf(line_buffer, " %hu,%f,%f", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].influence);

        // printf("%d\n", websites[i].avg_duration);
    }
        
    fclose(file_pointer);
}
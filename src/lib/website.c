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
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    char *file_addr = "input/websites_data.csv";
    FILE *file_pointer = fopen(file_addr, "r");
    char line_buffer[MAX_LINE_LEN];
    char *token = NULL;
    char *tokens[NUM_COLUMNS];
    int i = -1, j;
    
    /* Opens the input file to read it */
    
    
    if (file_pointer == NULL) {
        printf("ERROR: Can't open file\n");
        exit(1);
    }

    /* Reads text until newline is encountered */
    while (i < 12) {
        int i1;
        double d2, d3;
        printf("%p\n", file_pointer);
        fscanf(file_pointer, "%d,%lf,%lf", &i1, &d2, &d3);
        printf("%d\n", i);
        
        if (i != -1) {

            
        }
        
        i++;
   
    }
    // for (j = 0; j < sim_input->num_websites; j++) {
    //     printf("ID: %2d\tavg_dur: %4d\tpages_per_visit: %5.2lf\tinfluence: %.4lf\n",
    //     websites[j].id, websites[j].avg_duration, websites[j].pages_per_visit, websites[j].influence
    //     );
    // }
    
    printf("I'm here now! :-D");
    
    fclose(file_pointer);
}
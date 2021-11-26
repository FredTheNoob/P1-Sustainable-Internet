#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "website.h"

#define BUFFER_SIZE 50

Website *get_website_v2(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    double propabilities_sum = websites[0].influence;
    int i = 0;

    while (propabilities_sum < rand_0_1) {
        i++;
        propabilities_sum += websites[i].influence;
    }

    if (i != previous_website_id) {
        return &websites[i];
    } else if (i != num_websites - 1) {
        return &websites[i + 1];
    } else if (i == num_websites - 1) {
        return &websites[i - 1];
    } else {
        printf("Failed to get website in website.c");
        exit(1);
    }
}

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

    printf("Failed to get website in website.c");
    exit(1);
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    char *file_addr = "input/websites_data.csv";
    FILE *file_pointer = fopen(file_addr, "r");
    char line_buffer[BUFFER_SIZE];
    int i;
    
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
    }
    
    /* Close file */
    fclose(file_pointer);

    /* Check if the influences add up to 100 */
    double sum = 0;
    for (unsigned int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].influence;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("Failed to load websites. Influences didn't add up to 100%%\n");
        exit(1);
    }
}
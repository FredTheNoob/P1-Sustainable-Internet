#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "website.h"

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    double propabilities_sum = websites[0].influence;
    int i = 0;

    /* Iterates through websites until the sum exceeds the random number */
    while (propabilities_sum < rand_0_1) {
        i++;
        propabilities_sum += websites[i].influence;
    }

    /* If i is not the previous id */
    if (i != previous_website_id) { 
        return &websites[i];
    }
    /* If i is not the last index */
    else if (i != num_websites - 1) { 
        return &websites[i + 1];
    }
    /* If i is the last index */
    else if (i == num_websites - 1) { 
        return &websites[i - 1];
    } 
    /* Should not happen */
    else { 
        printf("[ERROR] Failed to get website in website.c");
        exit(EXIT_FAILURE);
    }
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    FILE *fp = fopen("input/websites_data.csv", "r");
    char line_buffer[BUFFER_SIZE];
    int i;
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening websites_data.csv\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /* Skip over first line of csv file */
    fscanf(fp, " %s", line_buffer);
    
    /* Read file line by line */
    for (i = 0; i < sim_input->num_websites; i++) {
        fscanf(fp, " %s", line_buffer);
        
        websites[i].id = i;
        sscanf(line_buffer, " %hu,%f,%f", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].influence);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the influences add up to 100 */
    double sum = 0;
    for (unsigned int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].influence;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("[ERROR] Failed to load websites. Influences didn't add up to 100%%\n");
        exit(EXIT_FAILURE);
    }
}
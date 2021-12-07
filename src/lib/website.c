#include "website.h"

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    double propabilities_sum = 0;
    int i = 0;

    /* Iterates through websites until the sum exceeds the random number */
    while (propabilities_sum < rand_0_1 && i < num_websites - 1) {
        propabilities_sum += websites[i].influence;
        i++;
    }

    /* If i is not the previous id */
    if (i != previous_website_id) { 
        return &websites[i];
    }
    /* If i is not the last index */
    else if (i != num_websites - 1) { 
        return &websites[i + 1];
    }
    /* If i is the last index (very unlikely) */
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
    char category_buffer[BUFFER_SIZE];
    int i;
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening websites_data.csv\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /* Skip over first line of csv file */
    while (fgetc(fp) != '\n');
    
    /* Read file line by line */
    for (i = 0; i < sim_input->num_websites; i++) {
        
        websites[i].id = i;
        fscanf(fp, " %hu,%f,%f,%[^\n] ", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].influence, category_buffer);
    
        websites[i].category = get_category(category_buffer);
    }

    /* Print websites */
    for (i = 0; i < sim_input->num_websites; i++) {

        printf("%2d   %lf   %d\n", websites[i].id, websites[i].influence, websites[i].category);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the influences add up to 100 */
    double sum = 0;
    for (unsigned int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].influence;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("[ERROR] Failed to load websites. Influences(=%lf) didn't add up to 1.0 (100%%)\n", sum);
        exit(EXIT_FAILURE);
    }
}

/* Function that returns the corresponding enum */
website_category get_category(char *category) {

    char* categories[] = {
        "Adult",
        "Arts and Entertainment",
        "Computer Technology",
        "Consumer Electronics",
        "E commerce",
        "Email",
        "Encyclopedias",
        "Games",
        "News and Media",
        "Programming and Developer Software",
        "Search Engines",
        "Social Networks and Online Communities",
        "Sports",
        "Telecommunications",
        "Video Streaming",
        "Weather",
    };

    for (int i = ADULT; i <= WEATHER; i++) {
        if (strcmp(category, categories[i]) == 0) return i;
    }

    printf("Category not recognized while loading websites\n"); 
    exit(EXIT_FAILURE);
}
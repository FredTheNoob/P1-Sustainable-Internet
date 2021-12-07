#include "website.h"

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    double propabilities_sum = 0;
    int i = 0;

    /* Iterates through websites until the sum exceeds the random number */
    while (propabilities_sum < rand_0_1 && i < num_websites - 1) {
        propabilities_sum += websites[i].weight;
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
        fscanf(fp, " %hu,%f,%f,%[^\n] ", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].weight, category_buffer);
    
        /* Calculates the website's avg pages shown per minute */
        websites[i].pages_per_minute = websites[i].pages_per_visit / (websites[i].avg_duration / 60);

        /* Assign the category from website data to the selected website */
        websites[i].category = get_category(category_buffer);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the weights add up to 100 */
    double sum = 0;
    for (unsigned int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].weight;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("[ERROR] Failed to load websites. Weights(=%lf) didn't add up to 1.0 (100%%)\n", sum);
        exit(EXIT_FAILURE);
    }
}

/* Function that returns the corresponding enum */
WebsiteCategory get_category(char *category) {

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
#include "website.h"

Website *get_website(Website *websites, const short NUM_WEBSITES, short previous_website_id) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    double propabilities_sum = 0;
    int i = 0;

    /* Iterates through websites until the sum exceeds the random number */
    while (propabilities_sum < rand_0_1 && i < NUM_WEBSITES - 1) {
        propabilities_sum += websites[i].weight;
        i++;
    }

    /* If i is not the previous id */
    if (i != previous_website_id) { 
        return &websites[i];
    }
    /* If i is not the last index */
    else if (i != NUM_WEBSITES - 1) { 
        return &websites[i + 1];
    }
    /* If i is the last index (very unlikely) */
    else if (i == NUM_WEBSITES - 1) { 
        return &websites[i - 1];
    } 
    /* Should not happen */
    else { 
        printf("[ERROR] Failed to get website in website.c");
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

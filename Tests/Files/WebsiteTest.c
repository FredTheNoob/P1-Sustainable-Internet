#include "../CuTest.h"
#include "../../src/lib/simulation.h"

#define AVG_USER_TIME 3600

void load_websites(Website *websites, SimulationInput *sim_input);
WebsiteCategory get_category(char *category);
Website *get_website_modified(Website *websites, const short NUM_WEBSITES, short previous_website_id, double rand_0_1);


void test_get_website_no_previous(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = 50;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, sim_input.num_websites, -1, 0.684);

    int actual_id = website->id;
    int expected_id = 4;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

void test_get_website_different_from_previous(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = 50;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, sim_input.num_websites, 1, 0.684);

    int actual_id = website->id;
    int expected_id = 5;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

void test_get_website_not_last(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = 50;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, sim_input.num_websites, 0, 0.38);

    int actual_id = website->id;
    int expected_id = 1;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

void test_get_website_last(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = 50;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, sim_input.num_websites, 49, 0.998);

    int actual_id = website->id;
    int expected_id = 48;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    FILE *fp = fopen("../input/websites_data.csv", "r");
    char category_buffer[BUFFER_SIZE];
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening websites_data.csv\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /* Skip over first line of csv file */
    while (fgetc(fp) != '\n');
    
    /* Read file line by line */
    for (int i = 0; i < sim_input->num_websites; i++) {
        
        websites[i].id = i;
        fscanf(fp, " %hu,%f,%f,%[^\n] ", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].weight, category_buffer);
    
        /* Calculates the website's avg pages shown per minute */
        websites[i].pages_per_minute = websites[i].pages_per_visit / ((float)websites[i].avg_duration / 60);

        /* Assign the category from website data to the selected website */
        websites[i].category = get_category(category_buffer);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the weights add up to 100 */
    double sum = 0;
    for (int i = 0; i < sim_input->num_websites; i++) {
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

Website *get_website_modified(Website *websites, const short NUM_WEBSITES, short previous_website_id, double rand_0_1) {
    double propabilities_sum = 0;
    int i = 0;

    /* Iterates through websites until the sum exceeds the random number */
    while (propabilities_sum < rand_0_1 && i < NUM_WEBSITES - 1) {
        propabilities_sum += websites[i].weight;
        i++;
    }

    /* If we have no previous id */
    if (previous_website_id == -1) {
        return &websites[i - 1];
    }
    /* If i is not the previous id */
    else if (i != previous_website_id) { 
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

CuSuite* websiteGetSuite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_get_website_no_previous);
    SUITE_ADD_TEST(suite, test_get_website_different_from_previous);
    SUITE_ADD_TEST(suite, test_get_website_not_last);
    SUITE_ADD_TEST(suite, test_get_website_last);
    
    return suite;
}
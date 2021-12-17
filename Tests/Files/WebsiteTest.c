#include "../CuTest.h"
#include "../../src/lib/simulation.h"
#include <time.h>

#define AVG_USER_TIME 3600

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
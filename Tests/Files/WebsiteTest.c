#include "../CuTest.h"
#include "../../src/lib/simulation.h"
#include <time.h>

#define AVG_USER_TIME 3600
#define NUM_WEBSITES 50

Website *get_website_modified(Website *websites, short previous_website_id, double rand_0_1);

void test_get_website_not_previous(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = NUM_WEBSITES;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, -1, 0.684);

    int actual_id = website->id;
    int expected_id = 4;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

void test_get_website_is_previous(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = NUM_WEBSITES;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, 0, 0.3);


    int actual_id = website->id;
    int expected_id = 0;

    /* If the function does not return the same index again, we know it works since we shouldn't be able to get previous again */
    CuAssertTrue(tc, actual_id != expected_id);
}

void test_get_website_out_of_bounds(CuTest *tc) {
    SimulationInput sim_input;
    sim_input.num_websites = NUM_WEBSITES;

    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    Website *website = get_website_modified(websites, -1, 0.99999999999999);

    int actual_id = website->id;
    int expected_id = 49;

    CuAssertIntEquals(tc, expected_id, actual_id);
}

Website *get_website_modified(Website *websites, short previous_website_id, double rand_0_1) {
    double probabilities_sum = 0;
    int i = 0;

    while (probabilities_sum < rand_0_1) {
        probabilities_sum += websites[i].weight;
        i++;
    }

    if (i - 1 != previous_website_id) { 
        return &websites[i - 1];
    }
    else {
        return get_website(websites, previous_website_id);
    }
}

CuSuite* websiteGetSuite() {
    srand(time(NULL));

    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_get_website_not_previous);
    SUITE_ADD_TEST(suite, test_get_website_is_previous);
    SUITE_ADD_TEST(suite, test_get_website_out_of_bounds);

    return suite;
}
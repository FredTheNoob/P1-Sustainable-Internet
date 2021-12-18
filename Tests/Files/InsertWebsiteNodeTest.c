#include "../CuTest.h"
#include "../../src/lib/simulation.h"

#define NUM_CATEGORIES 16
#define CURR_CATEGORY 8

/* CuTest Functions */
void test_insert_website_node_start(CuTest *tc);
void test_insert_website_node_between(CuTest *tc);
void test_insert_website_node_end(CuTest *tc);

Website *make_stub_website(int category, float pages_per_minute);
WebsiteNode *get_node_at_index(WebsiteNode **linked_list, int index);

void free_linked_websites(WebsiteNode **linked_websites);


void test_insert_website_node_start(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.75));

    int expected_index = 0;
    float inserted_val = 0.84;

    /* The website we're checking needs to be bigger than 0.75 (in this case) */
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, inserted_val));

    CuAssertTrue(tc, linked_websites[CURR_CATEGORY]->website->pages_per_minute == inserted_val);

    /* Free all WebsiteNodes and their Websites */
    free_linked_websites(linked_websites);
    
}

void test_insert_website_node_between(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.75));
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.84));
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.75));

    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.66));

    int expected_index = 3;
    float inserted_val = 0.69;

    /* The website we're checking needs to be between 0.75 and 0.66 (in this case) */
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, inserted_val));

    WebsiteNode *found_node = get_node_at_index(linked_websites, expected_index);

    CuAssertTrue(tc, found_node->website->pages_per_minute == inserted_val);

    /* Free all WebsiteNodes and their Websites */
    free_linked_websites(linked_websites);
}

void test_insert_website_node_end(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.75));
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.84));
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, 0.75));

    int expected_index = 3;
    float inserted_val = 0.69;

    /* The website we're checking needs to be below 0.75 (in this case) */
    insert_website_node(linked_websites, make_stub_website(CURR_CATEGORY, inserted_val));

    WebsiteNode *found_node = get_node_at_index(linked_websites, expected_index);

    CuAssertTrue(tc, found_node->website->pages_per_minute == inserted_val);


    /* Free all WebsiteNodes and their Websites */
    free_linked_websites(linked_websites);
}

Website *make_stub_website(int category, float pages_per_minute) {
    Website* website = (Website*)malloc(sizeof(Website));
    website->category = category;
    website->pages_per_minute = pages_per_minute;

    return website;
}

WebsiteNode *get_node_at_index(WebsiteNode **linked_list, int index) {
    WebsiteNode *current_node = linked_list[CURR_CATEGORY];
    
    for (int i = 0; i < index; i++)
    {
        current_node = current_node->next;
    }
    
    return current_node;
}

CuSuite* simGetSuite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_insert_website_node_start);
    SUITE_ADD_TEST(suite, test_insert_website_node_between);
    SUITE_ADD_TEST(suite, test_insert_website_node_end);

    return suite;
}

void free_linked_websites(WebsiteNode **linked_websites) {
    
    WebsiteNode *current_node = linked_websites[CURR_CATEGORY];
    WebsiteNode *temp_current_node;

    while (current_node != NULL) {

        temp_current_node = current_node;

        current_node = current_node->next;

        free(temp_current_node->website);
        free(temp_current_node);
    }
}
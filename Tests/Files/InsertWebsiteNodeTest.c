#include "../CuTest.h"
#include "../../src/lib/website.h"

#define NUM_CATEGORIES 16
#define CURR_CATEGORY 8

/* CuTest Functions */
void test_insert_website_node_start(CuTest *tc);
void test_insert_website_node_between(CuTest *tc);
void test_insert_website_node_end(CuTest *tc);

/* Imported functions from project modified for testing */
int insert_website_node(WebsiteNode **linked_websites, Website *website);
WebsiteNode *alloc_website_node(Website *website);
Website *make_stub_website(int category, float pages_per_minute);

void test_insert_website_node_start(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(8, 0.75));
    /* The website we're checking needs to be bigger than 0.75 (in this case) */
    int actual_index = insert_website_node(linked_websites, make_stub_website(8, 0.84));

    int expected_index = 0;
    CuAssertIntEquals(tc, expected_index, actual_index);
}

void test_insert_website_node_between(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(8, 0.75));
    insert_website_node(linked_websites, make_stub_website(8, 0.84));
    insert_website_node(linked_websites, make_stub_website(8, 0.75));

    /* The website we're checking needs to be between 0.75 and 0.66 (in this case) */
    int actual_index = insert_website_node(linked_websites, make_stub_website(8, 0.69));

    insert_website_node(linked_websites, make_stub_website(8, 0.66));

    int expected_index = 3;
    CuAssertIntEquals(tc, expected_index, actual_index);
}

void test_insert_website_node_end(CuTest *tc) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * NUM_CATEGORIES);

    insert_website_node(linked_websites, make_stub_website(8, 0.75));
    insert_website_node(linked_websites, make_stub_website(8, 0.84));
    insert_website_node(linked_websites, make_stub_website(8, 0.75));

    /* The website we're checking needs to be below 0.75 (in this case) */
    int actual_index = insert_website_node(linked_websites, make_stub_website(8, 0.69));

    int expected_index = 3;
    CuAssertIntEquals(tc, expected_index, actual_index);
}

int insert_website_node(WebsiteNode **linked_websites, Website *website) {
    WebsiteNode *new_node = NULL, *previous_node = NULL, *current_node = linked_websites[website->category];  
    int curr_index = 0;

    if (current_node == NULL) {
        new_node = alloc_website_node(website);
        linked_websites[website->category] = new_node;         
    }
    else {
        while (current_node != NULL && current_node->website->pages_per_minute > website->pages_per_minute) {
            previous_node = current_node;
            current_node = current_node->next;
            curr_index++;
        }

        new_node = alloc_website_node(website);

        /* Insert at the start of the list */
        if (previous_node == NULL) {
            new_node->next = current_node;
            linked_websites[website->category] = new_node;
        }
        /* Insert at the end of the list */
        else if (current_node == NULL) {
            previous_node->next = new_node;

        }
        /* Insert in between */
        else {
            previous_node->next = new_node;
            new_node->next = current_node;
        }
        return curr_index;
    }
} 

WebsiteNode *alloc_website_node(Website *website) {
    WebsiteNode *new_website_node = (WebsiteNode*)malloc(sizeof(WebsiteNode));
    
    if (new_website_node == NULL) {
        printf("Failed to allocate memory for website_node\n\n");
        exit(EXIT_FAILURE);
    }

    new_website_node->website = website;
    new_website_node->next = NULL;

    return new_website_node;
}

Website *make_stub_website(int category, float pages_per_minute) {
    Website* website = (Website*)malloc(sizeof(Website));
    website->category = category;
    website->pages_per_minute = pages_per_minute;

    return website;
}

CuSuite* simGetSuite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_insert_website_node_start);
    SUITE_ADD_TEST(suite, test_insert_website_node_between);
    SUITE_ADD_TEST(suite, test_insert_website_node_end);

    return suite;
}
#ifndef WEBSITE
#define WEBSITE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"

#define BUFFER_SIZE 50
#define NUM_CATEGORIES 16

enum website_category {
    ADULT,
    ARTS_AND_ENTERTAINMENT,
    COMPUTER_TECHNOLOGY,
    CONSUMER_ELECTRONICS,
    E_COMMERCE,
    EMAIL,
    ENCYCLOPEDIAS,
    GAMES,
    NEWS_AND_MEDIA,
    PROGRAMMING_AND_DEVELOPER_SOFTWARE,
    SEARCH_ENGINES,
    SOCIAL_NETWORKS_AND_ONLINE_COMMUNITIES,
    SPORTS,
    TELECOMMUNICATION,
    VIDEO_STREAMING,
    WEATHER
};
typedef enum website_category website_category;

struct Website {
    short id;
    website_category category;
    unsigned short avg_duration;
    float pages_per_visit;
    float influence;
};
typedef struct Website Website;

website_category get_category(char *category);

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id);

void load_websites(Website *websites, SimulationInput *sim_input);

#endif

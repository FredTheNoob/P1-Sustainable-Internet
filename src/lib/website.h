#ifndef WEBSITE
#define WEBSITE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 50

typedef struct WebsiteAlternative WebsiteAlternative;

enum WebsiteCategory {
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
typedef enum WebsiteCategory WebsiteCategory;

struct Website {
    short id;
    short category_index;
    WebsiteCategory category;
    short avg_duration;
    float pages_per_visit;
    float pages_per_minute;
    float weight;
    WebsiteAlternative *alternatives_matrix;
};
typedef struct Website Website;

struct WebsiteNode {
    Website *website;
    struct WebsiteNode *next;
};
typedef struct WebsiteNode WebsiteNode;

struct WebsiteAlternative {
    Website **matrix;
    short num_x;
    short num_y;
    short num_websites_in_category;
};
typedef struct WebsiteAlternative WebsiteAlternative;



WebsiteCategory get_category(char *category);

Website *get_website(Website *websites, const short NUM_WEBSITES, short previous_website_id);

#endif

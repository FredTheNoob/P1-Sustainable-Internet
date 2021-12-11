#ifndef USER
#define USER

#include <stdbool.h>

#include "website.h"

struct User {
    unsigned int total_daily_time;
    unsigned int max_daily_time;
    float total_pages;
    Website *current_website;
};
typedef struct User User;

void reset_users(User *users, const short num_users);

void handle_user(User *user, Website *websites, WebsiteNode **linked_websites, const short NUM_WEBSITES, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *chosen_website); /* Assign website to user */

Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short num_categories);

Website *choose_website(Website *website, Website *sustainable_website, float sustainable_choice);

#endif

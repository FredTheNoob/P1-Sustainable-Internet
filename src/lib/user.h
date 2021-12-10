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

void reset_users(User *users, unsigned short num_users);

void handle_user(User *user, Website *websites, unsigned short num_websites); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *websites, unsigned short num_websites); /* Assign website to user */

Website *recommend_website(WebsiteNode *linked_websites, Website *current_website, short num_categories);

bool choose_website(User *user, Website *recommended_website, float sustainable_choice);

#endif

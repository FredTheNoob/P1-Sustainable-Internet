#ifndef USER
#define USER

#include <stdbool.h>

#include "website.h"

#define NO_WEBSITE_ID -1
#define NO_USER_ID -1

struct User {
    int id;
    short daily_time_spent;
    short max_daily_time;
    float total_pages;
    Website *current_website;
};
typedef struct User User;

void reset_users(User *users, const int NUM_USERS);

void handle_user(User *user, Website *websites, WebsiteNode **linked_websites, const short NUM_WEBSITES, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *chosen_website);

Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short user_id, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE);

Website *choose_website(Website *website, Website *sustainable_website, short user_id, const float SUSTAINABLE_CHOICE);

#endif

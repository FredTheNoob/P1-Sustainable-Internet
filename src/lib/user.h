#ifndef USER
#define USER

#include "website.h"

struct User {
    unsigned int current_time;
    unsigned int max_daily_time;
    Website *current_website;
};
typedef struct User User;

void handle_website(User *user, Website *websites, unsigned short time_increment); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *websites); /* Assign website to user */

#endif
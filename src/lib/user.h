#ifndef USER
#define USER

#include <stdio.h>
#include <stdbool.h>

#include "website.h"
#include "simulation.h"

struct User {
    unsigned int current_time;
    unsigned int total_daily_time;
    unsigned int max_daily_time;
    bool has_reached_max_daily_time;
    float total_clicks;
    Website *current_website;
};
typedef struct User User;

void generate_users(User *users, SimulationInput *sim_input);

void reset_users(User *users, unsigned short num_users);

void handle_user(User *user, Website *websites, unsigned short num_websites, unsigned short time_increment); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *websites, unsigned short num_websites); /* Assign website to user */

#endif
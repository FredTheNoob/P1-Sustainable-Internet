#include <stdio.h>
#include "user.h"

/* Generate user data for the given number of users (from the simulation input file) */
void generate_users(User *users, SimulationInput *sim_input) {
    for (int i = 0; i < sim_input->num_users; i++) {
        users[i].current_time = 0;
        users[i].current_website = NULL;
        users[i].max_daily_time = sim_input->avg_user_time;
    }
}

/* Logic to conrol whether a user should be assigned a new website */
void handle_website(User *user, Website *websites, unsigned int num_websites, unsigned short time_increment) { 
    if (user->current_time == 0) {
        assign_website(user, websites, num_websites);
    } else if (user->current_time < user->max_daily_time) {
        user->current_time += time_increment;
    } else {
        user->current_time = 0;
    }
}

/* Assign website to user */
void assign_website(User *user, Website *websites, unsigned int num_websites) {
    int prev_website_id;

    /* Determine whether the user has a current website or not */
    if (user->current_website == NULL) {
        prev_website_id = -1;
    } else {
        prev_website_id = user->current_website->id;
    }

    /* Gets a single website from the website data and assigns it to the given user's current website */
    user->current_website = get_website(websites, num_websites, prev_website_id);
} 
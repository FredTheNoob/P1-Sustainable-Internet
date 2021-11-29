#include "user.h"

/* Generate user data for the given number of users (from the simulation input file) */
void generate_users(User *users, SimulationInput *sim_input) {
    for (int i = 0; i < sim_input->num_users; i++) {
        users[i].current_time = 0;
        users[i].total_daily_time = 0;
        users[i].max_daily_time = sim_input->avg_user_time; /* ----- generate "random" avg_user_time ----- */
        users[i].has_reached_max_daily_time = false;
        users[i].total_clicks = 0;
        users[i].current_website = NULL;
    }
}

void reset_users(User *users, unsigned short num_users) {
    for (int i = 0; i < num_users; i++) {
        users[i].has_reached_max_daily_time = false;
        users[i].total_daily_time = 0;
        users[i].current_time = 0;
        users[i].current_website = NULL;
    }
}

/* Logic to conrol whether a user should be assigned a new website */
void handle_website(User *user, Website *websites, unsigned short num_websites, unsigned short time_increment) { 
    /* ---------------- Fix current_time ---------------- */
    if (user->current_time == 0) {
        assign_website(user, websites, num_websites);
        user->current_time += time_increment;
    }
    else if (user->current_time < user->current_website->avg_duration) {
        user->current_time += time_increment;
    }
    else if (user->total_daily_time >= user->max_daily_time) {
        user->has_reached_max_daily_time = true;
    }
    else {
        user->total_daily_time += user->current_time;
        user->current_time = 0;
        
        /* Add the current website's avg pages per visit to the the user's total clicks */
        user->total_clicks += user->current_website->pages_per_visit;
    }
}

/* Assign website to user */
void assign_website(User *user, Website *websites, unsigned short num_websites) {
    int prev_website_id;

    /* Determine whether the user has a current website or not */
    if (user->current_website == NULL) {
        prev_website_id = -1;
    } 
    else {
        prev_website_id = user->current_website->id;
    }

    /* Gets a single website from the website data and assigns it to the given user's current website */
    user->current_website = get_website(websites, num_websites, prev_website_id);
} 
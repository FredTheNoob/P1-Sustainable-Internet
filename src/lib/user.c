#include "user.h"

/* Generate user data for the given number of users (from the simulation input file) */
void generate_users(User *users, SimulationInput *sim_input) {
    for (int i = 0; i < sim_input->num_users; i++) {
        users[i].total_daily_time = 0;
        users[i].max_daily_time = sim_input->avg_user_time; /* ----- generate "random" avg_user_time ----- */
        users[i].total_pages = 0;
        users[i].current_website = NULL;
    }
}

void reset_users(User *users, unsigned short num_users) {
    for (int i = 0; i < num_users; i++) {
        users[i].total_daily_time = 0;
        users[i].current_website = NULL;
    }
}

/* Logic to conrol whether a user should be assigned a new website */
void handle_user(User *user, Website *websites, unsigned short num_websites) { 
    
    /* Assign first website to user */
    assign_website(user, websites, num_websites);

    /* If user's daily time + their current website's avg duration doesn't exceed their max daily time */
    while (user->total_daily_time + user->current_website->avg_duration < user->max_daily_time) {

        user->total_daily_time += user->current_website->avg_duration;

        user->total_pages += user->current_website->pages_per_visit;

        assign_website(user, websites, num_websites);
    }
    
    /* Calculates the remaining pages */
    float remaining_pages = (float)(user->max_daily_time - user->total_daily_time) / 
                            (float)(user->current_website->avg_duration * 
                            user->current_website->pages_per_visit); 

    user->total_pages += remaining_pages;
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

Website *recommend_website(WebsiteNode *linked_websites, Website *current_website, short num_categories) {
    Website *recommended_website; /* Pointer to the recommended website in the list */
    
    while (current_website->category) {

    }

    return recommended_website;
}
#include "user.h"

void generate_users(User *users, SimulationInput *sim_input) {

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
    user->current_website = get_website(websites, num_websites, user->current_website->id);
} 
#ifndef USER
#define USER

#include "website.h"
#include "simulation.h"

struct User {
    unsigned int current_time;
    unsigned int max_daily_time;
    Website *current_website;
};
typedef struct User User;

void generate_users(User *users, SimulationInput *sim_input);

void handle_website(User *user, Website *websites, unsigned int num_websites, unsigned short time_increment); /* Logic to conrol whether a user should be assigned a new website */

void assign_website(User *user, Website *websites, unsigned int num_websites); /* Assign website to user */

#endif
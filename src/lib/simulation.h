#ifndef SIMULATION
#define SIMULATION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "user.h"

typedef struct Website Website;

struct SimulationInput {
    unsigned short num_websites;
    unsigned short num_categories;
    unsigned short num_users;
    unsigned short avg_user_time;
    unsigned short sim_duration_days;
    unsigned short num_simulations;
    float sustainable_choice;
};
typedef struct SimulationInput SimulationInput;

struct SimulationOutput {
    float total_pages;
};
typedef struct SimulationOutput SimulationOutput;

SimulationInput get_sim_input(void); /* Read the input file */

int check_key(char *key, char *valid_key);

void generate_users(User *users, SimulationInput *sim_input);

void load_websites(Website *websites, SimulationInput *sim_input);

void convert_websites(WebsiteNode **linked_websites, Website *websites, SimulationInput *sim_input);

SimulationOutput run_simulation(SimulationInput *sim_input, User *users, Website *websites, WebsiteNode **linked_websites);

void generate_matrices(WebsiteAlternative **website_matrices, WebsiteNode **linked_websites, unsigned short num_categories, unsigned short num_users, const unsigned short NUM_WEBSITE_ALTERNATIVES);

void print_sim_input(SimulationInput *sim_input);

void print_simulation_output(SimulationOutput *sim_output, unsigned short num_simulations);

#endif

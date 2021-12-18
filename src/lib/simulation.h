#ifndef SIMULATION
#define SIMULATION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "user.h"

#define MAX_FILE_NAME_LEN 50

typedef struct Website Website;

struct SimulationInput {
    short num_websites;
    short num_categories;
    short avg_user_time;
    short sim_duration_days;
    short num_simulations;
    int num_users;
    float sustainable_choice;
    float sustainable_choice_increment;
};
typedef struct SimulationInput SimulationInput;

struct SimulationOutput {
    float total_pages;
};
typedef struct SimulationOutput SimulationOutput;

SimulationInput get_sim_input(void); /* Read the input file */

void generate_users(User *users, SimulationInput *sim_input);

void load_websites(Website *websites, SimulationInput *sim_input);

WebsiteNode **convert_websites(Website *websites, SimulationInput *sim_input);

void insert_website_node(WebsiteNode **linked_websites, Website *website);

WebsiteNode *alloc_website_node(Website *website);

void print_linked_websites(WebsiteNode **linked_websites, int num_categories);

SimulationOutput run_simulation(SimulationInput *sim_input, User *users, Website *websites, WebsiteNode **linked_websites);

void generate_matrices(WebsiteNode **linked_websites, const short NUM_CATEGORIES, const int NUM_USERS, const short NUM_WEBSITE_ALTERNATIVES);

void write_sim_output(SimulationOutput *sim_outputs, const short NUM_SIMULATIONS, const short SIM_DURATION_DAYS, const float SUSTAINABLE_CHOICE);

void create_file_name(char file_name[MAX_FILE_NAME_LEN], const float SUSTAINABLE_CHOICE);

void combine_output_files(short num_simulations, const short num_files);

void free_all(WebsiteNode **linked_websites, short num_categories);

int check_key(char *key, char *valid_key);

void print_sim_input(SimulationInput *sim_input);

#endif

#ifndef SIMULATION
#define SIMULATION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "user.h"
#include "website.h"

struct SimulationInput {
    unsigned short num_websites;
    unsigned short num_users;
    unsigned short avg_user_time;
    unsigned short sim_duration_days;
    unsigned short num_simulations;
};
typedef struct SimulationInput SimulationInput;

struct SimulationOutput {
    float total_pages;
};
typedef struct SimulationOutput SimulationOutput;

int check_key(char *key, char *valid_key);
void print_sim_input(SimulationInput *sim_input);
SimulationInput get_sim_input(void); /* Read the input file */

SimulationOutput run_simulation(SimulationInput *sim_input);

void vary_input_parameter(SimulationInput *sim_input, Website *website, char *parameter, int multiplier);

void print_simulation_output(SimulationOutput *sim_output, unsigned short num_simulations);

#endif

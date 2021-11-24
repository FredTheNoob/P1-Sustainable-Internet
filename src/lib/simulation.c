#include "simulation.h"
#include "user.h"
#include "website.h"

SimulationInput get_sim_input() {
    /* Temporarily hard-coded input. In final version input should be read from a file */
    SimulationInput sim_input = {50, 10, 3600, 10, 7200}; /* num_users, avg_user_time, time_increment, sim_duration */

    /* Returning simulation input */
    return sim_input;
}


void run_simulation (SimulationInput *sim_input) {
    unsigned int current_time = 0;
    unsigned short user_index;

    SimulationInput simulation_input;
    simulation_input = get_sim_input(); /*Get simulation input*/

    /* Create arrrays for users */
    User users_list[simulation_input.num_users];
    
    /* Create function call for website list */
    Website website_list[simulation_input.num_websites];

    /* check if users internet time has run out : false -> go fourth : true -> write to output file */

    while(current_time < simulation_input.sim_duration){

        /* loop through all users and call handle website function */
        for (user_index = 0; user_index < simulation_input.num_users; user_index++){
            handle_website(&users_list[user_index], website_list, simulation_input.time_increment);    
        }

        /* Incriment time and go back up to time check for loop */
        current_time += simulation_input.time_increment;
    }

    /* Figure out output for function */

    /* print_simulation_output(); */
}

void print_simulation_output () {
    
}


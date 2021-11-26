#include "simulation.h"
#include "user.h"
#include "website.h"

SimulationInput get_sim_input() {
    /* Temporarily hard-coded input. In final version input should be read from a file */
    SimulationInput sim_input = {50, 10, 3600, 10, 7200}; /* num_users, num_websites, avg_user_time, time_increment, sim_duration */

    /* Returning simulation input */
    return sim_input;
}


void run_simulation (SimulationInput *sim_input) {
    unsigned int sim_time = 0;
    unsigned short user_index;

    SimulationInput simulation_input;
    simulation_input = get_sim_input(); /*Get simulation input*/

    /* Create arrays for users */
    User users[simulation_input.num_users];
    generate_users(users, &simulation_input);
    
    /* Create function call for website list */
    Website websites[simulation_input.num_websites];
    load_websites(websites, &simulation_input);

    /* check if users internet time has run out : false -> go fourth : true -> write to output file */

    while(sim_time < simulation_input.sim_duration){

        /* loop through all users and call handle website function */
        for (user_index = 0; user_index < simulation_input.num_users; user_index++){
            handle_website(&users[user_index], websites, simulation_input.num_websites, simulation_input.time_increment);    
        }

        /* Incriment time and go back up to time check for loop */
        sim_time += simulation_input.time_increment;
    }

    /* Figure out output for function */

    /* print_simulation_output(); */
}

void print_simulation_output () {
    
}


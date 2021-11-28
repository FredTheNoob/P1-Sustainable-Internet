#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "simulation.h"
#include "user.h"
#include "website.h"

SimulationInput get_sim_input() {
    FILE *fp;
    SimulationInput sim_input;
    char key[BUFFER_SIZE];
    int value, i;
    

    /* Opens the input file in read mode (r) */
    fp = fopen("input/simulation_input.txt", "r");
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening simulation_input.txt\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /*  */
    i = 0;
    while (!feof(fp)) {
        fscanf(fp, "%s", key);
        fscanf(fp, "%d", &value);

        if (check_key(key, "NUM_USERS")) {
            sim_input.num_users = value;
        }
        else if (check_key(key, "NUM_WEBSITES")) {
            sim_input.num_websites = value;
        }
        else if (check_key(key, "AVG_USER_TIME")) {
            sim_input.avg_user_time = value;
        }
        else if (check_key(key, "TIME_INCREMENT")) {
            sim_input.time_increment = value;
        }
        else if (check_key(key, "SIM_DURATION")) {
            sim_input.sim_duration = value;
        }
        else {
            printf("[WARNING] %s: Invalid parameter detected in simulation_input.txt on line %d (%s). This parameter will be ignored\n\n", __FILE__, i + 1, key);
        }

        i++;
    }

    fclose(fp);
    
    /*
    print_sim_input(&sim_input);
    */

    /* We should maybe validate sim_input here with some function, if some members we're not set correctly (if val is not a number, etc) */


    /* Returning simulation input */
    return sim_input;
}


void run_simulation(SimulationInput *simulation_input) {
    unsigned int sim_time = 0;
    unsigned short user_index;

    srand(time(NULL));

    /* Create arrays for users */
    User users[simulation_input->num_users];
    generate_users(users, simulation_input);

    
    /* Create function call for website list */
    Website websites[simulation_input->num_websites];
    load_websites(websites, simulation_input);


    /* check if users internet time has run out : false -> go fourth : true -> write to output file */

    while(sim_time < simulation_input->sim_duration){

        /* loop through all users and call handle website function */
        for (user_index = 0; user_index < simulation_input->num_users; user_index++){
            handle_website(&users[user_index], websites, simulation_input->num_websites, simulation_input->time_increment);
        }

        /* Incriment time and go back up to time check for loop */
        sim_time += simulation_input->time_increment;
    }

    /* Figure out output for function */

    /* print_simulation_output(); */
}

void print_simulation_output () {
    
}

int check_key(char *key, char *valid_key) {
    return strcmp(key, valid_key) == 0;
}

/* Print for debugging */
void print_sim_input(SimulationInput *sim_input) {
    printf("users: %d, websites: %d, user_time: %d, time_inc: %d, sim_dur: %d\n", sim_input->num_users, 
    sim_input->num_websites, 
    sim_input->avg_user_time,
    sim_input->time_increment,
    sim_input->sim_duration);
}

#include "simulation.h"
#include "user.h"

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

    /* While EOF has not been reached, load data into sim_imput */
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
        else if (check_key(key, "SIM_DURATION_DAYS")) {
            sim_input.sim_duration_days = value;
        }
        else if (check_key(key, "NUM_SIMULATIONS")) {
            sim_input.num_simulations = value;
        }
        else {
            printf("[WARNING] %s: Invalid parameter detected in simulation_input.txt on line %d (%s). This parameter will be ignored\n\n", __FILE__, i + 1, key);
        }

        i++;
    }

    fclose(fp);

    /* Returning simulation input */
    return sim_input;
}

SimulationOutput run_simulation(SimulationInput *simulation_input) {
    SimulationOutput simulation_output;
    unsigned short sim_days, user_index;

    /* Create array of users */
    User users[simulation_input->num_users];
    generate_users(users, simulation_input);

    /* Create array of websites */
    Website websites[simulation_input->num_websites];
    load_websites(websites, simulation_input);


    /* Vary input parameters */
    vary_input_parameter(simulation_input, websites, "PAGES_PER_VISIT", 0.8);


    /* Main loop - keeps looping until sim_days reaches sim_duration_days */
    for (sim_days = 0; sim_days < simulation_input->sim_duration_days; sim_days++) {

        /* loop through all users and call handle website function */
        for (user_index = 0; user_index < simulation_input->num_users; user_index++) {
            handle_user(&users[user_index], websites, simulation_input->num_websites);
        }

        /* Handle output before resetting users */

        /* Resets all users */
        reset_users(users, simulation_input->num_users);
    }

    /* Calculate total number of pages downloaded */
    simulation_output.total_pages = 0;
    for (int i = 0; i < simulation_input->num_users; i++) {
        simulation_output.total_pages += users[i].total_pages;
    }


    return simulation_output;
}

void vary_input_parameter(SimulationInput *sim_input, Website *websites, char *parameter, float multiplier) {
    if (strcmp(parameter, "PAGES_PER_VISIT") == 0) {
        for (int i = 0; i < sim_input->num_websites; i++) {
            websites[i].pages_per_visit *= multiplier;
        }
    }
}

void print_simulation_output(SimulationOutput *sim_outputs, unsigned short num_simulations) {
    
    /* Print smt about the simulation - Parameters */

    for (int i = 0; i < num_simulations; i++) {
        printf("%f\n", sim_outputs[i].total_pages); 
    }


    /* Eventually print what websites were accessed the most? - So in terms of clicks */
    /* Categorys for websites */
    /* Score based on how long the user spends on a website and how many clicks are used */
    /* Traceroute for users and what websites they accessed */

}

/* Check if a key is valid */
int check_key(char *key, char *valid_key) {
    return strcmp(key, valid_key) == 0;
}

/* Print for debugging */
void print_sim_input(SimulationInput *sim_input) {
    printf("users: %d, websites: %d, user_time: %d, sim_dur: %d\n", sim_input->num_users, 
    sim_input->num_websites, 
    sim_input->avg_user_time,
    sim_input->sim_duration_days);
}

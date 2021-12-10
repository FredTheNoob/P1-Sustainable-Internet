#include "simulation.h"
#include "user.h"

SimulationInput get_sim_input() {
    FILE *fp;
    SimulationInput sim_input;
    char key[BUFFER_SIZE];
    float value;
    int i;

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
        fscanf(fp, "%f", &value);

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
        else if (check_key(key, "SUSTAINABLE_CHOICE")) {
            sim_input.sustainable_choice = value;
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

/* Generate user data for the given number of users (from the simulation input file) */
void generate_users(User *users, SimulationInput *sim_input) {
    for (int i = 0; i < sim_input->num_users; i++) {
        users[i].total_daily_time = 0;
        users[i].max_daily_time = sim_input->avg_user_time; /* ----- generate "random" avg_user_time ----- */
        users[i].total_pages = 0;
        users[i].current_website = NULL;
    }
}

void load_websites(Website *websites, SimulationInput *sim_input) {
    FILE *fp = fopen("input/websites_data.csv", "r");
    char category_buffer[BUFFER_SIZE];
    int i;
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening websites_data.csv\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /* Skip over first line of csv file */
    while (fgetc(fp) != '\n');
    
    /* Read file line by line */
    for (i = 0; i < sim_input->num_websites; i++) {
        
        websites[i].id = i;
        fscanf(fp, " %hu,%f,%f,%[^\n] ", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].weight, category_buffer);
    
        /* Calculates the website's avg pages shown per minute */
        websites[i].pages_per_minute = websites[i].pages_per_visit / (websites[i].avg_duration / 60);

        /* Assign the category from website data to the selected website */
        websites[i].category = get_category(category_buffer);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the weights add up to 100 */
    double sum = 0;
    for (unsigned int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].weight;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("[ERROR] Failed to load websites. Weights(=%lf) didn't add up to 1.0 (100%%)\n", sum);
        exit(EXIT_FAILURE);
    }
}

void convert_websites(WebsiteNode *linked_websites, Website *websites, SimulationInput *sim_input) {
    /* Write the function here */
}

SimulationOutput run_simulation(SimulationInput *simulation_input, User *users, Website *websites) {
    SimulationOutput simulation_output;
    unsigned short sim_days, user_index;

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

void print_simulation_output(SimulationOutput *sim_outputs, unsigned short num_simulations) {
    
    /* Print smt about the simulation - Parameters */

    for (int i = 0; i < num_simulations; i++) {
        printf("%.2f\n", sim_outputs[i].total_pages); 
    }

    /* Eventually print what websites were accessed the most? - So in terms of clicks */
    /* Categorys for websites */
    /* Score based on how long the user spends on a website and how many clicks are used */
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

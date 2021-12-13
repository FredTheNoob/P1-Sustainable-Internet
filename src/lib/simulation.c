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
        else if (check_key(key, "NUM_CATEGORIES")) {
            sim_input.num_categories = value;
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
    
    if (fp == NULL) {
        printf("[ERROR] %s: Error opening websites_data.csv\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    /* Skip over first line of csv file */
    while (fgetc(fp) != '\n');
    
    /* Read file line by line */
    for (int i = 0; i < sim_input->num_websites; i++) {
        
        websites[i].id = i;
        fscanf(fp, " %hu,%f,%f,%[^\n] ", &websites[i].avg_duration, &websites[i].pages_per_visit, &websites[i].weight, category_buffer);
    
        /* Calculates the website's avg pages shown per minute */
        websites[i].pages_per_minute = websites[i].pages_per_visit / ((float)websites[i].avg_duration / 60);

        /* Assign the category from website data to the selected website */
        websites[i].category = get_category(category_buffer);
    }
    
    /* Close file */
    fclose(fp);

    /* Check if the weights add up to 100 */
    double sum = 0;
    for (int i = 0; i < sim_input->num_websites; i++) {
        sum += websites[i].weight;
    }
    if (sum < 0.999 || sum > 1.001) {
        printf("[ERROR] Failed to load websites. Weights(=%lf) didn't add up to 1.0 (100%%)\n", sum);
        exit(EXIT_FAILURE);
    }
}

WebsiteNode **convert_websites(Website *websites, SimulationInput *sim_input) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * sim_input->num_categories);

    /* Set all indexes to NULL */
    for (int i = 0; i < sim_input->num_categories; i++) {
        linked_websites[i] = NULL;
    }

    /* Insert a website node for each website in the website array */
    for (int i = 0; i < sim_input->num_websites; i++) {
        insert_website_node(linked_websites, &websites[i]);
    }

    return linked_websites;
}
    
void insert_website_node(WebsiteNode **linked_websites, Website *website) {
    WebsiteNode *new_node = NULL, *previous_node = NULL, *current_node = linked_websites[website->category];  
    
    if (current_node == NULL) {
        new_node = alloc_website_node(website);
        linked_websites[website->category] = new_node;         
    }
    else {
        while (current_node != NULL && current_node->website->pages_per_minute > website->pages_per_minute) {
            previous_node = current_node;
            current_node = current_node->more_sustainable_website;
        }

        new_node = alloc_website_node(website);

        /* Insert at the start of the list */
        if (previous_node == NULL) {
            new_node->more_sustainable_website = current_node;
            linked_websites[website->category] = new_node;
        }
        /* Insert at the end of the list */
        else if (current_node == NULL) {
            previous_node->more_sustainable_website = new_node;

        }
        /* Insert in between */
        else {
            previous_node->more_sustainable_website = new_node;
            new_node->more_sustainable_website = current_node;
        }
    }
} 

WebsiteNode *alloc_website_node(Website *website) {
    WebsiteNode *new_website_node = (WebsiteNode*)malloc(sizeof(WebsiteNode));
    
    if (new_website_node == NULL) {
        printf("Failed to allocate memory for website_node\n\n");
        exit(EXIT_FAILURE);
    }

    new_website_node->website = website;
    new_website_node->more_sustainable_website = NULL;

    return new_website_node;
}

/*
WebsiteNode *find_node_index(WebsiteNode **linked_websites, Website *website) {
    WebsiteNode *website_node;
    WebsiteNode *tmp_node;
    
    while (linked_websites[website->category]->website != NULL){
        
        tmp_node = linked_websites[website->category];
        if(linked_websites[website->category]->website->pages_per_minute < website->pages_per_minute && linked_websites[website->category]->more_sustainable_website != NULL) {
            return tmp_node;
        }
        
        
    }
    
    return website_node;        
}
*/

void print_linked_websites(WebsiteNode **linked_websites, int num_categories) {

    for (int i = 0; i < num_categories; i++)
    {
        WebsiteNode *current_node = linked_websites[i];

        printf("%d ", i);
        while (current_node != NULL) {
            printf("%.3f ", current_node->website->pages_per_minute);
            current_node = current_node->more_sustainable_website;
        }
        printf("\n");
    }
    
}

SimulationOutput run_simulation(SimulationInput *simulation_input, User *users, Website *websites, WebsiteNode **linked_websites) {
    SimulationOutput simulation_output;
    short current_day, user_index;

    /* Simulation input constants */
    const int NUM_USERS = simulation_input->num_users;
    const short SIM_DURATION_DAYS = simulation_input->sim_duration_days;
    const short NUM_WEBSITES = simulation_input->num_websites;
    const short NUM_CATEGORIES = simulation_input->num_categories;
    const float SUSTAINABLE_CHOICE = simulation_input->sustainable_choice;

    const short NUM_WEBSITE_ALTERNATIVES = NUM_WEBSITES - NUM_CATEGORIES;


    WebsiteAlternative *website_matrices[NUM_WEBSITE_ALTERNATIVES];
    generate_matrices(website_matrices, linked_websites, NUM_CATEGORIES, NUM_USERS, NUM_WEBSITE_ALTERNATIVES);

    /* TEST Print matrices */
    // for (int i = 0; i < NUM_WEBSITE_ALTERNATIVES; i++) {

    //     for (int j = 0; j < website_matrices[i]->num_x * website_matrices[i]->num_y; j++) {
    //         printf("%d ", website_matrices[i]->matrix[j]);
    //     }
    // }

    /* Main loop - keeps looping until current_day reaches sim_duration_days */
    for (current_day = 0; current_day < SIM_DURATION_DAYS; current_day++) {

        /* loop through all users and call handle_user function */
        for (user_index = 0; user_index < NUM_USERS; user_index++) {

            handle_user(&users[user_index], websites, linked_websites, NUM_WEBSITES, NUM_CATEGORIES, SUSTAINABLE_CHOICE);
        }

        /* Handle output before resetting users */

        /* Resets all users */
        reset_users(users, NUM_USERS);
    }

    /* Calculate total number of pages downloaded */
    simulation_output.total_pages = 0;
    for (int i = 0; i < NUM_USERS; i++) {
        simulation_output.total_pages += users[i].total_pages;

        /* Reset each user's total pages before next simulation */
        users[i].total_pages = 0;
    }

    return simulation_output;
}

void generate_matrices(WebsiteAlternative **website_matrices, WebsiteNode **linked_websites, const short NUM_CATEGORIES, const int NUM_USERS, const short NUM_WEBSITE_ALTERNATIVES) {
    short website_alt_index = 0;

    for (short category_index = ADULT; category_index < NUM_CATEGORIES; category_index++) {
        WebsiteNode *current_node = linked_websites[category_index];
        short num_websites_in_category = 0, website_index = 0;

        /* Calculate number of websites in category */
        while (current_node != NULL) {
            num_websites_in_category++;
            current_node = current_node->more_sustainable_website;
        }

        /* Reset current_node */
        current_node = linked_websites[category_index];

        /* Generate matrix for each website - except the last in each category */
        while (current_node->more_sustainable_website != NULL) {

            WebsiteAlternative* website_alternative = (WebsiteAlternative*) malloc(sizeof(WebsiteAlternative));

            short num_x = (num_websites_in_category - 1) - website_index;
            short num_y = NUM_USERS;

            short *matrix = (short*) malloc(sizeof(short) * num_x * num_y);
            for (int y = 0; y < num_y; y++) {
                for (int x = 0; x < num_x; x++) {
                    short matrix_index = x + y * num_x;
                    matrix[matrix_index] = -1;
                }
            }
            
            /* Assign width and height to the WebsiteAlternative struct */
            website_alternative->num_x = num_x;
            website_alternative->num_y = num_y;

            /* Assign generated matrix to website alternative */
            website_alternative->matrix = matrix;

            /* Assign current website to website alternative */
            current_node->website->alternatives_matrix = website_alternative;

            /* Increment values */
            website_index++;
            website_alt_index++;
            current_node = current_node->more_sustainable_website;
        }
    }
    if (website_alt_index != NUM_WEBSITE_ALTERNATIVES) {
        printf("[ERROR] Number of website alternatives didn't add up (%d != %d)\n", website_alt_index, NUM_WEBSITE_ALTERNATIVES);
    }
}

void print_simulation_output(SimulationOutput *sim_outputs, const short NUM_SIMULATIONS) {
    
    /* Print smt about the simulation - Parameters */

    for (int i = 0; i < NUM_SIMULATIONS; i++) {
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

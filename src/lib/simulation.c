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
        else if (check_key(key, "SUSTAINABLE_CHOICE_INCREMENT")) {
            sim_input.sustainable_choice_increment = value;
        }
        else {
            printf("[WARNING] %s: Invalid parameter detected in simulation_input.txt on line %d (%s). This parameter will be ignored\n\n", __FILE__, i + 1, key);
        }

        i++;
    }

    fclose(fp);

    sim_input.sustainable_choice = 0.0;

    /* Returning simulation input */
    return sim_input;
}

/* Generate user data for the given number of users (from the simulation input file) */
void generate_users(User *users, SimulationInput *sim_input) {
    for (int i = 0; i < sim_input->num_users; i++) {
        users[i].id = i;
        users[i].daily_time_spent = 0;
        users[i].max_daily_time = sim_input->avg_user_time;
        users[i].total_pages = 0;
        users[i].current_website = NULL;
    }
}

/* Load website data from csv file into array of website structs */
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
        websites[i].category_index = 0;
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

/* Convert array of websites to array of linked lists (sorted by pages_per_minute) */
WebsiteNode **convert_websites(Website *websites, SimulationInput *sim_input) {
    WebsiteNode **linked_websites = (WebsiteNode**)malloc(sizeof(WebsiteNode*) * sim_input->num_categories);
    short category_index;

    /* Set all indexes to NULL */
    for (int i = 0; i < sim_input->num_categories; i++) {
        linked_websites[i] = NULL;
    }

    /* Insert a website node for each website in the website array */
    for (int i = 0; i < sim_input->num_websites; i++) {
        insert_website_node(linked_websites, &websites[i]);
    }

    /* Loop through linked_websites and set category_index for all websites */
    for (int i = 0; i < sim_input->num_categories; i++) {
        WebsiteNode *current_website_node = linked_websites[i];
        category_index = 0;
        while (current_website_node != NULL) {
            current_website_node->website->category_index = category_index;
            category_index++;
            current_website_node = current_website_node->next;
        }
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
            current_node = current_node->next;
        }

        new_node = alloc_website_node(website);

        /* Insert at the start of the list */
        if (previous_node == NULL) {
            new_node->next = current_node;
            linked_websites[website->category] = new_node;
        }
        /* Insert at the end of the list */
        else if (current_node == NULL) {
            previous_node->next = new_node;

        }
        /* Insert in between */
        else {
            previous_node->next = new_node;
            new_node->next = current_node;
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
    new_website_node->next = NULL;

    return new_website_node;
}

void print_linked_websites(WebsiteNode **linked_websites, int num_categories) {

    for (int i = 0; i < num_categories; i++)
    {
        WebsiteNode *current_node = linked_websites[i];

        printf("%d ", i);
        while (current_node != NULL) {
            // printf("%.3f ", current_node->website->pages_per_minute);
            printf("%p ", current_node->website->alternatives_matrix);
            current_node = current_node->next;
        }
        printf("\n");
    }
    
}

SimulationOutput run_simulation(SimulationInput *sim_input, User *users, Website *websites, WebsiteNode **linked_websites) {
    SimulationOutput simulation_output;

    /* Simulation input constants */
    const int NUM_USERS = sim_input->num_users;
    const short SIM_DURATION_DAYS = sim_input->sim_duration_days;
    const short NUM_WEBSITES = sim_input->num_websites;
    const short NUM_CATEGORIES = sim_input->num_categories;
    const float SUSTAINABLE_CHOICE = sim_input->sustainable_choice;

    const short NUM_WEBSITE_ALTERNATIVES = NUM_WEBSITES - NUM_CATEGORIES;

    /* Generate a matrix of alternative websites for each website and assign it to the corresponding website */
    generate_matrices(linked_websites, NUM_CATEGORIES, NUM_USERS, NUM_WEBSITE_ALTERNATIVES);

    /* Loop through all days in simulation */
    for (int current_day = 0; current_day < SIM_DURATION_DAYS; current_day++) {
        
        /* Loop through all users */
        for (int user_index = 0; user_index < NUM_USERS; user_index++) {
            handle_user(&users[user_index], websites, linked_websites, NUM_WEBSITES, NUM_CATEGORIES, SUSTAINABLE_CHOICE);
        }

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

void generate_matrices(WebsiteNode **linked_websites, const short NUM_CATEGORIES, const int NUM_USERS, const short NUM_WEBSITE_ALTERNATIVES) {
    short website_alt_index = 0;
    double rand_0_1;

    /* Loop through linked_websites */
    for (int i = ADULT; i < NUM_CATEGORIES; i++) {
        WebsiteNode *current_node = linked_websites[i];
        short num_websites_in_category = 0, website_index = 0;

        /* Calculate number of websites in category */
        while (current_node != NULL) {
            num_websites_in_category++;
            current_node = current_node->next;
        }

        /* Reset current_node */
        current_node = linked_websites[i];

        /* Generate matrix for each website - except the last in each category */
        while (current_node->next != NULL) {

            WebsiteAlternative* website_alternative = (WebsiteAlternative*) malloc(sizeof(WebsiteAlternative));

            /* Calculates the width and height of the matrix */
            short num_x = (num_websites_in_category - 1) - website_index;
            short num_y = NUM_USERS;

            Website **matrix = (Website**) malloc(sizeof(Website*) * num_x * num_y);
            int matrix_index;

            /* Loop through the matrix of the current website */
            for (int y = 0; y < num_y; y++) {
                for (int x = 0; x < num_x; x++) {
                    matrix_index = x + y * num_x;
                    rand_0_1 = (double)rand() / (double)RAND_MAX;

                    /* 50% chance user has made a choice */
                    if (rand_0_1 < 0.5) {
                        /* 25% chance user made a sustainable choice */
                        if (rand_0_1 < 0.25) {
                            WebsiteNode *temp_current_node = current_node->next;
                            short temp_index = 0;
                            /* Makes sure the right website pointer is assigned to the corresponding x value in the matrix */
                            while (temp_index < x) {
                                temp_index++;
                                temp_current_node = temp_current_node->next;
                            }
                            matrix[matrix_index] = temp_current_node->website;
                        }
                        /* 25% chance user has made a non-sustainable choice */
                        else {
                            matrix[matrix_index] = current_node->website;
                        }
                    }
                    /* User has made no choice */
                    else {
                        matrix[matrix_index] = NULL;
                    }
                }
            }
            
            /* Assign width and height to the WebsiteAlternative struct */
            website_alternative->num_x = num_x;
            website_alternative->num_y = num_y;
            website_alternative->num_websites_in_category = num_websites_in_category;

            /* Assign generated matrix to website alternative */
            website_alternative->matrix = matrix;

            /* Assign current website to website alternative */
            current_node->website->alternatives_matrix = website_alternative;

            /* Increment values */
            website_index++;
            website_alt_index++;
            current_node = current_node->next;
        }
        /* Set the alternative matrix of the last website in each category to NULL */
        current_node->website->alternatives_matrix = NULL;
    }
    if (website_alt_index != NUM_WEBSITE_ALTERNATIVES) {
        printf("[ERROR] Number of website alternatives didn't add up (%d != %d)\n", website_alt_index, NUM_WEBSITE_ALTERNATIVES);
    }
}

/* Write simulation output to file */
void write_sim_output(SimulationOutput *sim_outputs, const short NUM_SIMULATIONS, const short SIM_DURATION_DAYS, const float SUSTAINABLE_CHOICE) {
    FILE *fp;
    char file_name[MAX_FILE_NAME_LEN] = "output/output_";

    /* Create file name based on the value of SUSTAINABLE_CHOICE */
    create_file_name(file_name, SUSTAINABLE_CHOICE);

    fp = fopen(file_name, "w");
    if (fp == NULL) {
        printf("[ERROR] Unable to write to output file: %s\n\n", file_name);
        exit(EXIT_FAILURE);
    }

    /* Write output for all simulations */
    fprintf(fp, "SUSTAINABLE CHOICE = %.2f\nSIMULATION;TOTAL_PAGES\n", SUSTAINABLE_CHOICE);
    for (int i = 0; i < NUM_SIMULATIONS; i++) {
        fprintf(fp, "%d;%.2f\n", i+1, sim_outputs[i].total_pages);
    }

    fclose(fp);
}

/* Create file name based on the value of SUSTAINABLE_CHOICE */
void create_file_name(char file_name[MAX_FILE_NAME_LEN], const float SUSTAINABLE_CHOICE) {
    char sus_choice[20];

    sprintf(sus_choice, "%.2f", SUSTAINABLE_CHOICE);
    strcat(file_name, sus_choice);
    strcat(file_name, ".csv");
}

/* Combine output files */
void combine_output_files(short num_simulations, const short num_files) {
    FILE *fps[num_files];
    FILE *out_fp;
    char file_name[MAX_FILE_NAME_LEN];
    float output_val;
    int clear_index;

    /* Open all files */
    for (int i = 0; i < num_files; i++) {
        sprintf(file_name, "output/output_%.2f.csv", (float)i / (num_files - 1));
        FILE *fp = fopen(file_name, "r");

        if (fp == NULL) {
            printf("[ERROR] Failed to open file (combine_output_files)");
            exit(1);
        }

        fps[i] = fp;
    }

    /* Skip first two lines in all files */
    for (int i = 0; i < num_files; i++) {
        while (fgetc(fps[i]) != '\n');
        while (fgetc(fps[i]) != '\n');
    }

    /* print to output_all */
    out_fp = fopen("output/output_all.csv", "w");
    if (out_fp == NULL) {
        printf("[ERROR] Failed to open file (combine_output_files)");
        exit(1);
    }
    
    for (int i = 0; i < num_simulations; i++) {
        /* Skip index of all files */
        for (int i = 0; i < num_files; i++) {
            fscanf(fps[i], "%d;", &clear_index);
        }
        
        /* Put the next value from each file into the output file */
        for (int j = 0; j < num_files; j++) {
            fscanf(fps[j], "%f ;", &output_val);

            fprintf(out_fp, "%.2f;", output_val);
        }
        
        fprintf(out_fp, "\n");
    }

    /* close all files */
    for (int i = 0; i < num_files; i++) {
        fclose(fps[i]);
    }
}

void free_all(WebsiteNode **linked_websites, short num_categories) {
    for (int i = 0; i < num_categories; i++) {
        WebsiteNode *current_node = linked_websites[i];
        WebsiteNode *temp_current_node;

        while (current_node->next != NULL) {

            temp_current_node = current_node;

            current_node = current_node->next;

            free(temp_current_node->website->alternatives_matrix->matrix);
            free(temp_current_node->website->alternatives_matrix);
            free(temp_current_node);
        }
        free(current_node); /* Free the last element in the linked list */
    }
    
    free(linked_websites);
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

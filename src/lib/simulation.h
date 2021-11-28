#ifndef SIMULATION
#define SIMULATION

struct SimulationInput {
    unsigned short num_websites;
    unsigned short num_users;
    unsigned short avg_user_time;
    unsigned short time_increment;
    unsigned int sim_duration;      
};
typedef struct SimulationInput SimulationInput;

int check_key(char *key, char *valid_key);
void print_sim_input(SimulationInput *sim_input);
SimulationInput get_sim_input(void); /* Read the input file */

void run_simulation(SimulationInput *sim_input);

void print_simulation_output();

#endif




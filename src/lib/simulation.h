#ifndef SIMULATION
#define SIMULATION

#define SIM_INPUT_LENGTH 60

#define ERROR_COLOR "\033[0;31m"
#define DEFAULT_COLOR "\033[0m"
#define WARNING_COLOR "\033[0;33m"

struct InputData {
    char key[SIM_INPUT_LENGTH];
    int val;
};
typedef struct InputData InputData;

struct SimulationInput {
    unsigned short num_websites;
    unsigned short num_users;
    unsigned short avg_user_time;
    unsigned short time_increment;
    unsigned int sim_duration;      
};
typedef struct SimulationInput SimulationInput;

int get_lines_in_file(FILE *fp);
int check_key(char *key, char *valid_key);
void print_sim_input(SimulationInput *sim_input);
SimulationInput get_sim_input(void); /* Read the input file */

void run_simulation(SimulationInput *sim_input);

void print_simulation_output();

#endif




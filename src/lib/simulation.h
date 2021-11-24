#ifndef SIMULATION
#define SIMULATION

struct SimulationInput {
    unsigned short num_users;
    unsigned short avg_user_time;
    unsigned short time_increment;
    unsigned int sim_duration;
};
typedef struct SimulationInput SimulationInput;

void run_simulation(SimulationInput *sim_input);

void print_simulation(...);

#endif




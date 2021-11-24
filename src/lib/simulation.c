#include "simulation.h"

SimulationInput get_sim_input() {
    SimulationInput sim_input = {10, 3600, 10, 7200}; /* num_users, avg_user_time, time_increment, sim_duration */
    return sim_input;
}
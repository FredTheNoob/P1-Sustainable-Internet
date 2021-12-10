/* 
 *  Title:          Sustainable Webbrowsing
 *  Group:          SW B222
 *  Names:          Ali, Alexander, Viktor, Frederik, Thorbjørn & Jonas.
 *  Supervisor:     Morten Konggard Schou
 *  Initial commit: 23/11-2021
 *  Date completed: ??/12-2021
 *  Description:
 *      Add description here ...
 *      ....
 *      ....
 */

#include "lib/simulation.h"

int main(void) {
    /* Seed random generator */
    srand(time(NULL));

    /* Simulation input */
    SimulationInput sim_input = get_sim_input();

    /* Create array of users */
    User users[sim_input.num_users];
    generate_users(users, &sim_input);

    /* Create array of websites */
    Website websites[sim_input.num_websites];
    load_websites(websites, &sim_input);

    /* Convert array of websites to array of linked lists (sorted by pages_per_minute) */
    WebsiteNode *linked_websites[sim_input.num_categories];
    convert_websites(linked_websites, websites, &sim_input);

    /* Array of all simulation outputs */
    SimulationOutput sim_outputs[sim_input.num_simulations];

    /* Run all simulations */
    for (int i = 0; i < sim_input.num_simulations; i++) {
        
        sim_outputs[i] = run_simulation(&sim_input, users, websites, linked_websites);
    }

    print_simulation_output(sim_outputs, sim_input.num_simulations);

    printf("Reached end of program\n");

    return EXIT_SUCCESS;
}

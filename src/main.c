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

    /* Load website data */
    /* ... */

    /* Convert to array of linked lists */
    /* ... */

    /* Array of all simulation outputs */
    SimulationOutput sim_outputs[sim_input.num_simulations];

    /* Run all simulations */
    for (int i = 0; i < sim_input.num_simulations; i++) {
        
        sim_outputs[i] = run_simulation(&sim_input);
    }

    print_simulation_output(sim_outputs, sim_input.num_simulations);

    printf("Reached end of program\n");

    return EXIT_SUCCESS;
}
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
#include <time.h>

int main(void) {
    clock_t curr_sim_start, curr_sim_end, total_sim_start, total_sim_end;

    /* Seed random generator */
    srand(time(NULL));

    /* Simulation input */
    SimulationInput sim_input = get_sim_input();

    /* Calculate how many output files to make */
    short num_files = (1 / sim_input.sustainable_choice_increment) + 1;

    total_sim_start = clock();

    for (int n = 0; n < num_files; n++) {

        /* Declare array of all simulation outputs */
        SimulationOutput sim_outputs[sim_input.num_simulations];

        /* Create array of users */
        User users[sim_input.num_users];
        generate_users(users, &sim_input);

        /* Create array of websites */
        Website websites[sim_input.num_websites];
        load_websites(websites, &sim_input);

        /* Convert array of websites to array of linked lists (sorted by pages_per_minute) */
        WebsiteNode **linked_websites = convert_websites(websites, &sim_input);

        curr_sim_start = clock(); /* Start the timer */

        /* Run all simulations */
        for (int i = 0; i < sim_input.num_simulations; i++) {
            
            sim_outputs[i] = run_simulation(&sim_input, users, websites, linked_websites);
        }

        curr_sim_end = clock(); /* Stop the timer */

        /* Write simulation output to file */
        write_sim_output(sim_outputs, sim_input.num_simulations, sim_input.sim_duration_days, sim_input.sustainable_choice);
        
        printf("Simulation no. %d / %d   SUSTAINABLE_CHOICE = %.2f   Took %.3f s\n", n + 1, num_files, sim_input.sustainable_choice, (double)(curr_sim_end - curr_sim_start) / (double)CLOCKS_PER_SEC);
        sim_input.sustainable_choice += sim_input.sustainable_choice_increment;

        /* Free matrices + WebsiteAlternatives + WebsiteNodes + linked_websites (array of pointers) */
        /* Websites have not been allocated dynamically */
        free_all(linked_websites, sim_input.num_categories);
    }

    total_sim_end = clock();

    combine_output_files(sim_input.num_simulations, num_files);

    double total_sim_time = (double)(total_sim_end - total_sim_start) / (double)CLOCKS_PER_SEC;
    printf("Finished simulation in a total of %.3f seconds (%.3f minutes)\n", total_sim_time, total_sim_time / 60);
    
    return EXIT_SUCCESS;
}

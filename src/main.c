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
    clock_t start_t, end_t;

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
    WebsiteNode **linked_websites = convert_websites(websites, &sim_input);

    /* Array of all simulation outputs */
    SimulationOutput sim_outputs[sim_input.num_simulations];


    start_t = clock();
    /* Run all simulations */
    for (int i = 0; i < sim_input.num_simulations; i++) {
        
        sim_outputs[i] = run_simulation(&sim_input, users, websites, linked_websites);
    }
    end_t = clock();

    /* TEST PRINT FOR FIRST WEBSITE'S MATRIX */
    for (int y = 0; y < linked_websites[0]->website->alternatives_matrix->num_y; y++) {
        printf("%p\t", &users[y]);
        for (int x = 0; x < linked_websites[0]->website->alternatives_matrix->num_x; x++) {
            int index = x + y * linked_websites[0]->website->alternatives_matrix->num_x;
            printf("%-15p  ", linked_websites[0]->website->alternatives_matrix->matrix[index]);

        }
        printf("\n");
    }
    printf("\n%p\n", linked_websites[0]->website);
    printf("\n%p\n", linked_websites[0]->next->website);
    printf("\n%p\n", linked_websites[0]->next->next->website);



    // print_linked_websites(linked_websites, sim_input.num_categories);
    // print_sim_output(sim_outputs, sim_input.num_simulations, sim_input.sim_duration_days, sim_input.sustainable_choice);

    printf("Reached end of program in %.3f seconds\n", (double)(end_t - start_t) / (double)CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}

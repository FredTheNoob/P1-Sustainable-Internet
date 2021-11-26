/* 
 *  Title:          Sustainable Webbrowsing
 *  Group:          SW B222
 *  Names:          Andreas, Ali, Alexander, Viktor, Frederik, Thorbjørn & Jonas.
 *  Supervisor:     Morten Konggard Schou
 *  Initial commit: 23/11-2021
 *  Date completed: ??/12-2021
 *  Description:
 *      Add description here ...
 *      ....
 *      ....
 */

#include <stdio.h>
#include <stdlib.h>

#include "lib/simulation.h"

int main(void){

    SimulationInput sim_input = get_sim_input();

    run_simulation(&sim_input);


    printf("Reached end of program\n");
    return EXIT_SUCCESS;
}
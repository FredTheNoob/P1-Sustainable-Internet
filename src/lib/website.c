#include "website.h"

Website *get_website(Website *websites, unsigned int websites_length, short previous_website_id) {
    /* Roll a dice between 0 and 100 */
    int roll = rand() % 100;

    /* Loop over websites */
    int curr_range = 0;
    for (int i = 0; i < websites_length; i++) {
        if (roll > curr_range && roll <= websites[i].influence + curr_range
            && websites[i].id != previous_website_id) {
            return &websites[i];
        }   
        else {
            curr_range += websites[i].influence;
        }
    }
}
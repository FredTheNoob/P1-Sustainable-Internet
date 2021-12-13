#include "user.h"

#define NO_WEBSITE_ID -1

void reset_users(User *users, const int NUM_USERS) {
    for (int i = 0; i < NUM_USERS; i++) {
        users[i].total_daily_time = 0;
        users[i].current_website = NULL;
    }
}

/* Logic to conrol whether a user should be assigned a new website */
void handle_user(User *user, Website *websites, WebsiteNode **linked_websites, const short NUM_WEBSITES, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE) { 
    
    Website *website = NULL;
    Website *sustainable_website = NULL;
    Website *chosen_website = NULL;

    bool below_daily_time = true;

    /* If user's daily time + their current website's avg duration doesn't exceed their max daily time */
    while (below_daily_time) {
        if (user->current_website == NULL) {
            website = get_website(websites, NUM_WEBSITES, NO_WEBSITE_ID);
        } else {
            website = get_website(websites, NUM_WEBSITES, user->current_website->id); /* Returns a new website */
        }

        /* Find out if it is possible to recommend a website to the user, which is 
        more sustainable than the most sustainable website that the user has already
        accepted as an alternative to this specific website */
        /* FUNCTION CALL */

            
        /* Recommend a more sustainable website based on the initial website */
        sustainable_website = recommend_website(linked_websites, website, user->id, NUM_CATEGORIES);

        /* Logic to control which website to choose - the sustainable alternative or the initial website */
        chosen_website = choose_website(website, sustainable_website, SUSTAINABLE_CHOICE);
        
        /* Assign the chosen website to user */
        assign_website(user, chosen_website);


        if (user->total_daily_time + user->current_website->avg_duration < user->max_daily_time) {

            /* Increment user's total daily time spent on webbrowsing */
            user->total_daily_time += user->current_website->avg_duration;

            /* Increment user's total pages downloaded during webbrowsing */
            user->total_pages += user->current_website->pages_per_visit;

        } else {
            below_daily_time = false;
        }
    }
    
    /* Calculates the remaining pages */
    float remaining_pages = (float)(user->max_daily_time - user->total_daily_time) / 
                            (float)(user->current_website->avg_duration * 
                            user->current_website->pages_per_visit); 

    user->total_pages += remaining_pages;
}

/* Assign website to user */
void assign_website(User *user, Website *chosen_website) {
    /* Gets a single website from the website data and assigns it to the given user's current website */
    user->current_website = chosen_website;
} 

Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short user_id, const short NUM_CATEGORIES) {
    int index, num_total_interactions;

    int user_index = user_id;

    int most_similar_user_id = -1;
    float most_similar_jaccard = 0, current_jaccard;
    
    int num_common_interactions = 0;

    int num_users = current_website->alternatives_matrix->num_y;
    int num_alternatives_in_category = current_website->alternatives_matrix->num_x;
    short *alternative_matrix = current_website->alternatives_matrix->matrix;

    for (int y = 0; y < num_users; y++) {
        /* (x) + y * width */
        index = y * num_alternatives_in_category;
        num_common_interactions = 0;
        num_total_interactions = 0;
        for (int x = 0; x < num_alternatives_in_category; x++) {
            /* Make sure that the user itself is skipped */
            if (index == user_index) {
                continue;
            }
            /* If both values are -1, skip to the next iteration in the for loop */
            else if (alternative_matrix[index + x] == -1 && alternative_matrix[user_index + x] == -1) {
                continue;
            }
            /* Else if both values are the same, increment num_common_interactions */
            else if (alternative_matrix[index + x] == alternative_matrix[user_index + x]) {
                num_common_interactions++;
            }
            /* If one of the values are not -1, increment num_total_interactions */
            if (alternative_matrix[index + x] != -1 || alternative_matrix[user_index + x] != -1) {
                num_total_interactions++;
            }
        }
        
        /* Calculate similarity for user */
        current_jaccard = num_common_interactions / num_total_interactions;

        if (current_jaccard > most_similar_jaccard) {
            most_similar_user_id = y;
            most_similar_jaccard = current_jaccard;
        }
    }
    

    
    
    /* return recommend_website; */
    return current_website;
}

Website *choose_website(Website *website, Website *sustainable_website, const float SUSTAINABLE_CHOICE) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;

    /* Update  */
    
    return rand_0_1 < SUSTAINABLE_CHOICE ? sustainable_website : website;
}

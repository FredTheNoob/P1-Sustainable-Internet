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

        /* If the website's matrix == NULL it is the most sustainable website in the category */
        if (website->alternatives_matrix == NULL) {
            chosen_website = website;
        } else {
            /* Recommend a more sustainable website based on the initial website */
            sustainable_website = recommend_website(linked_websites, website, user->id, NUM_CATEGORIES);

            if (sustainable_website == website) {
                chosen_website = website;
            } else {
                /* Logic to control which website to choose - the sustainable alternative or the initial website */
                chosen_website = choose_website(website, sustainable_website, user->id, SUSTAINABLE_CHOICE);
            }
        }
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

/* Return a website to recommend the user */
Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short user_id, const short NUM_CATEGORIES) {
    Website *recommended_website = NULL;
    int num_total_interactions, num_common_interactions;
    int user_index = user_id, similar_user_index;

    int most_similar_user_id = -1;
    float most_similar_jaccard = 0, current_jaccard;
    
    /* Number of users and websites in the current website's matrix of alternative websites */
    int num_users = current_website->alternatives_matrix->num_y;
    int num_alternatives_in_category = current_website->alternatives_matrix->num_x;

    Website **alternative_matrix = current_website->alternatives_matrix->matrix;

    for (int y = 0; y < num_users; y++) {
        /* (x) + y * width */
        similar_user_index = y * num_alternatives_in_category;
        num_common_interactions = 0;
        num_total_interactions = 0;
        for (int x = 0; x < num_alternatives_in_category; x++) {
            /* Make sure that the user itself is skipped */
            if (similar_user_index == user_index) {
                continue;
            }
            /* If both values are NULL, skip to the next iteration in the for loop */
            else if (alternative_matrix[similar_user_index + x] == NULL && alternative_matrix[user_index + x] == NULL) {
                continue;
            }
            /* Else if both values are the same, increment num_common_interactions */
            else if (alternative_matrix[similar_user_index + x] == alternative_matrix[user_index + x]) {
                num_common_interactions++;
            }
            /* If one of the values are not -1, increment num_total_interactions */
            if (alternative_matrix[similar_user_index + x] != NULL || alternative_matrix[user_index + x] != NULL) {
                num_total_interactions++;
            }
        }
        
        /* Calculate similarity for user */
        current_jaccard = num_total_interactions > 0 ? num_common_interactions / num_total_interactions : 0;

        if (current_jaccard > most_similar_jaccard) {
            most_similar_user_id = y;
            most_similar_jaccard = current_jaccard;
        }
    }

    /* If most_similar_user_id hasn't been updated from -1, the algortithm hasn't 
    found proper recommendation - therefore return the current_website */
    if (most_similar_user_id == -1) {
        return current_website;
    }

    /* Loop through the matrix at index 'user_id' and 'most_similar_user_id' and compare */
    short compare_index = num_alternatives_in_category - 1;
    bool found_alternative = false;

    similar_user_index = most_similar_user_id * num_alternatives_in_category;

    while (compare_index >= 0 && !found_alternative) {

        /* If the user has accepted a more sustainable website than the similar user can recommend, just choose that website */
        if (alternative_matrix[user_index + compare_index] != NULL && alternative_matrix[user_index + compare_index] != current_website) {
            recommended_website = alternative_matrix[user_index + compare_index];
            found_alternative = true;
        }
        /* If the similar user has accepted a website that the user hasn't interacted with yet, recommend that website */
        else if (alternative_matrix[similar_user_index + compare_index] != NULL && 
                 alternative_matrix[similar_user_index + compare_index] != current_website && 
                 alternative_matrix[user_index + compare_index] == NULL) {
            
            recommended_website = alternative_matrix[similar_user_index + compare_index];
            found_alternative = true;
        }

        compare_index--;
    }
    
    /* If no recommendation was found, return the current website */
    return recommended_website != NULL ? recommended_website : current_website;
}

Website *choose_website(Website *website, Website *sustainable_website, short user_id, const float SUSTAINABLE_CHOICE) {
    /* Generate random number between 0 and 1 */
    double rand_0_1 = (double)rand() / (double)RAND_MAX;
    Website *chosen_website = NULL;
    Website **matrix;
    short num_alternatives_in_category, num_websites_in_category, first_alternative_category_index, sus_website_index, matrix_index;
    short matrix_x, matrix_y, matrix_width;

    /* Make shorter local variables */
    matrix = website->alternatives_matrix->matrix;
    num_alternatives_in_category = website->alternatives_matrix->num_x;
    num_websites_in_category = website->alternatives_matrix->num_websites_in_category;

    /* Calculate category index of the first website in the matrix of alternatives */
    first_alternative_category_index = num_websites_in_category - num_alternatives_in_category;
    
    /* Calculate the index (on the x-axis) that the sustainable website has in the matrix */
    sus_website_index = sustainable_website->category_index - first_alternative_category_index;

    /* Calculate the index (overall) that the sustainable website has in the matrix */
    matrix_x = sus_website_index;
    matrix_y = user_id;
    matrix_width = num_alternatives_in_category;

    matrix_index = matrix_x + matrix_y * matrix_width;

    /* Check if the user should accept the sustainable website or not */
    if (rand_0_1 < SUSTAINABLE_CHOICE) {
        /* Update the pointer in the matrix and choose the sustainable website */
        matrix[matrix_index] = sustainable_website;
        chosen_website = sustainable_website;
    } else {
        /* Update the pointer in the matrix and choose the original website */
        matrix[matrix_index] = website;
        chosen_website = website;
    }

    return chosen_website;
}

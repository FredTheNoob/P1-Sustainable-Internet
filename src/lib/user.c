#include "user.h"

/* Reset users */
void reset_users(User *users, const int NUM_USERS) {
    for (int i = 0; i < NUM_USERS; i++) {
        users[i].daily_time_spent = 0;
        users[i].current_website = NULL;
    }
}

/* Simulates a users behaviour throughout one day */
void handle_user(User *user, Website *websites, WebsiteNode **linked_websites, const short NUM_WEBSITES, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE) { 
    
    Website *website = NULL;
    Website *sustainable_website = NULL;
    Website *chosen_website = NULL;

    bool below_daily_time = true;

    /* Check if the user is below its max daily time */
    while (below_daily_time) {
        if (user->current_website == NULL) {
            /* Returns the first website of the day */
            website = get_website(websites, NO_WEBSITE_ID); 
        } 
        else {
            /* Returns a new website */
            website = get_website(websites, user->current_website->id); 
        }

        /* If the website's matrix == NULL it is the most sustainable website in the category */
        if (website->alternatives_matrix == NULL) {
            chosen_website = website;
        } 
        else {
            /* Recommend a more sustainable website based on the current website */
            sustainable_website = recommend_website(linked_websites, website, user->id, NUM_CATEGORIES, SUSTAINABLE_CHOICE);

            if (sustainable_website == website) {
                chosen_website = website;
            } 
            else {
                /* Loop through all alternatives for the user and check 
                if the recommended website has already been chosen */
                Website **matrix = website->alternatives_matrix->matrix;
                short num_x = website->alternatives_matrix->num_x;
                int matrix_index = user->id * num_x, i = 0;
                bool found_recommended_website = false;

                while (!found_recommended_website && i < num_x) {
                    if (matrix[matrix_index + i] == sustainable_website) {
                        chosen_website = sustainable_website;
                        found_recommended_website = true;
                    }
                    i++;
                }

                if (!found_recommended_website) {
                    /* Check which website to choose - the sustainable alternative or the current website */
                    chosen_website = choose_website(website, sustainable_website, user->id, SUSTAINABLE_CHOICE);
                }   
            }
        }

        /* Assign the chosen website to user */
        assign_website(user, chosen_website);

        /* Check if the users max daily time is exceeded if the avg duration of the website is added */
        if (user->daily_time_spent + user->current_website->avg_duration < user->max_daily_time) {

            /* Increment user's daily time spent on webbrowsing */
            user->daily_time_spent += user->current_website->avg_duration;

            /* Increment user's total pages downloaded during webbrowsing */
            user->total_pages += user->current_website->pages_per_visit;
        } 
        else {
            below_daily_time = false;
        }
    }
    
    /* Calculates the remaining pages and adds them to the user */
    float remaining_pages = (float)(user->max_daily_time - user->daily_time_spent) / 
                            (float)(user->current_website->avg_duration * 
                            user->current_website->pages_per_visit); 

    user->total_pages += remaining_pages;
}

/* Assign website to user */
void assign_website(User *user, Website *chosen_website) {    
    user->current_website = chosen_website;
} 

/* Return a website to recommend the user */
Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short user_id, const short NUM_CATEGORIES, const float SUSTAINABLE_CHOICE) {
    
    Website *recommended_website = NULL;

    Website *fallback_website = current_website;
    short fallback_index, num_websites_in_category = current_website->alternatives_matrix->num_websites_in_category;

    int num_total_interactions, num_common_interactions;

    int most_similar_user_id = NO_USER_ID;
    float most_similar_jaccard = 0, current_jaccard;
    
    /* Number of users and websites in the current website's matrix of alternative websites */
    int num_users = current_website->alternatives_matrix->num_y;
    int num_alternatives_in_category = current_website->alternatives_matrix->num_x;

    int user_index = user_id * num_alternatives_in_category;
    int similar_user_index;

    Website **matrix = current_website->alternatives_matrix->matrix;

    /* Loop through all users in the matrix */
    for (int y = 0; y < num_users; y++) {
        /* (x) + y * width */
        similar_user_index = y * num_alternatives_in_category;
        num_common_interactions = 0;
        num_total_interactions = 0;

        /* Make sure that the user itself is skipped */
        if (similar_user_index == user_index) {
            continue;
        }

        /* Loop through all websites and compare the inputted user (user_id) and the 'y'th user */
        for (int x = 0; x < num_alternatives_in_category; x++) {
            
            /* If both values are NULL, skip to the next iteration in the for loop */
            if (matrix[similar_user_index + x] == NULL && matrix[user_index + x] == NULL) {
                continue;
            }
            /* Else if both values are the same, increment num_common_interactions */
            else if (matrix[similar_user_index + x] == matrix[user_index + x]) {
                num_common_interactions++;
                num_total_interactions++;
            }
            /* If one of the values are not NULL, increment num_total_interactions */
            else {
                num_total_interactions++;
            }
        }

        /* Calculate Jaccard similarity coefficient for current user */
        current_jaccard = num_total_interactions > 0 ? num_common_interactions / num_total_interactions : 0;

        /* Replace most similar user if the similirarity is greater than the current most similar user */
        if (current_jaccard >= most_similar_jaccard) {
            most_similar_user_id = y;
            most_similar_jaccard = current_jaccard;
        }
    }

    short compare_index = num_alternatives_in_category - 1;
    bool found_alternative = false;

    similar_user_index = most_similar_user_id * num_alternatives_in_category;

    /* Loop through the matrix at index 'user_id' and 'most_similar_user_id' and compare */
    while (compare_index >= 0 && !found_alternative) {

        /* If the user has accepted a more sustainable website than the similar user can recommend, just choose that website */
        if (matrix[user_index + compare_index] != NULL && matrix[user_index + compare_index] != current_website) {
            recommended_website = matrix[user_index + compare_index];
            found_alternative = true;
        }
        /* If the similar user has accepted a website that the user hasn't interacted with yet, recommend that website */
        else if (matrix[similar_user_index + compare_index] != NULL && 
                 matrix[similar_user_index + compare_index] != current_website && 
                 matrix[user_index + compare_index] == NULL) {
            
            recommended_website = matrix[similar_user_index + compare_index];
            found_alternative = true;
        }
        /* Create a fallback website in case no recommendation was found. 
        Recommend the most sustainable website that the user hasn't interacted with yet */
        else if (matrix[user_index + compare_index] == NULL) {

            WebsiteCategory website_category = current_website->category;
            WebsiteNode *current_node = linked_websites[website_category];

            fallback_index = num_websites_in_category - num_alternatives_in_category + compare_index;
            
            for (int i = 0; i < fallback_index; i++) {
                current_node = current_node->next;    
            }
            fallback_website = current_node->website;
        }

        compare_index--;
    }
    
    /* If no recommendation was found, return the fallback website */
    return recommended_website != NULL ? recommended_website : fallback_website;
}

/* Control which website to choose */
Website *choose_website(Website *website, Website *sustainable_website, short user_id, const float SUSTAINABLE_CHOICE) {
    /* Generate random number between 0 and 1 */
    float rand_0_1 = (float)rand() / (float)RAND_MAX;
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
    } 
    else {
        /* Update the pointer in the matrix and choose the original website */
        matrix[matrix_index] = website;
        chosen_website = website;
    }

    return chosen_website;
}

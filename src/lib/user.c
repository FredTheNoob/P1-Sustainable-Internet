#include "user.h"

#define NO_WEBSITE_ID -1

void reset_users(User *users, unsigned short num_users) {
    for (int i = 0; i < num_users; i++) {
        users[i].total_daily_time = 0;
        users[i].current_website = NULL;
    }
}

/* Logic to conrol whether a user should be assigned a new website */
void handle_user(User *user, Website *websites, WebsiteNode **linked_websites, unsigned short num_websites, short num_categories, float sustainable_choice) { 
    
    Website *website = NULL;
    Website *sustainable_website = NULL;
    Website *chosen_website = NULL;

    do {
        if (user->current_website == NULL) {
            website = get_website(websites, num_websites, NO_WEBSITE_ID); /* Returns the users first website */
        } else {
            website = get_website(websites, num_websites, user->current_website->id); /* Returns a new website */
        }
        
        /* Recommend a more sustainable website based on the initial website */
        sustainable_website = recommend_website(linked_websites, website, num_categories);

        /* Logic to control which website to choose - the sustainable alternative or the initial website */
        chosen_website = choose_website(website, sustainable_website, sustainable_choice);
        
        /* Assign the chosen website to user */
        assign_website(user, chosen_website);

        user->total_daily_time += user->current_website->avg_duration;

        user->total_pages += user->current_website->pages_per_visit;
        
        /* If user's daily time + their current website's avg duration doesn't exceed their max daily time */
    } while (user->total_daily_time + user->current_website->avg_duration < user->max_daily_time);
    
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

Website *recommend_website(WebsiteNode **linked_websites, Website *current_website, short num_categories) {
    Website *recommended_website = NULL;
    
    while (current_website->category) {

    }

    return recommended_website;
}

Website *choose_website(Website *website, Website *sustainable_website, float sustainable_choice) {
    Website *chosen_website = NULL;
    
    /* If sustaianable choice == 0.00 */
    /* chosen_website = website; */

    /* Write the function here */

    return chosen_website;
}

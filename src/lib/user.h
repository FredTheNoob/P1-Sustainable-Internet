#ifndef USER
#define USER

#include "website.h"

struct User {
    unsigned int current_time;
    unsigned int max_daily_time;
    Website *current_website;
};
typedef struct User User;

#endif
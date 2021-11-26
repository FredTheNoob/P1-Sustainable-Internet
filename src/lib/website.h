#ifndef WEBSITE
#define WEBSITE

#include "simulation.h"

struct Website {
    short id;
    unsigned short avg_duration;
    float pages_per_visit;
    float influence;
};
typedef struct Website Website;

Website *get_website(Website *websites, unsigned int num_websites, short previous_website_id);
Website *get_website_v2(Website *websites, unsigned int num_websites, short previous_website_id);

void load_websites(Website *websites, SimulationInput *sim_input);

#endif

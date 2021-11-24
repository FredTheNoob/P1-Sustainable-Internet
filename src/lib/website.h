#ifndef WEBSITE
#define WEBSITE

struct Website {
    short id;
    unsigned short avg_duration;
    float pages_per_visit;
    float influence;
};
typedef struct Website Website;

Website *get_website(Website *websites, unsigned int websites_length, short previous_website_id);

#endif

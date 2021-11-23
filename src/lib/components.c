struct website;
struct website_size;
struct server;

struct website {
    char* name;
    char* url;
    website_size size;
    unsigned int findability;
    server server;
};
typedef struct website website;

struct website_size {
    unsigned int js;
    unsigned int css;
    unsigned int fonts;
    unsigned int images;
    unsigned int media;
    unsigned int html;
    unsigned int total;
};
typedef struct website_size website_size;

struct server {
    unsigned int green_score;
    double pue;
};
typedef struct server server;

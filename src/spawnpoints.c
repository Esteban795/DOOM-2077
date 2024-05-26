#include "../include/spawnpoints.h"
#include <time.h>

const spawnpoint SPAWNPOINTS[SPAWNPOINTS_COUNT] = {
    (spawnpoint){.x =-3072, .y = 768,.z = 264,.angle = 45.0},   
    (spawnpoint){.x = -1792, .y = -64,.z = 280,.angle = 0.0},
    (spawnpoint){.x = -1216, .y = -1344,.z = 8, .angle = 225.0}, 
    (spawnpoint){.x = -256, .y = -832,.z = 8, .angle = 90.0},
    (spawnpoint){.x = 256, .y = -832,.z = 8, .angle = 270.0},    
    (spawnpoint){.x = 1216, .y = 1344,.z = 8, .angle = 45.0},
    (spawnpoint){.x = 1792, .y = 64,.z = 280,.angle = 180.0},
    (spawnpoint){.x = 3072, .y = -768,.z = 264,.angle = 225.0}
};

time_t t;

spawnpoint get_random_spawnpoint() {
    srand((unsigned) time(&t)); // srand(time(NULL)
    int r = rand();
    return SPAWNPOINTS[r % SPAWNPOINTS_COUNT];
}
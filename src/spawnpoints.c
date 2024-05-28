#include "../include/spawnpoints.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

spawnpoint SPAWNPOINTS[SPAWNPOINTS_COUNT] = {
    {-3072,768,264,45.0},   
    {-1792,  -64, 280, 0.0},
    { -1216,  -1344, 8,  225.0}, 
    { -256,  -832, 8,  90.0},
    { 256,  -832, 8,  270.0},    
    { 1216,  1344, 8,  45.0},
    { 1792,  64, 280, 180.0},
    { 3072,  -768, 264, 225.0}
};

time_t t;

spawnpoint get_random_spawnpoint() {
    static bool initialized = false;
    if (!initialized) {
        srand((unsigned) time(&t)); // srand(time(NULL)
        initialized = true;
    }
    int r = rand();
    return SPAWNPOINTS[r % SPAWNPOINTS_COUNT];
}
#ifndef SPAWNPOINTS_H
#define SPAWNPOINTS_H

#include "vec2.h"
#include <stdlib.h>
#include <time.h>


#define SPAWNPOINTS_COUNT 8

struct Spawnpoint {
    double x;
    double y;
    double z;
    double angle;
};

typedef struct Spawnpoint spawnpoint;

extern const spawnpoint SPAWNPOINTS[SPAWNPOINTS_COUNT];

spawnpoint get_random_spawnpoint();
#endif
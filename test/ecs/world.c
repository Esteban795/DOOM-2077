#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/ecs/archetype.h"
#include "../../include/ecs/world.h"

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_world(void) {
    int failed = 0;
    printf("Testing libecs/world...\n");

    printf("libecs/world tests finished with %d failures.\n", failed);
    return failed;
}
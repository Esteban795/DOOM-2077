#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_archetype(void);
int test_world(void);

int main(void)
{
    int failed = 0;
    printf("Testing libecs...\n");
    failed += test_archetype();
    failed += test_world();
    printf("libecs tests finished with %d failures.\n", failed);
    return failed;
}

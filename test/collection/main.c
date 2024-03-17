#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_vec(void);

int main(void)
{
    int failed = 0;
    printf("Testing libcollection...\n");
    failed += test_vec();
    printf("libcollection tests finished with %d failures.\n", failed);
    return failed;
}

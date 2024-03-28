#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/collection/vec.h"

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_vec_init() {
    printf("collection::vec::vec_init...\n");
    vec_t vec;
    vec_init(&vec);
    ASSERT(vec.capacity == VEC_INIT_CAPACITY, "vec_init: capacity is not equal to VEC_INIT_CAPACITY");
    ASSERT(vec.length == 0, "vec_init: length is not 0");
    ASSERT(vec.data != NULL, "vec_init: data is NULL");
    free(vec.data);
    return 0;
}

int test_vec_destroy() {
    printf("collection::vec::vec_destroy...\n");
    vec_t vec;
    vec_init(&vec);
    vec_destroy(&vec, false);
    ASSERT(vec.capacity == 0, "vec_destroy: capacity is not 0");
    return 0;
}

int test_vec_push_one() {
    printf("collection::vec::vec_push_one...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    vec_push(&vec, a);
    ASSERT(vec.length <= vec.capacity, "vec_push: length is not less than (or equal to) capacity");
    ASSERT(vec.length == 1, "vec_push: length is not 1");
    ASSERT(vec.data[0] == a, "vec_push: data[0] is not a");
    ASSERT(*(int*)vec.data[0] == 1, "vec_push: data[0] is not 1");
    free(vec.data[0]);
    free(vec.data);
    return 0;
}
 
int test_vec_push_many() {
    printf("collection::vec::vec_push_many...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    int* b = malloc(sizeof(int));
    *b = 2;
    int* c = malloc(sizeof(int));
    *c = 3;
    vec_push(&vec, a);
    vec_push(&vec, b);
    vec_push(&vec, c);
    ASSERT(vec.length <= vec.capacity, "vec_push: length is not less than (or equal to) capacity");
    ASSERT(vec.length == 3, "vec_push: length is not 3");
    ASSERT(vec.data[0] == a, "vec_push: data[0] is not a");
    ASSERT(vec.data[1] == b, "vec_push: data[1] is not b");
    ASSERT(vec.data[2] == c, "vec_push: data[2] is not c");
    ASSERT(*(int*)vec.data[0] == 1, "vec_push: data[0] is not 1");
    ASSERT(*(int*)vec.data[1] == 2, "vec_push: data[1] is not 2");
    ASSERT(*(int*)vec.data[2] == 3, "vec_push: data[2] is not 3");
    free(vec.data[0]);
    free(vec.data[1]);
    free(vec.data[2]);
    free(vec.data);
    return 0;
}

int test_vec_push_over_capacity() {
    printf("collection::vec::vec_push_over_capacity...\n");
    vec_t vec;
    vec_init(&vec);
    for (int i = 0; i < 2*VEC_INIT_CAPACITY; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        vec_push(&vec, a);
    }
    ASSERT(vec.length <= vec.capacity, "vec_push: length is not less than (or equal to) capacity");
    for (int i = 0; i < 2*VEC_INIT_CAPACITY; i++) {
        ASSERT(*(int*)vec.data[i] == i, "vec_push: data[i] is not i");
        free(vec.data[i]);
    }
    free(vec.data);
    return 0;
}

int test_vec_pop() {
    printf("collection::vec::vec_pop...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    vec_push(&vec, a);
    int* b = malloc(sizeof(int));
    *b = 2;
    vec_push(&vec, b);
    int* c = malloc(sizeof(int));
    *c = 3;
    vec_push(&vec, c);
    int* d = vec_pop(&vec);
    ASSERT(d == c, "vec_pop: d is not c");
    ASSERT(*(int*)d == 3, "vec_pop: d is not 3");
    ASSERT(vec.length == 2, "vec_pop: length is not 2");
    free(d);
    int* e = vec_pop(&vec);
    ASSERT(e == b, "vec_pop: e is not b");
    ASSERT(*(int*)e == 2, "vec_pop: e is not 2");
    ASSERT(vec.length == 1, "vec_pop: length is not 1");
    free(e);
    int* f = vec_pop(&vec);
    ASSERT(f == a, "vec_pop: f is not a");
    ASSERT(*(int*)f == 1, "vec_pop: f is not 1");
    ASSERT(vec.length == 0, "vec_pop: length is not 0");
    free(f);
    free(vec.data);
    return 0;
}

int test_vec_get() {
    printf("collection::vec::vec_get...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    vec_push(&vec, a);
    int* b = malloc(sizeof(int));
    *b = 2;
    vec_push(&vec, b);
    int* c = malloc(sizeof(int));
    *c = 3;
    vec_push(&vec, c);
    int* d = vec_get(&vec, 0);
    ASSERT(d == a, "vec_get: d is not a");
    ASSERT(*(int*)d == 1, "vec_get: d is not 1");
    int* e = vec_get(&vec, 1);
    ASSERT(e == b, "vec_get: e is not b");
    ASSERT(*(int*)e == 2, "vec_get: e is not 2");
    int* f = vec_get(&vec, 2);
    ASSERT(f == c, "vec_get: f is not c");
    ASSERT(*(int*)f == 3, "vec_get: f is not 3");
    ASSERT(vec.length == 3, "vec_get: length is not 3");
    free(vec.data[0]);
    free(vec.data[1]);
    free(vec.data[2]);
    free(vec.data);
    return 0;
}

int test_vec_set() {
    printf("collection::vec::vec_set...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    vec_push(&vec, a);
    int* b = malloc(sizeof(int));
    *b = 2;
    vec_push(&vec, b);
    int* c = malloc(sizeof(int));
    *c = 3;
    vec_push(&vec, c);
    int* d = malloc(sizeof(int));
    *d = 4;
    vec_set(&vec, 0, d);
    ASSERT(vec.data[0] == d, "vec_set: data[0] is not d");
    ASSERT(*(int*)vec.data[0] == 4, "vec_set: data[0] is not 4");
    int* e = malloc(sizeof(int));
    *e = 5;
    vec_set(&vec, 1, e);
    ASSERT(vec.data[1] == e, "vec_set: data[1] is not e");
    ASSERT(*(int*)vec.data[1] == 5, "vec_set: data[1] is not 5");
    int* f = malloc(sizeof(int));
    *f = 6;
    vec_set(&vec, 2, f);
    ASSERT(vec.data[2] == f, "vec_set: data[2] is not f");
    ASSERT(*(int*)vec.data[2] == 6, "vec_set: data[2] is not 6");
    ASSERT(vec.length == 3, "vec_set: length is not 3");
    free(vec.data);
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
    free(f);
    return 0;
}

int test_vec_insert() {
    printf("collection::vec::vec_insert...\n");
    int start[4] = {1, 2, 3, 4};
    int end[7] = {0, 1, 2, 5, 3, 4, 6};
    vec_t vec;
    vec_init(&vec);
    
    for (int i = 0; i < 4; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, a);
    }

    int* b = malloc(sizeof(int));
    *b = 5;
    vec_insert(&vec, 2, b);

    int* c = malloc(sizeof(int));
    *c = 6;
    vec_insert(&vec, 5, c);

    int* d = malloc(sizeof(int));
    *d = 0;
    vec_insert(&vec, 0, d);

    for (int i = 0; i < 7; i++) {
        char msg[35] = "vec_insert: data[i] is not end[i]";
        msg[17] = '0' + i;
        msg[31] = '0' + i;
        ASSERT(*(int*)vec.data[i] == end[i], msg);
    }

    vec_destroy(&vec, true);
    return 0;
}

int test_vec_remove() {
    printf("collection::vec::vec_remove...\n");
    int start[5] = {1, 2, 3, 4, 5};
    int end[2] = {2, 3};
    vec_t vec;
    vec_init(&vec);
    
    for (int i = 0; i < 5; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, (void*) a);
    }

    vec_remove(&vec, 3, true);
    vec_remove(&vec, 3, true);
    vec_remove(&vec, 0, true);

    for (int i = 0; i < 2; i++) {
        char msg[35] = "vec_remove: data[i] is not end[i]";
        msg[17] = '0' + i;
        msg[31] = '0' + i;
        ASSERT(*(int*)vec.data[i] == end[i], msg);
    }

    vec_destroy(&vec, true);
    return 0;
}

int test_vec_clear() {
    printf("collection::vec::vec_clear...\n");
    vec_t vec;
    vec_init(&vec);
    int* a = malloc(sizeof(int));
    *a = 1;
    vec_push(&vec, a);
    int* b = malloc(sizeof(int));
    *b = 2;
    vec_push(&vec, b);
    int* c = malloc(sizeof(int));
    *c = 3;
    vec_push(&vec, c);
    vec_clear(&vec, true);
    ASSERT(vec.length == 0, "vec_clear: length is not 0");
    ASSERT(vec.data != NULL, "vec_clear: data is NULL");
    free(vec.data);
    return 0;
}

int test_vec_swap() {
    printf("collection::vec::vec_swap...\n");
    int start[5] = {1, 2, 3, 4, 5};
    int end[5] = {1, 5, 3, 4, 2};
    vec_t vec;
    vec_init(&vec);

    for (int i = 0; i < 5; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, a);
    }

    vec_swap(&vec, 1, 4);

    for (int i = 0; i < 5; i++) {
        char msg[35] = "vec_swap: data[i] is not end[i]";
        msg[15] = '0' + i;
        msg[29] = '0' + i;
        ASSERT(*(int*)vec.data[i] == end[i], msg);
    }

    vec_destroy(&vec, true);
    return 0;
}

int test_vec_swap_remove() {
    printf("collection::vec::vec_swap_remove...\n");
    int start[5] = {1, 2, 3, 4, 5};
    int end[4] = {1, 5, 3, 4};
    vec_t vec;
    vec_init(&vec);

    for (int i = 0; i < 5; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, a);
    }

    vec_swap_remove(&vec, 1, true);

    for (int i = 0; i < 4; i++) {
        char msg[40] = "vec_swap_remove: data[i] is not end[i]";
        msg[22] = '0' + i;
        msg[36] = '0' + i;
        ASSERT(*(int*)vec.data[i] == end[i], msg);
    }

    ASSERT(vec.length == 4, "vec_swap_remove: length is not 4");

    vec_destroy(&vec, true);
    return 0;
}

int compare_ints(const void* a, const void* b) {
    int arg1 = **(int**) a;
    int arg2 = **(int**) b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int test_vec_binary_search() {
    printf("collection::vec::vec_binary_search...\n");
    int start[10] = {1, 24, 35, 46, 57, 68, 79, 80, 91, 102};
    vec_t vec;
    vec_init(&vec);
    
    for (int i = 0; i < 10; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, a);
    }

    int* a = malloc(sizeof(int));
    *a = 102;
    int i = vec_binary_search(&vec, (void*) a, compare_ints);
    ASSERT(i == 9, "vec_binary_search: i is not 9");
    *a = 24;
    i = vec_binary_search(&vec, (void*) a, compare_ints);
    ASSERT(i == 1, "vec_binary_search: i is not 1");
    *a = 37;
    i = vec_binary_search(&vec, (void*) a, compare_ints);
    ASSERT(i == ~3, "vec_binary_search: i is not ~3");
    *a = 0;
    i = vec_binary_search(&vec, (void*) a, compare_ints);
    ASSERT(i == ~0, "vec_binary_search: i is not ~0");
    *a = 11;
    i = vec_binary_search(&vec, (void*) a, compare_ints);
    ASSERT(i == ~1, "vec_binary_search: i is not ~1");
    free(a);
    vec_destroy(&vec, true);
    return 0;
}

int test_vec_sort() {
    printf("collection::vec::vec_sort...\n");
    int start[10] = {102, 91, 80, 79, 68, 57, 46, 35, 24, 1};
    int end[10] = {1, 24, 35, 46, 57, 68, 79, 80, 91, 102};
    vec_t vec;
    vec_init(&vec);
    
    for (int i = 0; i < 10; i++) {
        int* a = malloc(sizeof(int));
        *a = start[i];
        vec_push(&vec, a);
    }

    ASSERT(*(int*)vec.data[0] == 102, "vec_sort: data[0] is not 102");
    ASSERT((int) vec_length(&vec) == 10, "vec_sort: length is not 10");

    vec_sort(&vec, compare_ints);

    for (int i = 0; i < 10; i++) {
        char msg[35] = "vec_sort: data[i] is not end[i]";
        msg[15] = '0' + i;
        msg[29] = '0' + i;
        ASSERT(*(int*)vec.data[i] == end[i], msg);
    }

    vec_destroy(&vec, true);
    return 0;

}

int test_vec() {
    int failed = 0;
    printf("Testing libcollection/vec...\n");
    failed += test_vec_init();
    failed += test_vec_destroy();
    failed += test_vec_push_one();
    failed += test_vec_push_many();
    failed += test_vec_push_over_capacity();
    failed += test_vec_pop();
    failed += test_vec_get();
    failed += test_vec_set();
    failed += test_vec_insert();
    failed += test_vec_remove();
    failed += test_vec_clear();
    failed += test_vec_swap();
    failed += test_vec_swap_remove();
    failed += test_vec_binary_search();
    failed += test_vec_sort();
    printf("libcollection/vec tests finished with %d failures.\n", failed);
    return failed;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef _LIB_DOOM_COLLECTION_VEC_H
#include "../../include/collection/vec.h"
#endif

size_t vec_capacity(vec_t* vec) {
    return vec->capacity;
}

size_t vec_length(vec_t* vec) {
    return vec->length;
}

void vec_init(vec_t* vec) {
    vec->capacity = VEC_INIT_CAPACITY;
    vec->length = 0;
    vec->data = malloc(sizeof(void*) * vec->capacity);
}

void vec_destroy(vec_t* vec, bool free_data) {
    if (free_data) {
        for (size_t i = 0; i < vec->length; i++) {
            free(vec->data[i]);
        }
    }
    free(vec->data);
    vec->capacity = 0;
    vec->length = 0;
}

void vec_push(vec_t* vec, void* item) {
    if (vec->length == vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void*) * vec->capacity);
        assert(vec->data != NULL);
    }
    vec->data[vec->length++] = item;
}

void* vec_pop(vec_t* vec) {
    if (vec->length == 0) {
        return NULL;
    }
    return vec->data[--vec->length];
}

void* vec_get(vec_t* vec, size_t index) {
    if (index >= vec->length) {
        return NULL;
    }
    return vec->data[index];
}

void vec_set(vec_t* vec, size_t index, void* item) {
    assert(index < vec->length);
    vec->data[index] = item;
}

void vec_insert(vec_t* vec, size_t index, void* item) {
    if (vec->length == vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void*) * vec->capacity);
        assert(vec->data != NULL);
    }
    for (size_t i = vec->length; i > index; i--) {
        vec->data[i] = vec->data[i - 1];
    }
    vec->data[index] = item;
    vec->length++;
}

void vec_remove(vec_t* vec, size_t index, bool free_data) {
    assert(index < vec->length);
    assert(vec->length > 0);
    if (free_data) {
        free(vec->data[index]);
    }
    for (size_t i = index; i < vec->length - 1; i++) {
        vec->data[i] = vec->data[i + 1];
    }
    vec->length--;
}

void vec_clear(vec_t* vec, bool free_data) {
    if (free_data) {
        for (size_t i = 0; i < vec->length; i++) {
            free(vec->data[i]);
        }
    }
    vec->length = 0;
}

void swap(vec_t* vec, size_t index1, size_t index2) {
    assert(index1 < vec->length);
    assert(index2 < vec->length);
    void* temp = vec->data[index1];
    vec->data[index1] = vec->data[index2];
    vec->data[index2] = temp;
}

void vec_swap_remove(vec_t* vec, size_t index, bool free_data) {
    assert(index < vec->length);
    if (free_data) {
        free(vec->data[index]);
    }
    vec->data[index] = vec->data[vec->length - 1];
    vec->length--;
}

int vec_binary_search(vec_t* vec, void* data, int (*cmp)(const void*, const void*)) {
    int left = 0;
    int right = vec->length - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        int cmp_result = cmp(data, vec->data[middle]);
        if (cmp_result == 0) {
            return middle;
        }
        if (cmp_result > 0) {
            right = middle - 1;
        } else {
            left = middle + 1;
        }
    }
    return -1;
}

void vec_sort(vec_t* vec, int (*cmp)(const void*, const void*)) {
    qsort(vec->data, vec->length, sizeof(void*), cmp);
}
#ifndef _LIB_DOOM_COLLECTION_VEC_H
#define _LIB_DOOM_COLLECTION_VEC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_INIT_CAPACITY 8

/**
 * A simple dynamic array implementation.
 */
typedef struct {
  size_t capacity;
  size_t length;
  void **data;
} vec_t;

/**
 * Returns the capacity of the vector.
 */
size_t vec_capacity(vec_t *vec);

/**
 * Returns the length of the vector.
 */
size_t vec_length(vec_t *vec);

/**
 * Initializes a vector.
 */
void vec_init(vec_t *vec);

/**
 * Destroys a vector, and its items, if free_data is true.
 */
void vec_destroy(vec_t *vec, bool free_data);

/**
 * Pushes an item to the end of the vector.
 */
void vec_push(vec_t *vec, void *item);

/**
 * Pops an item from the end of the vector.
 */
void *vec_pop(vec_t *vec);

/**
 * Returns the item at the given index.
 */
void *vec_get(vec_t *vec, size_t index);

/**
 * Sets the item at the given index.
 *
 * If the index is greater or equal than the length of the vector, it will
 * crash.
 */
void vec_set(vec_t *vec, size_t index, void *item);

/**
 * Inserts an item at the given index.
 *
 * If the index is greater than the length of the vector, it will crash.
 */
void vec_insert(vec_t *vec, size_t index, void *item);

/**
 * Removes the item at the given index.
 *
 * If the index is greater or equal than the length of the vector, it will
 * crash. If free_data is true, it will free the item.
 */
void vec_remove(vec_t *vec, size_t index, bool free_data);

/**
 * Clears the vector.
 *
 * If free_data is true, it will free the items.
 */
void vec_clear(vec_t *vec, bool free_data);

/**
 * Swap two items in the vector.
 *
 * If the index is greater or equal than the length of the vector, it will
 * crash.
 */
void vec_swap(vec_t *vec, size_t index1, size_t index2);

/**
 * Remove the item at index by swapping it with the last item.
 *
 * If free_data is true, it will free the item.
 */
void vec_swap_remove(vec_t *vec, size_t index, bool free_data);

/**
 * Binary search for an item in the vector.
 *
 * Returns the index of the item, or a negative index if it is not found.
 *
 * The vector must be sorted. Check vec_sort, for more information about the
 * comparison function. Due to the nature of the comparison function, data will
 * be passed as void** to the comparison function, as the first argument.
 *
 * The negative index is the bitwise negation of the index where the item should
 * be inserted.
 */
int vec_binary_search(vec_t *vec, void *data,
                      int (*cmp)(const void *, const void *));

/**
 * Sorts the vector.
 *
 * The comparison function must return a negative value if the first item is
 * less than the second item, a positive value if the first item is greater than
 * the second item, and zero if the items are equal.
 *
 * The comparison function must have the following signature:
 * int cmp(const void* a, const void* b);
 * Note that the comparison function is expected to receive pointers to the
 * items. Example: if this vec_t is a vector of integers, the comparison
 * function should expected that a and b are castable to int**.
 */
void vec_sort(vec_t *vec, int (*cmp)(const void *, const void *));
#endif
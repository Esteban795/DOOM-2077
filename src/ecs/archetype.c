#include <stdio.h>
#include <stdlib.h>

#include "../../include/ecs/archetype.h"

int compare_int(const void* a, const void* b) {
    return (*(int*) b - *(int*) a);
}

int compare_entity(const void* a, const void* b) {
    entity_t* entity_a = *(entity_t**) a;
    entity_t* entity_b = *(entity_t**) b;
    return (entity_b->id - entity_a->id);
}

int index_of_entity(archetype_t* archetype, entity_t* entity) {
    // TODO: Technically, the goal of an archetype ECS is that getting an entity is very efficient (O(1)),
    // but it needs some guarantees of the generation of the entity ID.
    // For now, we will use a binary search, but we might change this in the future, if the performance is not good enough.
    return vec_binary_search(&archetype->entities, entity, compare_entity);
}

void archetype_init(archetype_t* archetype, int component_count, int component_tags[]) {
    vec_init(&archetype->entities);
    vec_init(&archetype->components);
    vec_init(&archetype->tags);

    for (int i = 0; i < component_count; i++) {
        int* tag = malloc(sizeof(int));
        *tag = component_tags[i];
        vec_push(&archetype->tags, (void*) tag);
        vec_t* component = malloc(sizeof(vec_t));
        vec_init(component);
        vec_push(&archetype->components, (void*) component);
    }

    vec_sort(&archetype->tags, compare_int);
}

void archetype_destroy(archetype_t* archetype) {
    vec_destroy(&archetype->entities, false);

    while (vec_length(&archetype->components) > 0) {
        vec_t* component = (vec_t*) vec_pop(&archetype->components);
        vec_destroy(component, true);
        free(component);
    }

    vec_destroy(&archetype->components, false);
    vec_destroy(&archetype->tags, true);
}


void archetype_add_entity(archetype_t* archetype, entity_t* entity, component_t* components[]) {
    int ind = vec_binary_search(&archetype->entities, entity, compare_entity);
    if (ind >= 0)  {
        fprintf(stderr, "FATAL: Entity with id %ld already exists in archetype!\n", entity->id);
        return;
    }
    vec_insert(&archetype->entities, ~ind, (void*) entity);
    for (int i = 0; (size_t) i < vec_length(&archetype->tags); i++) {
        int tag = components[i]->tag;
        int ind = vec_binary_search(&archetype->tags, &tag, compare_int);
        if (ind >= 0) {
            vec_t* component = (vec_t*) vec_get(&archetype->components, ind);
            vec_insert(component, ~ind, components[i]);
        } else {
            printf("FATAL: Component with tag %d not found in archetype!\n", tag);
        }
    }
}

void archetype_add_entity_unordered(archetype_t* archetype, entity_t* entity, component_t* components[]) {
    vec_push(&archetype->entities, (void*) entity);
    for (int i = 0; (size_t) i < vec_length(&archetype->tags); i++) {
        int tag = components[i]->tag;
        int ind = vec_binary_search(&archetype->tags, &tag, compare_int);
        if (ind >= 0) {
            vec_t* component = (vec_t*) vec_get(&archetype->components, ind);
            vec_push(component, components[i]);
        } else {
            printf("FATAL: Component with tag %d not found in archetype!\n", tag);
        }
    }
}

// Partition function for quicksort
void partition(archetype_t* archetype, int low, int high, int* pivot) {
    int i = low;
    int j = high;
    int pivot_index = (low + high) / 2;
    entity_t* pivot_value = (entity_t*) vec_get(&archetype->entities, pivot_index);
    while (i <= j) {
        entity_t* low_value = (entity_t*) vec_get(&archetype->entities, i);
        while (compare_entity(&low_value, &pivot_value) > 0) {
            i++;
        }
        entity_t* high_value = (entity_t*) vec_get(&archetype->entities, j);
        while (compare_entity(&high_value, &pivot_value) < 0){
            j--;
        }
        if (i < j) {
            vec_swap(&archetype->entities, i, j);
            for (int k = 0; (size_t) k < vec_length(&archetype->components); k++) {
                vec_t* component = (vec_t*) vec_get(&archetype->components, k);
                vec_swap(component, i, j);
            }
            i++;
            j--;
        }
    }
    *pivot = i;
}

// Quicksort of archetype entities
void quicksort(archetype_t* archetype, int low, int high) {
    // WARN: This is a recursive quicksort implementation. It might stack overflow if the number of entities is too high.
    if (low < high) {
        int pivot;
        partition(archetype, low, high, &pivot);
        quicksort(archetype, low, pivot - 1);
        quicksort(archetype, pivot, high);
    }
}

void archetype_sort_components(archetype_t* archetype) {
    quicksort(archetype, 0, vec_length(&archetype->entities) - 1);
}

bool archetype_remove_entity(archetype_t* archetype, entity_t* entity) {
    int ind = index_of_entity(archetype, entity);
    if (ind == -1) {
        return false;
    }
    vec_remove(&archetype->entities, ind, false);
    for (int i = 0; (size_t) i < vec_length(&archetype->tags); i++) {
        vec_t* component = (vec_t*) vec_get(&archetype->components, i);
        vec_remove(component, ind, true);
    }
    return true;
}

bool archetype_remove_entity_unordered(archetype_t* archetype, entity_t* entity) {
    int ind = index_of_entity(archetype, entity);
    if (ind == -1) {
        return false;
    }
    vec_swap_remove(&archetype->entities, ind, false);
    for (int i = 0; (size_t) i < vec_length(&archetype->tags); i++) {
        vec_t* component = (vec_t*) vec_get(&archetype->components, i);
        vec_swap_remove(component, ind, true);
    }
    return true;
}

component_t* archetype_get_component(archetype_t* archetype, entity_t* entity, int tag) {
    int ind = index_of_entity(archetype, entity);
    if (ind < 0) {
        printf("WARN: Entity %ld not found in archetype!\n", entity->id);
        return NULL;
    }
    int tag_ind = vec_binary_search(&archetype->tags, &tag, compare_int);
    if (tag_ind < 0) {
        printf("WARN: Component with tag %d not found in archetype!\n", tag);
        return NULL;
    }
    vec_t* component = (vec_t*) vec_get(&archetype->components, tag_ind);
    return (component_t*) vec_get(component, ind);
}


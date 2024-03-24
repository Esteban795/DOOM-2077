#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/ecs/entity.h"
#include "../../include/ecs/archetype.h"
#include "../../include/collection/vec.h"

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_compare_entity(void) {
    printf("Testing ecs::archetype::compare_entity...\n");
    entity_t* e1 = malloc(sizeof(entity_t));
    entity_t* e2 = malloc(sizeof(entity_t));
    e1->id = 1;
    e2->id = 2;
    ASSERT(compare_entity(&e1, &e2) == -1, "compare_entity(&e1, &e2) == -1");
    ASSERT(compare_entity(&e2, &e1) == 1, "compare_entity(&e2, &e1) == 1");
    ASSERT(compare_entity(&e1, &e1) == 0, "compare_entity(&e1, &e1) == 0");
    free(e1);
    free(e2);
    return 0;
}

int test_archetype_init(void) {
    printf("Testing ecs::archetype::archetype_init...\n");
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    ASSERT(archetype.entities.length == 0, "archetype.entities.length == 0");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");
    ASSERT(*(int*)archetype.tags.data[0] == 1, "archetype.tags.data[0] == 1");
    ASSERT(*(int*)archetype.tags.data[1] == 2, "archetype.tags.data[1] == 2");
    ASSERT(*(int*)archetype.tags.data[2] == 3, "archetype.tags.data[2] == 3");

    vec_destroy(&archetype.entities, true);
    for (int i = 0; i < (int) archetype.components.length; i++) {
        vec_destroy(archetype.components.data[i], true);
    }
    vec_destroy(&archetype.components, true);
    vec_destroy(&archetype.tags, true);
    return 0;
}

int test_archetype_destroy(void) {
    printf("Testing ecs::archetype::archetype_destroy...\n");
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    archetype_destroy(&archetype);
    ASSERT(archetype.entities.length == 0, "archetype.entities.length == 0");
    ASSERT(archetype.components.length == 0, "archetype.components.length == 0");
    ASSERT(archetype.tags.length == 0, "archetype.tags.length == 0");
    return 0;
}

int test_archetype_add_entity(void) {
    printf("Testing ecs::archetype::archetype_add_entity...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entity to archetype
    entity_t entity;
    entity.id = 1;

    // Create the components of the entity
    component_t** components = malloc(3 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    archetype_add_entity(&archetype, &entity, components);

    // Check if the entity was added to the archetype
    ASSERT(archetype.entities.length == 1, "archetype.entities.length == 1");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    return 0;
}

int test_archetype(void) {
    int failed = 0;
    printf("Testing libecs/archetype...\n");
    failed += test_compare_entity();
    failed += test_archetype_init();
    failed += test_archetype_destroy();
    failed += test_archetype_add_entity();
    printf("libecs/archetype tests finished with %d failures.\n", failed);
    return failed;
}
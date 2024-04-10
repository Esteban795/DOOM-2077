#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/event.h"
#include "../../include/ecs/component.h"
#include "../../include/ecs/archetype.h"
#include "../../include/ecs/world.h"

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

world_t init_simple_world(void) {
    world_t world;
    world_init(&world);

    component_t* components[4];
    // Create a component with 2 integers (for example, a position component)
    components[0] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[0]->tag = 0; 
    *((int*)components[0]->data) = 12; // x: 12
    *((int*)(components[0]->data) + 1) = 34; // y: 34
    components[2] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[2]->tag = 0; 
    *((int*)components[2]->data) = 12; // x: 24
    *((int*)(components[2]->data) + 1) = 34; // y: 18
    // Create a component with 1 integer (for example, a health component)
    components[1] = malloc(sizeof(component_t) + sizeof(int));
    components[1]->tag = 1;
    *((int*)components[1]->data) = 100; // 100 HP
    components[3] = malloc(sizeof(component_t) + sizeof(int));
    components[3]->tag = 1;
    *((int*)components[3]->data) = 78; // 78 HP

    // Create entities with the corresponding components
    world_create_entity(&world, components, 2);
    world_create_entity(&world, components + 2, 2);

    return world;
}

int test_world_create_entity(void) {
    printf("Testing ecs::world::world_create_entity...\n");
    world_t world;
    world_init(&world);

    component_t* components[2];
    // Create a component with 2 integers (for example, a position component)
    components[0] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[0]->tag = 0; 
    *((int*)components[0]->data) = 12; // x: 12
    *((int*)(components[0]->data) + 1) = 34; // y: 34
    // Create a component with 1 integer (for example, a health component)
    components[1] = malloc(sizeof(component_t) + sizeof(int));
    components[1]->tag = 1;
    *((int*)components[1]->data) = 100; // 100 HP

    // Create an entity with the components
    entity_t* entity = world_create_entity(&world, components, 2);

    // Check if the entity was created
    ASSERT(entity != NULL, "world_create_entity failed");
    ASSERT(world.entities.length == 1, "world_create_entity failed -> world.entities.length != 1");
    ASSERT(world.entity_archetype.length == 1, "world_create_entity failed -> world.entity_archetype.length != 1");
    ASSERT(world.archetypes.length == 1, "world_create_entity failed -> world.archetypes.length != 1");
    
    world_destroy(&world);
    return 0;
}

int test_world_create_bulk_entity(void) {
    printf("Testing ecs::world::world_create_bulk_entity...\n");
    world_t world;
    world_init(&world);

    component_t* components[4];
    // Create a component with 2 integers (for example, a position component)
    components[0] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[0]->tag = 0; 
    *((int*)components[0]->data) = 12; // x: 12
    *((int*)(components[0]->data) + 1) = 34; // y: 34
    components[2] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[2]->tag = 0; 
    *((int*)components[2]->data) = 12; // x: 24
    *((int*)(components[2]->data) + 1) = 34; // y: 18
    // Create a component with 1 integer (for example, a health component)
    components[1] = malloc(sizeof(component_t) + sizeof(int));
    components[1]->tag = 1;
    *((int*)components[1]->data) = 100; // 100 HP
    components[3] = malloc(sizeof(component_t) + sizeof(int));
    components[3]->tag = 1;
    *((int*)components[3]->data) = 78; // 78 HP

    // Create entities with the corresponding components
    entity_t* entity = world_create_entity(&world, components, 2);
    entity_t* entity2 = world_create_entity(&world, components + 2, 2);

    // Check if the entities have been created
    ASSERT(entity != NULL, "world_create_entity failed -> entity == NULL");
    ASSERT(entity2 != NULL, "world_create_entity failed -> entity2 == NULL");
    ASSERT(world.entities.length == 2, "world_create_entity failed -> world.entities.length != 2");
    ASSERT(world.entity_archetype.length == 2, "world_create_entity failed -> world.entity_archetype.length != 2");
    ASSERT(world.archetypes.length == 1, "world_create_entity failed -> world.archetypes.length != 1");
    
    world_destroy(&world);
    return 0;
}

int test_world_remove_entity(void)  {
    printf("Testing ecs::world::world_remove_entity...\n");
    world_t world;
    world_init(&world);

    component_t* components[4];
    // Create a component with 2 integers (for example, a position component)
    components[0] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[0]->tag = 0; 
    *((int*)components[0]->data) = 12; // x: 12
    *((int*)(components[0]->data) + 1) = 34; // y: 34
    components[2] = malloc(sizeof(component_t) + 2 * sizeof(int));
    components[2]->tag = 0; 
    *((int*)components[2]->data) = 24; // x: 24
    *((int*)(components[2]->data) + 1) = 18; // y: 18
    // Create a component with 1 integer (for example, a health component)
    components[1] = malloc(sizeof(component_t) + sizeof(int));
    components[1]->tag = 1;
    *((int*)components[1]->data) = 100; // 100 HP
    components[3] = malloc(sizeof(component_t) + sizeof(int));
    components[3]->tag = 1;
    *((int*)components[3]->data) = 78; // 78 HP

    // Create entities with the corresponding components
    entity_t* entity = world_create_entity(&world, components, 2);
    entity_t* entity2 = world_create_entity(&world, components + 2, 2);

    // Check if the entities have been created
    ASSERT(entity != NULL, "world_create_entity failed -> entity == NULL");
    ASSERT(entity2 != NULL, "world_create_entity failed -> entity2 == NULL");
    ASSERT(world.entities.length == 2, "world_create_entity failed -> world.entities.length != 2");

    // Remove the first entity
    world_remove_entity(&world, entity);

    // Check if the entity has been removed
    ASSERT(world.entities.length == 1, "world_remove_entity failed -> world.entities.length != 1");
    ASSERT(world.entity_archetype.length == 1, "world_remove_entity failed -> world.entity_archetype.length != 1");
    ASSERT(world.archetypes.length == 1, "world_remove_entity failed -> world.archetypes.length != 1");

    // Check it is actually the first entity that has been removed
    component_t* e2_health = world_get_component(&world, entity2, 1);
    ASSERT(e2_health != NULL, "world_get_component failed -> e2_health == NULL");
    ASSERT(*(int*)e2_health->data == 78, "world_get_component failed -> *(int*)e2_health->data != 78");
    component_t* e2_position = world_get_component(&world, entity2, 0);
    ASSERT(e2_position != NULL, "world_get_component failed -> e2_position == NULL");
    ASSERT(*(int*)e2_position->data == 24, "world_get_component failed -> e2_position_x != 24");
    ASSERT(*((int*)e2_position->data + 1) == 18, "world_get_component failed -> e2_position_y != 18");
    
    world_destroy(&world);
    return 0;
}

int test_world_register_system(void) {
    printf("Testing ecs::world::world_register_system...\n");
    world_t world;
    world_init(&world);

    // Register a system
    world_register_system(&world, NULL);

    // Check if the system has been registered
    ASSERT(world.systems.length == 1, "world_register_system failed -> world.systems.length != 1");

    world_destroy(&world);
    return 0;
}

int test_world_get_archetype(void) {
    printf("Testing ecs::world::world_get_archetype...\n");
    world_t world = init_simple_world();

    // Get the archetype of the first entity
    archetype_t* archetype = world_get_archetype(&world, world.entities.data[0]);

    // Check if the archetype has been retrieved    
    ASSERT(archetype != NULL, "world_get_archetype failed -> archetype == NULL");
    ASSERT(archetype->tags.length == 2, "world_get_archetype failed -> archetype->tags.length != 2");
    ASSERT(archetype->components.length == 2, "world_get_archetype failed -> archetype->components.length != 2");
    ASSERT(*(int*)archetype->tags.data[0] == 0, "world_get_archetype failed -> *(int*)archetype->tags.data[0] != 0");
    ASSERT(*(int*)archetype->tags.data[1] == 1, "world_get_archetype failed -> *(int*)archetype->tags.data[1] != 1");

    world_destroy(&world);
    return 0;
}

int test_world_get_component(void) {
    printf("Testing ecs::world::world_get_component...\n");
    world_t world = init_simple_world();

    // Get the position component of the first entity
    component_t* position = world_get_component(&world, world.entities.data[0], 0);

    // Check if the component has been retrieved
    ASSERT(position != NULL, "world_get_component failed -> position == NULL");
    ASSERT(*(int*)position->data == 12, "world_get_component failed -> *(int*)position->data != 12");
    ASSERT(*((int*)position->data + 1) == 34, "world_get_component failed -> *((int*)position->data + 1) != 34");

    // Get the health component of the first entity
    component_t* health = world_get_component(&world, world.entities.data[0], 1);
    ASSERT(health != NULL, "world_get_component failed -> health == NULL");
    ASSERT(*(int*)health->data == 100, "world_get_component failed -> *(int*)health->data != 100");

    world_destroy(&world);
    return 0;

}

int test_world_get_archetype_by_tags(void) {
    printf("Testing ecs::world::world_get_archetype_by_tags...\n");
    world_t world = init_simple_world();

    // Get the archetype of the first entity
    int tags[2] = {0, 1};
    archetype_t* archetype = world_get_archetype_by_tags(&world, tags, 2);

    // Check if the archetype has been retrieved    
    ASSERT(archetype != NULL, "world_get_archetype_by_tags failed -> archetype == NULL");
    ASSERT(archetype->tags.length == 2, "world_get_archetype_by_tags failed -> archetype->tags.length != 2");
    ASSERT(archetype->components.length == 2, "world_get_archetype_by_tags failed -> archetype->components.length != 2");
    ASSERT(*(int*)archetype->tags.data[0] == 0, "world_get_archetype_by_tags failed -> *(int*)archetype->tags.data[0] != 0");
    ASSERT(*(int*)archetype->tags.data[1] == 1, "world_get_archetype_by_tags failed -> *(int*)archetype->tags.data[1] != 1");

    // Get an archetype that doesn't exist
    int tags3[2] = {0, 2};
    archetype_t* archetype3 = world_get_archetype_by_tags(&world, tags3, 2);
    ASSERT(archetype3 == NULL, "world_get_archetype_by_tags failed -> archetype3 != NULL");

    world_destroy(&world);
    return 0;

}

int test_world_queue_event(void) {
    printf("Testing ecs::world::world_queue_event...\n");
    world_t world = init_simple_world();

    ASSERT(world.event_queue.length == 0, "world_queue_event failed -> world.event_queue.length != 0");

    for (int i = 0; i < 10; i++) {
        // Create an event
        event_t* event = malloc(sizeof(event_t));
        event->tag = i;
        // Queue the event
        world_queue_event(&world, event);
        // Check if the event has been queued
        ASSERT((int) world.event_queue.length == i+1, "world_queue_event failed -> world.event_queue.length != i");
    }

    world_destroy(&world);
    return 0;
}

int test_world_add_components(void) {
    printf("Testing ecs::world::world_add_components...\n");
    world_t world = init_simple_world();

    // Create a new component with an integer (for instance, ammo component)
    component_t* new_component = malloc(sizeof(component_t) + sizeof(int));
    new_component->tag = 2;
    *((int*)new_component->data) = 42; // 42 ammo left

    // Add the new component to the first entity
    world_add_components(&world, world.entities.data[0], &new_component, 1);

    // Check if the component has been added
    component_t* new_component_retrieved = world_get_component(&world, world.entities.data[0], 2);
    ASSERT(new_component_retrieved != NULL, "world_add_components failed -> new_component_retrieved == NULL");
    ASSERT(*(int*)new_component_retrieved->data == 42, "world_add_components failed -> *(int*)new_component_retrieved->data != 42");

    world_destroy(&world);
    return 0;
}

int test_world_remove_components(void) {
    printf("Testing ecs::world::world_remove_components...\n");
    world_t world = init_simple_world();

    // Create a new component with an integer (for instance, ammo component)
    component_t* new_component = malloc(sizeof(component_t) + sizeof(int));
    new_component->tag = 2;
    *((int*)new_component->data) = 42; // 42 ammo left

    // Add the new component to the first entity
    world_add_components(&world, world.entities.data[0], &new_component, 1);

    // Check if the component has been added
    component_t* new_component_retrieved = world_get_component(&world, world.entities.data[0], 2);
    ASSERT(new_component_retrieved != NULL, "world_add_components failed -> new_component_retrieved == NULL");
    ASSERT(*(int*)new_component_retrieved->data == 42, "world_add_components failed -> *(int*)new_component_retrieved->data != 42");

    // Remove the new component
    int tags[1] = {2};
    world_remove_components(&world, world.entities.data[0], tags, 1);

    // Check if the component has been removed
    component_t* new_component_retrieved2 = world_get_component(&world, world.entities.data[0], 2);
    ASSERT(new_component_retrieved2 == NULL, "world_remove_components failed -> new_component_retrieved2 != NULL");

    world_destroy(&world);
    return 0;
}

int test_world_update(void) {
    // TODO: If judged necessary
    return 0;
}

int test_world(void) {
    int failed = 0;
    printf("Testing libecs/world...\n");
    failed += test_world_create_entity();
    failed += test_world_create_bulk_entity();
    failed += test_world_remove_entity();
    failed += test_world_register_system();
    failed += test_world_get_archetype();
    failed += test_world_get_component();
    failed += test_world_get_archetype_by_tags();
    failed += test_world_queue_event();
    failed += test_world_add_components();
    failed += test_world_remove_components();
    printf("libecs/world tests finished with %d failures.\n", failed);
    return failed;
}
#include <stdio.h>
#include <stdlib.h>

#include "../../include/ecs/archetype.h"
#include "../../include/collection/vec.h"
#include "../../include/ecs/world.h"

void world_init(world_t* world) {
    vec_init(&world->entities);
    vec_init(&world->entity_archetype);
    vec_init(&world->archetypes);
    vec_init(&world->systems);
}

void world_destroy(world_t* world) {
    vec_destroy(&world->entities, true);
    vec_destroy(&world->entity_archetype, true);
    vec_destroy(&world->archetypes, true);
    vec_destroy(&world->systems, true);
}

entity_t* world_create_entity(world_t* world, component_t** components, int component_count) {
    // Create the entity
    entity_t* entity = malloc(sizeof(entity_t));
    entity->id = vec_length(&world->entities) + 1;
    vec_push(&world->entities, (void*) entity);

    // Find the archetype corresponding to the components of the entity
    archetype_tag_t archetype_pat;
    archetype_pat.component_count = component_count;
    archetype_pat.component_tags = malloc(sizeof(int) * component_count);
    for (int i = 0; i < component_count; i++) {
        archetype_pat.component_tags[i] = components[i]->tag;
    }
    
    int arch_index = vec_binary_search(&world->archetypes, &archetype_pat, archetype_match);
    if (arch_index < 0) {
        // Create a new archetype if it doesn't exist
        archetype_t* archetype = malloc(sizeof(archetype_t));
        archetype_init(archetype, component_count, archetype_pat.component_tags);
        vec_insert(&world->archetypes, ~arch_index, (void*) archetype);
        arch_index = ~arch_index;
    }

    free(archetype_pat.component_tags);

    // Add the entity to the archetype
    archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, arch_index);
    archetype_add_entity(archetype, entity, components);

    // Add the archetype index to the entity_archetype vector
    vec_push(&world->entity_archetype, (void*) &arch_index);

    return entity;
}

entity_t** world_create_bulk_entity(world_t* world, component_t*** components, int component_count, int count) {
    // Create the entities
    entity_t** entities = malloc(sizeof(entity_t*) * count);
    for (int i = 0; i < count; i++) {
        entities[i] = malloc(sizeof(entity_t));
        entities[i]->id = vec_length(&world->entities) + 1;
        vec_push(&world->entities, (void*) entities[i]);
    }

    // Find the archetype corresponding to the components of the entity
    archetype_tag_t archetype_pat;
    archetype_pat.component_count = component_count;
    archetype_pat.component_tags = malloc(sizeof(int) * component_count);
    for (int i = 0; i < component_count; i++) {
        archetype_pat.component_tags[i] = components[0][i]->tag;
    }

    int arch_index = vec_binary_search(&world->archetypes, &archetype_pat, archetype_match);
    if (arch_index < 0) {
        // Create a new archetype if it doesn't exist
        archetype_t* archetype = malloc(sizeof(archetype_t));
        archetype_init(archetype, component_count, archetype_pat.component_tags);
        vec_insert(&world->archetypes, ~arch_index, (void*) archetype);
        arch_index = ~arch_index;
    }

    // Add the entities to the archetype
    archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, arch_index);
    for (int i = 0; i < count; i++) {
        archetype_add_entity(archetype, entities[i], components[i]);
    }

    return entities;
}

void world_remove_entity(world_t* world, entity_t* entity) {
    // Find the entity in the world
    int ind = vec_binary_search(&world->entities, entity, compare_entity);
    if (ind < 0) {
        fprintf(stderr, "FATAL: Entity with id %ld not found in world!\n", entity->id);
        return;
    }

    // Remove the entity from the world
    vec_remove(&world->entities, ind, false);
    int arch_index = *(int*) vec_get(&world->entity_archetype, ind);
    archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, arch_index);
    archetype_remove_entity(archetype, entity);
    vec_remove(&world->entity_archetype, ind, true);

    // Free the entity
    free(entity);
}

void register_system(world_t* world, int (*system_fn)(world_t*, event_t*)) {
    system_t* s = malloc(sizeof(system_t));
    s->fn = system_fn;
    vec_push(&world->systems, s);
}

archetype_t* world_get_archetype(world_t* world, entity_t* entity) {
    int ind = vec_binary_search(&world->entities, entity, compare_entity);
    if (ind < 0) {
        fprintf(stderr, "FATAL: Entity with id %ld not found in world!\n", entity->id);
        return NULL;
    }

    int arch_index = *(int*) vec_get(&world->entity_archetype, ind);
    return (archetype_t*) vec_get(&world->archetypes, arch_index);
}

component_t* world_get_component(world_t* world, entity_t* entity, int tag) {
    archetype_t* archetype = world_get_archetype(world, entity);
    if (archetype == NULL) {
        return NULL;
    }
    return archetype_get_component(archetype, entity, tag);
}

archetype_t* world_get_archetype_by_tags(world_t* world, int component_tags[], int component_count) {
    archetype_tag_t archetype_pat;
    archetype_pat.component_count = component_count;
    archetype_pat.component_tags = component_tags;

    int arch_index = vec_binary_search(&world->archetypes, &archetype_pat, archetype_match);
    if (arch_index < 0) {
        return NULL;
    }

    return (archetype_t*) vec_get(&world->archetypes, arch_index);
}

void world_queue_event(world_t* world, event_t* event) {
    vec_push(&world->event_queue, (void*) event);
}


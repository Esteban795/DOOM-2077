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
    vec_init(&world->event_queue);
}

void world_destroy(world_t* world) {
    vec_destroy(&world->entities, true);
    vec_destroy(&world->entity_archetype, true);
    vec_destroy(&world->systems, true);
    for (size_t i = 0; i < vec_length(&world->archetypes); i++) {
        archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, i);
        archetype_destroy(archetype);
        free(archetype);
    }
    vec_destroy(&world->archetypes, false);
    vec_destroy(&world->event_queue, true);
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
    int* arch_index_ptr = malloc(sizeof(int));
    *arch_index_ptr = arch_index;
    vec_push(&world->entity_archetype, (void*) arch_index_ptr);

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

        // Add the archetype index to the entity_archetype vector
        int* arch_index_ptr = malloc(sizeof(int));
        *arch_index_ptr = arch_index;
        vec_push(&world->entity_archetype, (void*) arch_index_ptr);
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
    archetype_remove_entity(archetype, entity, true);
    vec_remove(&world->entity_archetype, ind, true);

    free(entity);
}

void world_register_system(world_t* world, int (*system_fn)(world_t*, event_t*)) {
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


void world_add_components(world_t* world, entity_t* entity, component_t** components, int component_count) {
    // Find the entity in the world
    int ind = vec_binary_search(&world->entities, entity, compare_entity);
    if (ind < 0) {
        fprintf(stderr, "FATAL: Entity with id %ld not found in world!\n", entity->id);
        return;
    }

    // Find the current archetype of the entity
    int arch_index = *(int*) vec_get(&world->entity_archetype, ind);
    archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, arch_index);
    
    // Create a new archetype tag with the new components
    int old_tag_count = (int) vec_length(&archetype->tags);
    int* new_tags = malloc(sizeof(int) * (old_tag_count + component_count));
    component_t** new_components = malloc(sizeof(component_t*) * (old_tag_count + component_count));
    for (int i = 0; i < old_tag_count; i++) {
        new_tags[i] = *(int*) vec_get(&archetype->tags, i);
        new_components[i] = archetype_get_component(archetype, entity, new_tags[i]);
    }
    for (int i = 0; i < component_count; i++) {
        new_tags[old_tag_count + i] = components[i]->tag;
        new_components[old_tag_count + i] = components[i];
    }
    
    // Find the new archetype
    int dest_index = vec_binary_search(&world->archetypes, new_tags, archetype_match);
    if (dest_index < 0) {
        // Create a new archetype if it doesn't exist
        archetype_t* dest_archetype = malloc(sizeof(archetype_t));
        archetype_init(dest_archetype, old_tag_count + component_count, new_tags);
        vec_insert(&world->archetypes, ~dest_index, (void*) dest_archetype);
        dest_index = ~dest_index;
    }

    // Remove the entity from the old archetype
    archetype_remove_entity(archetype, entity, false);

    // Add the entity to the new archetype
    archetype_t* dest_archetype = (archetype_t*) vec_get(&world->archetypes, dest_index);
    archetype_add_entity(dest_archetype, entity, new_components);

    // Update the entity_archetype vector
    *(int*) vec_get(&world->entity_archetype, ind) = dest_index;

    free(new_tags);
    free(new_components);
}

void world_remove_components(world_t* world, entity_t* entity, int* component_tag, int component_count) {
    // Find the entity in the world
    int ind = vec_binary_search(&world->entities, entity, compare_entity);
    if (ind < 0) {
        fprintf(stderr, "FATAL: Entity with id %ld not found in world!\n", entity->id);
        return;
    }

    // Find the current archetype of the entity
    int arch_index = *(int*) vec_get(&world->entity_archetype, ind);
    archetype_t* archetype = (archetype_t*) vec_get(&world->archetypes, arch_index);

    // Create a new archetype tag without the components to be removed
    int old_tag_count = (int) vec_length(&archetype->tags);
    int* new_tags = malloc(sizeof(int) * (old_tag_count - component_count));
    component_t** new_components = malloc(sizeof(component_t*) * (old_tag_count - component_count));
    int new_tag_count = 0;
    int j =0;

    component_t* component_to_free[component_count];
    for (int i = 0; i < old_tag_count; i++) {
        if (j < component_count && component_tag[j] == *(int*) vec_get(&archetype->tags, i)) {
            component_to_free[j] = archetype_get_component(archetype, entity, component_tag[j]);
            j++;
            continue;
        }
        new_tags[new_tag_count] = *(int*) vec_get(&archetype->tags, i);
        new_components[new_tag_count] = archetype_get_component(archetype, entity, new_tags[new_tag_count]);
        new_tag_count++;
    }
    if (new_tag_count != old_tag_count - component_count) {
        fprintf(stderr, "FATAL: Component tags to remove not found in entity!\n");
        free(new_tags);
        free(new_components);
        return;
    }

    // Find the new archetype
    int dest_index = vec_binary_search(&world->archetypes, new_tags, archetype_match);
    if (dest_index < 0) {
        // Create a new archetype if it doesn't exist
        archetype_t* dest_archetype = malloc(sizeof(archetype_t));
        archetype_init(dest_archetype, new_tag_count, new_tags);
        vec_insert(&world->archetypes, ~dest_index, (void*) dest_archetype);
        dest_index = ~dest_index;
    }

    // Remove the entity from the old archetype
    archetype_remove_entity(archetype, entity, false);

    // Free the components to be removed
    for (int i = 0; i < component_count; i++) {
        if (component_to_free[i] != NULL)
            free(component_to_free[i]);
    }

    // Add the entity to the new archetype
    archetype_t* dest_archetype = (archetype_t*) vec_get(&world->archetypes, dest_index);
    archetype_add_entity(dest_archetype, entity, new_components);

    // Update the entity_archetype vector
    *(int*) vec_get(&world->entity_archetype, ind) = dest_index;

    free(new_tags);
    free(new_components);
}

void world_update(world_t* world) {
    // Process the event queue
    // IMPORTANT NOTE: We process each event one after the other by all systems. 
    // This is to ensure that the order of events is maintained, and that systems can create new events
    // that are processed in the same tick. (This is why we don't use a for loop here and we iterate these items like this)
    int i = 0;
    while (i < (int) vec_length(&world->event_queue)) {
        event_t* event = (event_t*) vec_get(&world->event_queue, i);
        for (int j = 0; j < (int) vec_length(&world->systems); j++) {
            system_t* system = (system_t*) vec_get(&world->systems, j);
            system->fn(world, event);
        }
        i++;
    }
    vec_clear(&world->event_queue, true);
}

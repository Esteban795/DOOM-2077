#ifndef _LIB_DOOM_ECS_WORLD_H
#define _LIB_DOOM_ECS_WORLD_H

#include <time.h>

#include "../event/event.h"
#include "../collection/vec.h"

#include "entity.h"
#include "archetype.h"
#include "component.h"

/* 
* The world contains all the entities and archetypes in the game. 
* It is the main structure that holds the game state.
*/
typedef struct {
    // vec_t<entity_t>
    vec_t entities;
    // vec_t<archetype_t>
    vec_t entity_archetype;
    // vec_t<archetype_t>
    vec_t archetypes;
    // vec_t<system_t>
    vec_t systems;
    // vec_t<event_t>
    vec_t event_queue;
    // last_tick
    struct timespec last_tick;
    // delta_time (in us)
    uint64_t delta_time;
} world_t;

typedef struct {
    int (*fn)(world_t*, event_t*);
} system_t;

/*
* Initialize a new world
*/
void world_init(world_t* world);

/*
* Destroy a world
*/
void world_destroy(world_t* world);

/*
* Insert an entity into the world
*
* If an entity with the same id already exists, the function will return NULL, and the entity will not be inserted.
* This function is intented to be used on the client side, where all the entities are created by the server, and therefore
* the client already knows the id of the entity.
*/
entity_t* world_insert_entity(world_t* world, uint64_t entity_id, component_t** components, int component_count);

/*
* Create a new entity
*/
entity_t* world_create_entity(world_t* world, component_t** components, int component_count);

/*
* Create bulk entities (exactly count entities) with the respective components (each entity has the same type of components)
*
* Freeing the components array is the responsibility of the caller. IMPORTANT NOTE: Do not free the component items.
*/
entity_t** world_create_bulk_entity(world_t* world, component_t*** components, int component_count, int count);

/*
* Remove an entity from the world
*/
void world_remove_entity(world_t* world, entity_t* entity);

/*
* Remove an entity from the world using only its ID.
*/
void world_remove_entity_by_id(world_t* world, uint64_t entity_id);

/*
* Register a new system
*/
void world_register_system(world_t* world, int (*system_fn)(world_t*, event_t*));

/*
* Get the archetype of an entity
*/
archetype_t* world_get_archetype(world_t* world, entity_t* entity);

/*
* Get the component (by tag) of an entity in the world
*/
component_t* world_get_component(world_t* world, entity_t* entity, int tag);

/*
* Get the archetype of a component tags
*/
archetype_t* world_get_archetype_by_tags(world_t* world, int component_tags[], int component_count);

/*
* Queue an event
*/
void world_queue_event(world_t* world, event_t* event);

/*
* Get the length of the event queue
*/
unsigned int world_queue_length(world_t* world);

/*
* Add components to an entity
*/
void world_add_components(world_t* world, entity_t* entity, component_t** components, int component_count);

/*
* Remove components from an entity
*/
void world_remove_components(world_t* world, entity_t* entity, int* component_tag, int component_count);

/*
* Execute all systems
*
* This function will execute all systems in the world on the events in the queue.
*
* Note: This function guarantees that all events will be processed in the order they were queued.
* Moreover, events can be queued from within systems, and they will be processed in the same time step.
*/
void world_update(world_t* world);

bool world_entity_has_component(world_t* world, entity_t* entity, int tag);
#endif
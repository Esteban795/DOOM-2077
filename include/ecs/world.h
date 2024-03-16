#ifndef _LIB_DOOM_ECS_WORLD_H
#define _LIB_DOOM_ECS_WORLD_H

#include "../event/event.h"

#include "entity.h"
#include "archetype.h"
#include "component.h"

typedef int system_t(world_t* world, event_t* event);

/* 
* The world contains all the entities and archetypes in the game. 
* It is the main structure that holds the game state.
*/
typedef struct {
    int entity_count;
    int archetype_count;
    // vec_t<entity_t>
    vec_t* entities;
    // vec_t<archetype_t>
    vec_t* entity_archetype;
    // vec_t<archetype_t>
    vec_t* archetypes;
    // vec_t<system_t>
    vec_t* systems;
} world_t;

/*
* Initialize a new world
*/
void world_init(world_t* world);

/*
* Destroy a world
*/
void world_destroy(world_t* world);

/*
* Create a new entity
*/
entity_t* world_create_entity(world_t* world, component_t* components[], int component_count);

/*
* Create bulk entities (exactly count entities)
*/
entity_t** world_create_bulk_entity(world_t* world, component_t* components[], int component_count, int count);

/*
* Remove an entity from the world
*/
void world_remove_entity(world_t* world, entity_t* entity);

/*
* Register a new system
*/
int world_register_system(world_t* world, system_t* system);

/*
* Register a new component
*/
void world_register_component(world_t* world, component_t* component);

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
* Handle an event
*/
int world_handle_event(world_t* world, event_t* event);

/*
* Add components to an entity
*/
void world_add_components(world_t* world, entity_t* entity, component_t** components, int component_count);

/*
* Remove components from an entity
*/
void world_remove_components(world_t* world, entity_t* entity, int component_tag);

#endif
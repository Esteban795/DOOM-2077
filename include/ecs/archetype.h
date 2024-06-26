#ifndef _LIB_DOOM_ECS_ARCHETYPE_H
#define _LIB_DOOM_ECS_ARCHETYPE_H

#include "../collection/vec.h"

#include "entity.h"
#include "component.h"

/**
 * Archetype representation
 *
 * An archetype is a structure that holds the information necessary to find and manipulate the entities that share
 * a set of components. 
 */
typedef struct {
    // vec_t<entity_t*>
    vec_t entities;
    // vec_t<vec_t<component>>
    vec_t components;
    // vec_t<int>
    vec_t tags;
} archetype_t;

/**
* Archetype tag representation
*
* An archetype tag is a structure that holds the information necessary to determine if two archetypes stores the same
* set of components.
*/
typedef struct {
    int component_count;
    int* component_tags;
} archetype_tag_t;

int compare_entity(const void* a, const void* b);

/**
 * Intialize a new archetype
 *
 * This function inits a new archetype.
 */
void archetype_init(archetype_t* archetype, int component_count, int component_tags[]);

/**
* Destroy an archetype
*/
void archetype_destroy(archetype_t* archetype);

/**
* Add an entity to an archetype
*/
void archetype_add_entity(archetype_t* archetype, entity_t* entity, component_t* components[]);

/**
* Add an entity to an archetype, without reordering the component.
* This is useful when the world is being initialized, or you are creating a lot of entities at once.
*
* This function is faster than archetype_add_entity, but you must call archetype_sort_components after adding all entities.
*/
void archetype_add_entity_unordered(archetype_t* archetype, entity_t* entity, component_t* components[]);

/**
* Sort the components of an archetype.
*/
void archetype_sort_components(archetype_t* archetype);

/**
* Remove an entity from an archetype
*
* If should_free is true, the components of the entity will be freed.
*/
bool archetype_remove_entity(archetype_t* archetype, entity_t* entity, bool should_free);

/**
* Remove an entity without reordering the components.
* This is useful when the world is being destroyed, or you are removing a lot of entities at once.
*
* This function is faster than archetype_remove_entity, but you must call archetype_sort_components after adding all entities.
* If should_free is true, the components of the entity will be freed.
*/
bool archetype_remove_entity_unordered(archetype_t* archetype, entity_t* entity, bool should_free);

/**
* Get the component (by tag) of an entity in an archetype
*/
component_t* archetype_get_component(archetype_t* archetype, entity_t* entity, int tag);

/**
* Determine if an archetype matches a given archetype tag
*
* Returns 0 if the archetypes match.
* Returns a negative number if the archetype tag is less than the archetype.
* Returns a positive number if the archetype tag is greater than the archetype
*
* This function is designed to be used with vec_sort.
*
* int archetype_match(const archetype_tag_t** archetype_tag, const archetype_t** archetype);
*/
int archetype_match(const void* _archetype_tag, const void* _archetype);

#endif
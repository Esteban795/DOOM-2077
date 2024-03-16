#ifndef _LIB_DOOM_ECS_ARCHETYPE_H
#define _LIB_DOOM_ECS_ARCHETYPE_H

#include "entity.h"
#include "component.h"

/*
 * Archetype representation
 *
 * An archetype is a structure that holds the information necessary to find and manipulate the entities that share
 * a set of components. 
 */
typedef struct {
    int component_count;
    entity_t** entities;
    component_t** components;
} archetype_t;

#endif
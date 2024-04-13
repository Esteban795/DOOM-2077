#ifndef _LIB_DOOM_COMPONENT_HEALTH_H
#define _LIB_DOOM_COMPONENT_HEALTH_H

#include "../ecs/component.h"

extern const int COMPONENT_TAG_HEALTH;

/*
* health_ct is a component that represents the health of an entity.
* It contains information about the maximum health and the current health of the entity.
*/
typedef struct {
    int tag;
    float max_health;
    float health;
} health_ct;

// health_create creates a new health component with the given max_health and health.
component_t* health_create(float max_health, float health);

// health_get returns the current health of the entity.
float health_get(health_ct* health);

// health_get_max returns the maximum health of the entity.
float health_get_max(health_ct* health);

// health_set sets the health of the entity to the given value.
float health_set(health_ct* health, float value);

// health_set_max sets the maximum health of the entity to the given value.
// If the current health is greater than the new maximum health, the current health is set to the new maximum health.
float health_add(health_ct* health, float value);

// health_sub subtracts the given value from the health of the entity.
// If the health of the entity is less than the given value, the health is set to 0.
float health_sub(health_ct* health, float value);

// health_reset sets the health of the entity to the maximum health.
// This function is useful when the entity respawns, or regenerates health.
float health_reset(health_ct* health);

#endif
#include <stdlib.h>
#include <assert.h>

#include "../../include/ecs/component.h"
#include "../../include/component/health.h"

const int COMPONENT_TAG_HEALTH = 1;

component_t* health_create(float max_health, float health) {
    health_ct* health_component = malloc(sizeof(health_ct));
    health_component->max_health = max_health;
    health_component->health = health;
    health_component->tag = COMPONENT_TAG_HEALTH;
    return (component_t*) health_component;
}

extern inline float health_get(health_ct* health);
extern inline float health_get_max(health_ct* health);
extern inline float health_set(health_ct* health, float value);
extern inline float health_set_max(health_ct* health, float value);
extern inline float health_add(health_ct* health, float value);
extern inline float health_sub(health_ct* health, float value);
extern inline float health_reset(health_ct* health);
#include <stdlib.h>

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

float health_get(health_ct* health) {
    return health->health;
}

float health_get_max(health_ct* health) {
    return health->max_health;
}

float health_set(health_ct* health, float value) {
    if (value > health->max_health) {
        health->health = health->max_health;
    } else if (value < 0) {
        health->health = 0;
    } else {
        health->health = value;
    }
    return health->health;
}

float health_add(health_ct* health, float value) {
    health->health += value;
    if (health->health > health->max_health) {
        health->health = health->max_health;
    }
    return health->health;
}

float health_sub(health_ct* health, float value) {
    health->health -= value;
    if (health->health < 0) {
        health->health = 0;
    }
    return health->health;
}

float health_reset(health_ct* health) {
    health->health = health->max_health;
    return health->health;
}
#ifndef _LIB_DOOM_COMPONENT_HEALTH_H
#define _LIB_DOOM_COMPONENT_HEALTH_H

#include "../ecs/component.h"
#include <assert.h>

/// The tag for the health component.
extern const int COMPONENT_TAG_HEALTH;

/**
 * health_ct is a component that represents the health of an entity.
 * It contains information about the maximum health and the current health of
 * the entity.
 */
typedef struct {
  /// The tag of the component
  /// This should be COMPONENT_TAG_HEALTH
  int tag;
  /// The maximum health of the entity in health points.
  float max_health;
  /// The current health of the entity in health points.
  float health;
} health_ct;

/// health_create creates a new health component with the given max_health and
/// health.
component_t *health_create(float max_health, float health);

/// health_get returns the current health of the entity.
inline float health_get(health_ct *health) { return health->health; }

/// health_get_max returns the maximum health of the entity.
inline float health_get_max(health_ct *health) { return health->max_health; }

/// health_set sets the current health of the entity.
inline float health_set(health_ct *health, float value) {
  if (value > health->max_health) {
    health->health = health->max_health;
  } else if (value < 0) {
    health->health = 0;
  } else {
    health->health = value;
  }
  return health->health;
}

/// health_set_max sets the maximum health of the entity.
inline float health_set_max(health_ct *health, float value) {
  assert(value >= 0);
  health->max_health = value;
  return health->max_health;
}

/// health_add adds the given value to the current health of the entity.
///
/// If the health exceeds the maximum health, it is set to the maximum health.
inline float health_add(health_ct *health, float value) {
  health->health += value;
  if (health->health > health->max_health) {
    health->health = health->max_health;
  }
  return health->health;
}

/// health_sub subtracts the given value from the current health of the entity.
///
/// If the health goes below 0, it is set to 0.
inline float health_sub(health_ct *health, float value) {
  health->health -= value;
  if (health->health < 0) {
    health->health = 0;
  }
  return health->health;
}

/// health_reset resets the current health of the entity to the maximum health.
inline float health_reset(health_ct *health) {
  health->health = health->max_health;
  return health->health;
}

#endif
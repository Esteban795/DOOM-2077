#ifndef _LIB_DOOM_COMPONENT_POSITION_H
#define _LIB_DOOM_COMPONENT_POSITION_H

#include "../ecs/component.h"
#include "../vec2.h"
#include <stdbool.h>

#define WALK_SOUND_COOLDOWN 1000

/// The tag of the position component
extern const int COMPONENT_TAG_POSITION;

/**
 * position_ct is a component that represents the position of an entity.
 * It contains information about the x, y, and z coordinates of the entity, as
 * well as the angle of the entity.
 */
typedef struct {
  /// The tag of the component  
  /// This should be COMPONENT_TAG_POSITION
  int tag;
  /// The x coordinate of the entity
  double x;
  /// The y coordinate of the entity
  double y;
  /// The z coordinate of the entity
  double z;
  /// The angle of the entity in degrees
  double angle;
  /// The previous known x coordinate of the entity
  double last_x;
  /// The previous known y coordinate of the entity
  double last_y;
  /// The previous known z coordinate of the entity
  double last_z;
  /// The previous known angle of the entity in degrees
  double last_angle;
  /// A flag indicating if the position was updated recently.
  bool was_updated;
  /// The time in milliseconds that must pass before the entity can play a walking sound again.
  int walk_cooldown;
} position_ct;

/// Create a new position component
component_t *position_create(double coords[3], double angle);

/// Get the position of the entity
inline vec2 position_get_pos(position_ct *component) {
  vec2 pos = {component->x, component->y};
  return pos;
}

/// Get the previous known position of the entity
inline vec2 position_get_last_pos(position_ct *component) {
  vec2 pos = {component->last_x, component->last_y};
  return pos;
}

/// Get the x coordinate of the entity
inline double position_get_x(position_ct *component) { return component->x; }

/// Get the previous known x coordinate of the entity
inline double position_get_last_x(position_ct *component) {
  return component->last_x;
}

/// Get the y coordinate of the entity
inline double position_get_y(position_ct *component) { return component->y; }

/// Get the previous known y coordinate of the entity
inline double position_get_last_y(position_ct *component) {
  return component->last_y;
}

/// Get the z coordinate of the entity
inline double position_get_z(position_ct *component) { return component->z; }

/// Get the previous known z coordinate of the entity
inline double position_get_last_z(position_ct *component) {
  return component->last_z;
}

/// Get the angle of the entity
inline double position_get_angle(position_ct *component) {
  return component->angle;
}

/// Get the previous known angle of the entity
inline double position_get_last_angle(position_ct *component) {
  return component->last_angle;
}

/// Set the position (x,y) of the entity
inline vec2 position_set_pos(position_ct *component, vec2 pos) {
  component->x = pos.x;
  component->y = pos.y;
  return pos;
}

/// Set the previous known XY position of the entity
inline vec2 position_set_last_pos(position_ct *component, vec2 pos) {
  component->last_x = pos.x;
  component->last_y = pos.y;
  return pos;
}

/// Set the z coordinate of the entity
inline double position_set_z(position_ct *component, double z) {
  component->z = z;
  return z;
}

/// Set the previous known z coordinate of the entity
inline double position_set_last_z(position_ct *component, double z) {
  component->last_z = z;
  return z;
}

/// Set the angle of the entity
inline double position_set_angle(position_ct *component, double angle) {
  component->angle = angle;
  return angle;
}

/// Set the previous known angle of the entity
inline double position_set_last_angle(position_ct *component, double angle) {
  component->last_angle = angle;
  return angle;
}

/// Determine if the position has changed from the last known position.
inline bool position_has_moved(position_ct *component) {
  return component->x != component->last_x || component->y != component->last_y;
}

/// Get the walk cooldown of the entity.
///
/// The walk cooldown is the time in milliseconds that must pass before the entity
/// can play a walking sound again.
inline int position_get_walk_cooldown(position_ct *component) {
  return component->walk_cooldown;
}

/// Set the walk cooldown of the entity.
///
/// The walk cooldown is the time in milliseconds that must pass before the entity
/// can play a walking sound again.
inline int position_set_walk_cooldown(position_ct *component, int cooldown) {
  component->walk_cooldown = cooldown;
  return cooldown;
}
#endif
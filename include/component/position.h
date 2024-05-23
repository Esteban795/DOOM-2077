#ifndef _LIB_DOOM_COMPONENT_POSITION_H
#define _LIB_DOOM_COMPONENT_POSITION_H

#include "../ecs/component.h"
#include "../vec2.h"
#include <stdbool.h>

#define WALK_SOUND_COOLDOWN 1000
extern const int COMPONENT_TAG_POSITION;

/*
 * position_ct is a component that represents the position of an entity.
 * It contains information about the x, y, and z coordinates of the entity, as
 * well as the angle of the entity.
 */
typedef struct {
  int tag;
  double x;
  double y;
  double z;
  double angle;
  double last_x;
  double last_y;
  double last_z;
  double last_angle;
  bool was_updated;
  int walk_cooldown;
} position_ct;

// Create a new position component
component_t *position_create(double coords[3], double angle);

// Get the position of the entity
inline vec2 position_get_pos(position_ct *component) {
  vec2 pos = {component->x, component->y};
  return pos;
}

inline vec2 position_get_last_pos(position_ct *component) {
  vec2 pos = {component->last_x, component->last_y};
  return pos;
}

// Get the x coordinate of the entity
inline double position_get_x(position_ct *component) { return component->x; }

inline double position_get_last_x(position_ct *component) {
  return component->last_x;
}

// Get the y coordinate of the entity
inline double position_get_y(position_ct *component) { return component->y; }

inline double position_get_last_y(position_ct *component) {
  return component->last_y;
}

// Get the z coordinate of the entity
inline double position_get_z(position_ct *component) { return component->z; }

inline double position_get_last_z(position_ct *component) {
  return component->last_z;
}

// Get the angle of the entity
inline double position_get_angle(position_ct *component) {
  return component->angle;
}

inline double position_get_last_angle(position_ct *component) {
  return component->last_angle;
}

// Set the position (x,y) of the entity
inline vec2 position_set_pos(position_ct *component, vec2 pos) {
  component->x = pos.x;
  component->y = pos.y;
  return pos;
}

inline vec2 position_set_last_pos(position_ct *component, vec2 pos) {
  component->last_x = pos.x;
  component->last_y = pos.y;
  return pos;
}

// Set the z coordinate of the entity
inline double position_set_z(position_ct *component, double z) {
  component->z = z;
  return z;
}

inline double position_set_last_z(position_ct *component, double z) {
  component->last_z = z;
  return z;
}

// Set the angle of the entity
inline double position_set_angle(position_ct *component, double angle) {
  component->angle = angle;
  return angle;
}

inline double position_set_last_angle(position_ct *component, double angle) {
  component->last_angle = angle;
  return angle;
}

inline bool position_has_moved(position_ct *component) {
  return component->x != component->last_x || component->y != component->last_y;
}

inline int position_get_walk_cooldown(position_ct *component) {
  return component->walk_cooldown;
}

inline int position_set_walk_cooldown(position_ct *component, int cooldown) {
  component->walk_cooldown = cooldown;
  return cooldown;
}
#endif
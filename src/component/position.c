#include <stdlib.h>

#include "../../include/component/position.h"
#include "../../include/ecs/component.h"
#include "../../include/util/vec2.h"

const int COMPONENT_TAG_POSITION = 2;

component_t *position_create(double coords[3], double angle) {
  position_ct *position_component = malloc(sizeof(position_ct));
  position_component->x = coords[0];
  position_component->y = coords[1];
  position_component->z = coords[2];
  position_component->last_x = coords[0];
  position_component->last_y = coords[1];
  position_component->last_z = coords[2];
  position_component->last_angle = angle;
  position_component->angle = angle;
  position_component->tag = COMPONENT_TAG_POSITION;
  position_component->was_updated = true;
  position_component->walk_cooldown =
      -1; // will create a sound when first joining the server :D
  return (component_t *)position_component;
}

extern inline vec2 position_get_pos(position_ct *component);
extern inline vec2 position_get_last_pos(position_ct *component);
extern inline double position_get_x(position_ct *component);
extern inline double position_get_last_x(position_ct *component);
extern inline double position_get_y(position_ct *component);
extern inline double position_get_last_y(position_ct *component);
extern inline double position_get_z(position_ct *component);
extern inline double position_get_last_z(position_ct *component);
extern inline double position_get_angle(position_ct *component);
extern inline double position_get_last_angle(position_ct *component);
extern inline vec2 position_set_pos(position_ct *component, vec2 pos);
extern inline vec2 position_set_last_pos(position_ct *component, vec2 pos);
extern inline double position_set_z(position_ct *component, double z);
extern inline double position_set_last_z(position_ct *component, double z);
extern inline double position_set_angle(position_ct *component, double angle);
extern inline double position_set_last_angle(position_ct *component,
                                             double angle);
extern inline bool position_has_moved(position_ct *component);
extern inline int position_get_walk_cooldown(position_ct *component);
extern inline int position_set_walk_cooldown(position_ct *component,
                                             int cooldown);
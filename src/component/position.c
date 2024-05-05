#include <stdlib.h>

#include "../../include/ecs/component.h"
#include "../../include/component/position.h"
#include "../../include/vec2.h"

const int COMPONENT_TAG_POSITION = 2;

component_t* position_create(double coords[3], double angle) {
    position_ct* position_component = malloc(sizeof(position_ct));
    position_component->x = coords[0];
    position_component->y = coords[1];
    position_component->z = coords[2];
    position_component->angle = angle;
    position_component->tag = COMPONENT_TAG_POSITION;
    return (component_t*) position_component;
}

extern inline vec2 position_get_pos(position_ct* component);
extern inline double position_get_x(position_ct* component);
extern inline double position_get_y(position_ct* component);
extern inline double position_get_z(position_ct* component);
extern inline double position_get_angle(position_ct* component);
extern inline vec2 position_set_pos(position_ct* component, vec2 pos);
extern inline double position_set_z(position_ct* component, double z);
extern inline double position_set_angle(position_ct* component, double angle);
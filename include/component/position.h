#ifndef _LIB_DOOM_COMPONENT_POSITION_H
#define _LIB_DOOM_COMPONENT_POSITION_H

#include "../ecs/component.h"
#include "../vec2.h"

extern const int COMPONENT_TAG_POSITION;

/*
* position_ct is a component that represents the position of an entity.
* It contains information about the x, y, and z coordinates of the entity, as well as the angle of the entity.
*/
typedef struct {
    int tag;
    double x;
    double y;
    double z;
    double angle; 
} position_ct;

// Create a new position component
component_t* position_create(double coords[3], double angle);

// Get the position vector (x, y) of the entity
vec2 position_get_pos(position_ct* component);

// Get the x coordinate
double position_get_x(position_ct* component);

// Get the y coordinate
double position_get_y(position_ct* component);

// Get the z coordinate
double position_get_z(position_ct* component);

// Get the angle
double position_get_angle(position_ct* component);

// Set the position of the entity
vec2 position_set_pos(position_ct* component, vec2 pos);

// Set the z coordinate of the entity (elevation)
double position_set_z(position_ct* component, double z);

// Set the angle of the entity
double position_set_angle(position_ct* component, double angle);

#endif
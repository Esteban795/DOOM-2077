#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "settings.h"
#include "vec2.h"
#include <math.h>

double rad_to_deg(double rad);

int angle_to_x_pos(double angle);

double point_to_angle(vec2 p1, vec2 p2);

float norm(double angle);

double deg_to_rad(double deg);
#endif
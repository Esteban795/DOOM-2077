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

double dist(vec2 pos1, vec2 pos2);

double fmin(double x, double y);

double fmax(double x, double y);

int max(int x, int y);

int min(int x, int y);
#endif

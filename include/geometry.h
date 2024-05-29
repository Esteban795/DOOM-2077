#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "settings.h"
#include "vec2.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SCALE 64.0
#define MIN_SCALE 0.00390625

double rad_to_deg(double rad);

int angle_to_x_pos(double angle);

double point_to_angle(vec2 p1, vec2 p2);

float norm(double angle);

double mod(double f,double mod);

double deg_to_rad(double deg);

double dist(vec2 pos1, vec2 pos2);

double fmin(double x, double y);

double fmax(double x, double y);

int max(int x, int y);

int min(int x, int y);

bool is_point_in_FOV(double origin_x, double origin_y, double origin_angle,
                     double fov, double px, double py);

vec2 find_segments_intersection(vec2 p1, vec2 q1, vec2 p2, vec2 q2);

bool do_segs_intersect(int x1a, int x2a, int x1b, int x2b);

double distance(double posx_a, double posy_a, double posx_b, double posy_b);

double get_angular_distance(double origin_x, double origin_y, double origin_angle, double px, double py);

int rem(int a, int b);
#endif

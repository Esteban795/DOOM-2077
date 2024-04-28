#include "../include/geometry.h"
#include <stdio.h>

double rad_to_deg(double rad) { return rad * (180 / M_PI); }

double deg_to_rad(double deg) { return deg * (M_PI / 180); }
/*
Project point coordinate in SCREEN_DISTANCE, according to the angle
they form with player's POV
*/
int angle_to_x_pos(double angle) {
  if (angle > 0) {
    return (int)(SCREEN_DISTANCE - tan(deg_to_rad(angle)) * HALF_WIDTH);
  } else {
    return (int)(-tan(deg_to_rad(angle)) * HALF_WIDTH + SCREEN_DISTANCE);
  }
}

/*
Get the angle from p1 to p2
*/
double point_to_angle(vec2 p1, vec2 p2) {
  vec2 delta = {p2.x - p1.x, p2.y - p1.y};
  return rad_to_deg(atan2(delta.y, delta.x));
}

// makes sure we stay in [0,360[
float norm(double angle) { return fmod((fmod(angle, 360.0) + 360.0), 360.0); }

double mod(double f, double mod) { return fmod((fmod(f, mod) + mod), mod); }

double dist(vec2 pos1, vec2 pos2) {
  return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) +
              (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

double fmin(double x, double y) { return x < y ? x : y; }
double fmax(double x, double y) { return x > y ? x : y; }

int max(int x, int y) { return x > y ? x : y; }
int min(int x, int y) { return x < y ? x : y; }


bool is_point_in_FOV(double origin_x, double origin_y, double origin_angle,
                     double fov, double px, double py) {
  // Direction vector from camera to point
  origin_angle = deg_to_rad(origin_angle);
  fov = deg_to_rad(fov);
  double dx = px - origin_x;
  double dy = py - origin_y;

  // Calculate the angle between camera orientation and the point
  double theta_p = atan2(-dy, dx); // Angle from camera to point

  // Angular distance between camera angle and point
  double delta_theta = fmin(fabs(theta_p - origin_angle),mod(fabs(2 * M_PI + theta_p - origin_angle),2 * M_PI));
  
  // Ensure angles are within the FOV range (0 to 2*PI)
  delta_theta = mod(delta_theta, 2 * M_PI);
  return delta_theta <= fov / 2;
}

double scale_from_global_angle(double angle, int x, double normal_angle,
                               double dist) {

  double x_angle = rad_to_deg(atan((HALF_WIDTH - x) / SCREEN_DISTANCE));
  double num =
      fabs(SCREEN_DISTANCE * cos(deg_to_rad(-normal_angle + x_angle - angle)));
  double den = dist * cos(deg_to_rad(x_angle));
  double scale = num / den;
  scale = fmin(MAX_SCALE, fmax(MIN_SCALE, scale));
  return scale;
}

// Given endpoints of each segment, returns the coordinates of the point of
// intersection . /!\ assumes that the segments intersect
vec2 find_segments_intersection(vec2 p1, vec2 q1, vec2 p2, vec2 q2) {
  vec2 intersection;
  float A1 = q1.y - p1.y;
  float B1 = p1.x - q1.x;
  float C1 = A1 * p1.x + B1 * p1.y;

  float A2 = q2.y - p2.y;
  float B2 = p2.x - q2.x;
  float C2 = A2 * p2.x + B2 * p2.y;

  float determinant = A1 * B2 - A2 * B1;
  intersection.x = (B2 * C1 - B1 * C2) / determinant;
  intersection.y = (A1 * C2 - A2 * C1) / determinant;
  return intersection;
}
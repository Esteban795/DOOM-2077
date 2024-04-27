#include "../include/geometry.h"

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
float norm(double angle) {
  return fmod((fmod(angle, 360.0) + 360.0),360.0);
}

double mod(double f,double mod) {
  return fmod((fmod(f, mod) + mod),mod);
}

double dist(vec2 pos1, vec2 pos2){
  return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

double fmin(double x, double y){
  return x < y ? x : y;
}
double fmax(double x, double y){
  return x > y ? x : y;
}

int max(int x, int y){
  return x > y ? x : y;
}
int min(int x, int y){
  return x < y ? x : y;
}

bool is_point_in_FOV(double origin_x, double origin_y, double origin_angle,
                     double fov, double px, double py) {
  // Direction vector from camera to point
  origin_angle = deg_to_rad(origin_angle);
  fov = deg_to_rad(fov);
  double dx = px - origin_x;
  double dy = py - origin_y;

  // Calculate the angle between camera orientation and the point
  double theta_p = atan2(-dy, dx); // Angle from camera to point
  theta_p = mod(theta_p, 2 * M_PI);
  origin_angle = mod(origin_angle, 2 * M_PI);

  // Angular distance between camera angle and point
  double delta_theta = fabs(theta_p - origin_angle);

  // Ensure angles are within the FOV range (0 to 2*PI)
  delta_theta = mod(delta_theta, 2 * M_PI);
  return delta_theta <= fov / 2;
};
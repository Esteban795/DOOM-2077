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

double distance(double posx_a, double posy_a, double posx_b, double posy_b) {
  return sqrt(pow((posx_b - posx_a), 2) + pow((posy_b - posy_a), 2));
}

double get_angular_distance(double origin_x, double origin_y, double origin_angle, double px, double py) {
  origin_angle = deg_to_rad(origin_angle);
  double dx = px - origin_x;
  double dy = py - origin_y;
  double theta_p = atan2(-dy, dx);
  double delta_theta = theta_p - origin_angle;
  if (delta_theta < 0) {
    delta_theta += 2 * M_PI;
  } else if (delta_theta > 2 * M_PI) {
    delta_theta -= 2 * M_PI;
  }
  return delta_theta;
}
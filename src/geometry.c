#include "../include/geometry.h"

double rad_to_deg(double rad) { return rad * (180 / M_PI); }

double deg_to_rad(double deg) { return deg * (M_PI / 180); }
/*
Project point coordinate in SCREEN_DISTANCE, according to the angle
they form with player's POV
*/
int angle_to_x_pos(double angle) {
  if (angle > 0) {
    return (int)(SCREEN_DISTANCE - tan(deg_to_rad(angle)) * (double)WIDTH / 2);
  } else {
    return (int)(tan(deg_to_rad(angle)) * (double)WIDTH / 2 + SCREEN_DISTANCE);
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
float norm(double angle) { return fmod((angle + 360.0), 360.0); }

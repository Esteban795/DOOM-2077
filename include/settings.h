#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define M_PI 3.14159265358979323846
#define HALF_M_PI 1.57079632679489661923
#define FOV 90.0
#define HALF_FOV (FOV / 2.0)

#define RES_W 320
#define RES_H 200
#define SCALE 5

#define WIDTH RES_W *SCALE
#define HEIGHT RES_H *SCALE
#define HALF_WIDTH (WIDTH / 2.0)
#define HALF_HEIGHT (HEIGHT / 2.0)
#define OUT_MIN 30
#define OUT_MAX_W (WIDTH - 30)
#define OUT_MAX_H (HEIGHT - 30)

#define G 9.81
#define SCREEN_DISTANCE (HALF_WIDTH / tan(HALF_FOV * (M_PI / 180.0)))

#define DEFAULT_MAP_BOUNDS                                                     \
  { OUT_MIN, OUT_MAX_W, OUT_MIN, OUT_MAX_H }

#define SUBSECTOR_IDENTIFIER (size_t)0x8000

#define PLAYER_HEIGHT 56
#define PLAYER_STEP 24
#define PLAYER_RADIUS 6
#define PLAYER_SPEED .3
#define PLAYER_ROTATION_SPEED .2
#define MOUSE_SENSITIVITY 10
#define DIAGONAL_CORRECTION 1 / sqrt(2)

#define WEAPONS_NUMBER 2 


#endif
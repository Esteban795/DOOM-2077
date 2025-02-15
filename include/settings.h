#ifndef SETTINGS_H
#define SETTINGS_H

#include "util/vec2.h"
#include <stdint.h>

#define M_PI 3.14159265358979323846
#define HALF_M_PI 1.57079632679489661923
#define FOV 90.0
#define HALF_FOV (FOV / 2.0)

#define RES_W 320
#define RES_H 200
// #define SCALE 5

// #define WIDTH RES_W *SCALE
// #define HEIGHT RES_H *SCALE

#define WIDTH 800
#define HEIGHT 600
#define HALF_WIDTH (WIDTH / 2.0)
#define HALF_HEIGHT (HEIGHT / 2.0)
#define OUT_MIN 30
#define OUT_MAX_W (WIDTH - 30)
#define OUT_MAX_H (HEIGHT - 30)

#define X_SCALE WIDTH/RES_W
#define Y_SCALE HEIGHT/RES_H

#define G 9.81
#define SCREEN_DISTANCE (HALF_WIDTH / tan(HALF_FOV * (M_PI / 180.0)))

#define DEFAULT_MAP_BOUNDS                                                     \
  { OUT_MIN, OUT_MAX_W, OUT_MIN, OUT_MAX_H }

#define SUBSECTOR_IDENTIFIER (size_t)0x8000

#define PLAYER_HEIGHT 41
#define PLAYER_STEP 24
#define PLAYER_RADIUS 12
#define PLAYER_SPEED .3
#define PLAYER_ROTATION_SPEED .2
#define PLAYER_MAXIMUM 4
#define MOUSE_SENSITIVITY 10
#define DIAGONAL_CORRECTION 1 / sqrt(2)

#define WEAPONS_NUMBER 4 


#define AUDIO_SAMPLE_RATE 11025
#define AUDIO_MIXER_CHANNELS 16

#define AL_REFERENCE_DISTANCE 64
#define AL_ROLLOFF_FACTOR 1
#define AL_MAX_DISTANCE 1024

#define NUM_PLAYERS 3

#endif

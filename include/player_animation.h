#ifndef PLAYER_ANIMATION_H
#define PLAYER_ANIMATION_H

#include "vec2.h"
#include "geometry.h"
#include "settings.h"

enum AnimationType {
    PLAYER_IDLE,
    MOVING,
    SHOOTING,
    DYING,
    EXTREME_DYING,
};

#define NUMBER_ORIENTATION 8

#define ANIMATION_WALK_INIT 'A'
#define ANIMATION_WALK_END 'D'
#define ANIMATION_WALK_FRAME_COUNT (ANIMATION_WALK_END - ANIMATION_WALK_INIT + 1)

#define ANIMATION_SHOOT_INIT 'E'
#define ANIMATION_SHOOT_END 'F'
#define ANIMATION_SHOOT_FRAME_COUNT (ANIMATION_SHOOT_END - ANIMATION_SHOOT_INIT + 1)

#define ANIMATION_IDLE_INIT 'E'
#define ANIMATION_IDLE_END 'E'
#define ANIMATION_IDLE_FRAME_COUNT (ANIMATION_IDLE_END - ANIMATION_IDLE_INIT + 1)

// DEATH ANIMATION DO NOT SEEM TO BE ORIENTATION DEPENDENT ??
#define ANIMATION_DEATH_INIT 'H'
#define ANIMATION_DEATH_END 'N'
#define ANIMATION_DEATH_FRAME_COUNT (ANIMATION_DEATH_END - ANIMATION_DEATH_INIT + 1)

#define ANIMATION_EXTREME_DEATH_INIT 'O'
#define ANIMATION_EXTREME_DEATH_END 'W'
#define ANIMATION_EXTREME_DEATH_FRAME_COUNT (ANIMATION_EXTREME_DEATH_END - ANIMATION_EXTREME_DEATH_INIT + 1)

#define ANIMATION_COOLDOWN 150
/*
String builder to find the animation that is going to be played next for the opponent
A player sprite name is in the following format : 
PLAYA1 : PLAY, base prefix, A because it is the first frame of the walk animation, 1 because the sprite faces us 

PLAYA4A6 : PLAY, base prefix, A because it is the first frame of the walk animation, 
4 because the sprite is facing left far away, and 6 because the sprite is facing right far away (it is the mirror of 4)
*/
extern char ANIMATION_NAME[9];

/*
Keeps track of the current playing animation
*/
extern enum AnimationType CURRENT_ANIMATION[NUM_PLAYERS];

/*
Keep tracks of what was the last frame of the current animation that was played,
will be set to -1 if the animation is over and 0 when switching to a new animation
*/
extern int CURRENT_ANIMATION_PROGRESS[NUM_PLAYERS];

extern int ANIMATION_COOLDOWNS[NUM_PLAYERS];

bool set_correct_animation_name(int i,vec2 origin_pos,double origin_angle, vec2 pos,double angle,enum AnimationType* type,bool has_moved);
#endif
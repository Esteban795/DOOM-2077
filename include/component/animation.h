#ifndef _LIB_DOOM_COMPONENT_ANIMATION_H
#define _LIB_DOOM_COMPONENT_ANIMATION_H

#include "../ecs/component.h"
#include <stdio.h>
#include <stdlib.h>
#include "../player_animation.h"

extern const int COMPONENT_TAG_ANIMATION;

/*
animation is a component that represents the current animation of an entity.
It has only a single field : animation, which is an enum defined in player_animation.h
The goal is to able to synchronize the animations through the network for better results !
*/

typedef struct {
    int tag;
    enum AnimationType animation;
} animation_ct;

component_t* animation_create(enum AnimationType animation);

inline enum AnimationType animation_get(animation_ct* animation) {
    return animation->animation;
}


/*
ANIMATION PRECEDENCE ORDER 
EXTREME_DYING > DYING> SHOOTING > MOVING > PLAYER_IDLE
*/
enum AnimationType animation_set(animation_ct* animation, enum AnimationType animation_value);

#endif
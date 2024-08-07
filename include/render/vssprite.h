#ifndef VSSPRITE_H
#define VSSPRITE_H

#include "../WAD/patch.h"
#include "../util/vec2.h"
#include "../util/geometry.h"
#include <SDL2/SDL_stdinc.h>

#define MAX_SPRITES 128

struct VSSprite {
    vec2 pos; // pos of the sprite in real world coordinates
    int x1; // projection onto screen (left side)
    int x2; // projection onto screen (right side)
    double scale; // rendering scale
    patch* sprite; 
    bool use_mirror;
    int shift;
    bool shot;
};

typedef struct VSSprite vs_sprite;

extern vs_sprite VSSPRITES[MAX_SPRITES];
extern int VSSPRITES_INDEX;

void vssprite_add(vec2 camera_pos, double angle, vec2 pos, patch *sprite, bool use_mirror,int shift, bool shot);

void vssprite_sort();
#endif
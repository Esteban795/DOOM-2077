#ifndef _LIB_DOOM_COMPONENT_WEAPON_H
#define _LIB_DOOM_COMPONENT_WEAPON_H

#include "../ecs/component.h"

extern const int COMPONENT_TAG_WEAPON;

// Weapon component
// Contains the entity's ammo, active weapon, and cooldown status.
typedef struct {
    int tag;
    unsigned int* ammunitions;
    unsigned int active_weapon;
    int cooldown;
} weapon_ct;

// Create a new weapon component
component_t* weapon_create(unsigned int* ammunitions);

#endif
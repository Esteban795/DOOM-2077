#ifndef _LIB_DOOM_COMPONENT_WEAPON_H
#define _LIB_DOOM_COMPONENT_WEAPON_H

#include "../ecs/component.h"
#include "../settings.h"
#include <stdlib.h>

#ifndef WEAPONS_NUMBER
#define WEAPONS_NUMBER 4
#endif

/// The tag for the weapon component.
extern const int COMPONENT_TAG_WEAPON;

/// Weapon component  
/// Contains the entity's ammo, bullets left in each clip of each weapon,
/// active weapon, and cooldown status
typedef struct {
    /// The tag of the component
    /// This should be COMPONENT_TAG_WEAPON
    int tag;
    /// The ammunitions left for each weapon
    int ammunitions[WEAPONS_NUMBER];
    /// The bullets left in each clip of each weapon
    int mags[WEAPONS_NUMBER];
    /// The cooldown time of each weapon (in milliseconds)
    int cooldowns[WEAPONS_NUMBER];
    /// The active weapon
    ///
    /// The active weapon is the weapon that the entity is currently using. It is 
    /// an index in the ammunitions, mags, and cooldowns arrays.
    unsigned int active_weapon;
} weapon_ct;

/// Create a new weapon component
component_t* weapon_create(int ammunitions[WEAPONS_NUMBER],int mags[WEAPONS_NUMBER]);

/// Get the cooldown linked to the active weapon
inline int weapon_get_active_cooldown(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return wp->cooldowns[wp->active_weapon];
    return 0;
}

/// Get a mutable pointer to the cooldown linked to the active weapon
inline int* weapon_get_mut_active_cooldown(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return &wp->cooldowns[wp->active_weapon];
    return NULL;
}

/// Get the ammunitions left for the active weapon
inline int weapon_get_active_ammos_left(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return wp->ammunitions[wp->active_weapon];
    return 0;
}

/// Get the bullets left in the clip of the active weapon
inline int weapon_get_active_bullets_left(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return wp->mags[wp->active_weapon];
    return 0;
}

/// Decrement the ammunitions of the active weapon
inline int weapon_decrease_active_ammos(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return --wp->ammunitions[wp->active_weapon];
    return 0;
}

/// Decrement the bullets left in the clip of the active weapon
inline int weapon_decrease_active_bullets(weapon_ct* comp) {
    weapon_ct* wp = (weapon_ct*) comp;
    if (wp->active_weapon < WEAPONS_NUMBER)
        return --wp->mags[wp->active_weapon];
    return 0;
}
#endif
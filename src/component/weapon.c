#include <stdlib.h>

#include "../../include/ecs/component.h"
#include "../../include/component/weapon.h"

const int COMPONENT_TAG_WEAPON = 3;

component_t* weapon_create(int ammunitions[WEAPONS_NUMBER]) {
    weapon_ct* weapon_component = malloc(sizeof(weapon_ct));
    for (int i = 0; i < WEAPONS_NUMBER; i++) {
        weapon_component->ammunitions[i] = ammunitions[i];
    }
    weapon_component->active_weapon = 0;
    weapon_component->cooldown = 0;
    weapon_component->tag = COMPONENT_TAG_WEAPON;
    return (component_t*) weapon_component;
}

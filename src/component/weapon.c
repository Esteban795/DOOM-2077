#include <stdlib.h>

#include "../../include/ecs/component.h"
#include "../../include/component/weapon.h"

const int COMPONENT_TAG_WEAPON = 5;

component_t* weapon_create(unsigned int* ammunitions) {
    weapon_ct* weapon_component = malloc(sizeof(weapon_ct));
    weapon_component->ammunitions = ammunitions;
    weapon_component->active_weapon = 0;
    weapon_component->cooldown = 0;
    weapon_component->tag = COMPONENT_TAG_WEAPON;
    return (component_t*) weapon_component;
}

#include <stdio.h>

#include "../../include/server/player.h"

//TODO : Use the same constants as the client
#define SERVER_WEAPON_NUMBER 2 
#define SERVER_PLAYER_HEIGHT 41.0

entity_t* server_create_player(world_t* world, char* name) {
    unsigned int* ammo = malloc(SERVER_WEAPON_NUMBER*sizeof(unsigned int));
    ammo[0] = -2;
    for (int i = 1; i < SERVER_WEAPON_NUMBER; i++) {
        ammo[i] = -1;
    }

    double coords[3] = {0.0, 0.0, SERVER_PLAYER_HEIGHT};
    component_t** comps = malloc(sizeof(component_t*) * 4);
    comps[0] = position_create(coords, 180.0);
    comps[1] = health_create(100.0, 100.0);
    comps[2] = weapon_create(ammo);
    comps[3] = display_name_create(name);
        
    entity_t* player = world_create_entity(world, comps, 4);
    return player;
}
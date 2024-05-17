#include <stdio.h>

#include "../../include/server/player.h"
#include "../../include/server/state.h"

//TODO : Use the same constants as the client
#define SERVER_WEAPON_NUMBER 2 
#define SERVER_PLAYER_HEIGHT 41.0

entity_t* server_create_player(world_t* world, char* name) {
    int ammo[WEAPONS_NUMBER];
    ammo[0] = -2;
    for (int i = 1; i < WEAPONS_NUMBER; i++) {
        ammo[i] = -1;
    }
    double xi = SERVER_STATE->wad_data->things[0].x;
    double yi = SERVER_STATE->wad_data->things[0].y;
    double anglei = SERVER_STATE->wad_data->things[0].angle;
    double coords[3] = {xi, yi, SERVER_PLAYER_HEIGHT};
    component_t** comps = malloc(sizeof(component_t*) * 5);
    comps[0] = position_create(coords, anglei);
    comps[1] = health_create(100.0, 100.0);
    comps[2] = weapon_create(ammo);
    comps[3] = display_name_create(name);
    comps[4] = statistic_create(0, 0);
        
    entity_t* player = world_create_entity(world, comps, 5);
    free(comps);
    return player;
}
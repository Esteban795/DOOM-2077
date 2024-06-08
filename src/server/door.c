#include "../../include/server/door.h"
#include "../../include/WAD/linedef.h"
#include "../../include/core/door.h"
#include "../../include/core/lift.h"

entity_t** server_world_load_doors(world_t* world, wad_data* wad, int* door_count) {
    door** doors = get_doors(wad->linedefs, wad->len_linedefs, door_count, wad->sectors, wad->len_sectors);
    component_t** components = malloc(sizeof(component_t*));
    entity_t** entities = malloc(sizeof(entity_t*) * (*door_count));
    for (int i = 0; i < *door_count; i++) {
        components[0] = door_state_create(i, doors[i]->init_state);
        entities[i] = world_create_entity(world, components, 1);
    }
    free(components);
    doors_free(doors, *door_count);
    return entities;
}

entity_t** server_world_load_lifts(world_t* world, wad_data* wad, int* lift_count) {
    lift** lifts = get_lifts(wad->linedefs, wad->len_linedefs, lift_count, wad->sectors, wad->len_sectors);
    component_t** components = malloc(sizeof(component_t*));
    entity_t** entities = malloc(sizeof(entity_t*) * (*lift_count));
    for (int i = 0; i < *lift_count; i++) {
        components[0] = lift_state_create(i, lifts[i]->init_state);
        entities[i] = world_create_entity(world, components, 1);
    }
    free(components);
    lifts_free(lifts, *lift_count);
    return entities;
}

void server_world_unload_doors(world_t* world, entity_t** doors, int door_count) {
    for (int i = 0; i < door_count; i++) {
        world_remove_entity(world, doors[i]);
    }
    free(doors);
    doors = NULL;
}

void server_world_unload_lifts(world_t* world, entity_t** lifts, int lift_count) {
    // Exactly the same as server_world_unload_doors
    server_world_unload_doors(world, lifts, lift_count);
}
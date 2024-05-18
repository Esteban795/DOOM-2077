#include "../../include/server/door.h"
#include "../../include/linedef.h"
#include "../../include/collection/vec.h"

entity_t** server_world_load_doors(world_t* world, wad_data* wad, int* door_count) {
    *door_count = get_doors_count(wad->linedefs, wad->len_linedefs, wad->sectors, wad->len_sectors);
    component_t*** components = malloc(sizeof(component_t**) * (*door_count));
    for (int i = 0; i < *door_count; i++) {
        components[i] = malloc(sizeof(component_t*) * 1);
        components[i][0] = door_state_create(i, false);
    }
    entity_t** entities = world_create_bulk_entity(world, components, 1, *door_count);
    free(components);
    return entities;
}

entity_t** server_world_load_lifts(world_t* world, wad_data* wad, int* lift_count) {
    *lift_count = get_lifts_count(wad->linedefs, wad->len_linedefs, wad->sectors, wad->len_sectors);
    component_t*** components = malloc(sizeof(component_t**) * (*lift_count));
    for (int i = 0; i < *lift_count; i++) {
        components[i] = malloc(sizeof(component_t*) * 1);
        components[i][0] = lift_state_create(i, false);
    }
    entity_t** entities = world_create_bulk_entity(world, components, 1, *lift_count);
    free(components);
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
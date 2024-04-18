#ifndef DOOR_H
#define DOOR_H

#include "byte_reader.h"
#include "ecs/entity.h"
#include "sector.h"
#include "geometry.h"


#define MINIMUM_FLOOR_HEIGHT -512

enum DoorTransitionSpeed { // percentage of total height achieved per second
    NO_SPEED = 0,
    SLOW = 1,
    FAST = 2,
    TURBO = 3,
};

enum DoorFunction { //how the door behaves
    OPEN_WAIT_CLOSE,
    OPEN_STAY_OPEN,
    CLOSE_STAY_CLOSED,
    CLOSE_WAIT_OPEN
};

struct Door {
    entity_t* id;
    enum DoorTransitionSpeed speed;
    enum DoorFunction function;
    int wait_time; //wait time until the door switches to the next state
    int time_elapsed; //time elapsed since the door started moving
    bool is_open;
    bool is_switching;
    i16 delta_height;
    i16 max_height;
    sector* sector;
};

typedef struct Door door;

extern door* COLLISIONNED_DOOR;

door *door_create(entity_t *id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, bool is_open,
                  sector *sector);

void door_timeout(door *d, int DT);

void door_update(door *d, int DT);

void door_print(door* d);

void door_update_height(door *d, int delta_height);

void door_trigger_switch(door *d);

void doors_free(door** doors,int len_doors);
#endif
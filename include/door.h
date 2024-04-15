#ifndef DOOR_H
#define DOOR_H

#include "byte_reader.h"
#include "ecs/entity.h"
#include "sector.h"
#include "geometry.h"

enum DoorTransitionSpeed { // percentage of total height achieved per second
    SLOW = 10,
    FAST = 20,
    TURBO = 30,
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
    sector* sector;
};

typedef struct Door door;

extern door* COLLISIONNED_DOOR;
#endif
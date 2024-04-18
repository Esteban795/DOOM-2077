#include "../include/door.h"
#include <stdio.h>

door *COLLISIONNED_DOOR = NULL;

door *door_create(entity_t *id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, bool is_open,
                  sector *sector) {
  door *d = malloc(sizeof(door));
  d->id = id;
  d->speed = speed;
  d->function = function;
  d->wait_time = wait_time;
  d->time_elapsed = 0;
  d->is_open = is_open;
  d->is_switching = false;
  d->delta_height = MINIMUM_FLOOR_HEIGHT;
  d->max_height = sector->ceiling_height;
  d->sector = sector;
  return d;
}

void door_trigger_switch(door *d) {
  d->is_switching = true;
}

void door_update(door *d, int DT) {
  if (d->speed == NO_SPEED) {
    return;
  }
  if (d->is_open) {
    d->time_elapsed += DT;
    if (d->time_elapsed >= d->wait_time) {
      d->time_elapsed = 0;
      door_trigger_switch(d);
    }
  }
  if (d->is_switching) {
    int max_height = d->max_height;
    int min_height = d->sector->floor_height;
    double multiplier =
        d->is_open ? -1 : 1; // determines which side the wall is going
    double height_gained = multiplier * d->speed * d->delta_height * DT / 500;
    int new_height = (int)(d->sector->ceiling_height + height_gained);
    d->sector->ceiling_height =
        max(min_height,
            min(max_height, new_height)); // make sure we don't get under or
                                          // over the target heights
    if (new_height <= min_height ||
        new_height >= max_height) { // wall is at the top or bottom
      d->is_switching = false;
      d->is_open = !d->is_open; // transition is done , switch state
      d->sector->ceiling_height = d->is_open ? max_height : min_height;
      if (d->function == OPEN_STAY_OPEN ||
          d->function == CLOSE_STAY_CLOSED) { // one time transition
        d->speed = NO_SPEED;
      }
    }
  }
}

void door_update_height(door *d, int delta_height) {
  d->delta_height = delta_height;
  d->max_height = d->sector->ceiling_height + delta_height;
}

void door_print(door *d) {
  printf("Door: %p\n", (void *)d);
  printf("speed: %d\n", d->speed);
  printf("function: %d\n", d->function);
  printf("wait_time: %d\n", d->wait_time);
  printf("time_elapsed: %d\n", d->time_elapsed);
  printf("is_open: %d\n", d->is_open);
  printf("is_switching: %d\n", d->is_switching);
  printf("delta_height: %d\n", d->delta_height);
  printf("ceiling_height: %d\n", d->sector->ceiling_height);
  printf("floor_height: %d\n", d->sector->floor_height);
  printf("light level : %d\n", d->sector->light_level);
  printf("\n");
}

void doors_free(door** doors,int len_doors){
  for(int i = 0; i < len_doors; i++){
    free(doors[i]);
  }
  free(doors);
}
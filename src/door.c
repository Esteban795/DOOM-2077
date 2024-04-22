#include "../include/door.h"
#include <stdbool.h>
#include <stdio.h>

door *COLLISIONNED_DOOR = NULL;

door *door_create(entity_t *id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, bool is_collidable,
                  bool is_shootable, bool is_repeatable,sector *sector) {
  door *d = malloc(sizeof(door));
  d->id = id;
  d->speed = speed;
  d->function = function;
  d->wait_time = wait_time;
  d->time_elapsed = 0;
  d->is_open = false;
  d->is_colllidable = is_collidable;
  d->is_shootable = is_shootable;
  d->is_switching = false;
  d->delta_height = MINIMUM_FLOOR_HEIGHT;
  d->max_height = sector->ceiling_height;
  d->sector = sector;
  return d;
}

void door_trigger_switch(door *d) {
  if (!d->is_open)
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
      d->is_switching = true;
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

door *create_door_from_linedef(sidedef *sd, enum DoorTypes line_type) {
  door *d;
  enum DoorTransitionSpeed speed;
  bool repeatable;
  switch (line_type) {
    case CSC_SR_S:
    case CSC_SR_F:
      speed = line_type == CSC_SR_S ? SLOW : FAST;
      d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, false, false,true,sd->sector);
      break;
    case CSC_S1_S:
    case CSC_S1_F:
      speed = line_type == CSC_S1_S ? SLOW : FAST;
      d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, false, false,false,sd->sector);
      break;
    case CSC_WR_S:
    case CSC_WR_F:
      speed = line_type == CSC_WR_S ? SLOW : FAST;
      d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, true, false,true,sd->sector);
      break;
    case CSC_W1_S:
    case CSC_W1_F:
      speed = line_type == CSC_W1_S ? SLOW : FAST;
      d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, true, false,false,sd->sector);
      break;
    case CWO_SR_S:
    case CWO_S1_S:
      repeatable = line_type == CWO_SR_S;
      d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 30000, false, false,repeatable,sd->sector);
      break;
    case CWO_WR_S:
    case CWO_W1_S:
      repeatable = line_type == CWO_WR_S;
      d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 30000, true, false,repeatable,sd->sector);
      break;
    case OSO_P1_S:
    case OSO_P1_F:
    case OSO_WR_S:
    case OSO_WR_F:
    case OSO_W1_S:
    case OSO_W1_F:
      repeatable = line_type == OSO_WR_S || line_type == OSO_WR_S;
      speed = (line_type == OSO_P1_S || line_type == OSO_WR_S || line_type == OSO_W1_S) ? SLOW : FAST;
      d = door_create(NULL, speed, OPEN_STAY_OPEN, 0, true, false,repeatable,sd->sector);
      break;
    case OSO_SR_S:
    case OSO_SR_F:
    case OSO_S1_S:
    case OSO_S1_F:
      speed = (line_type == OSO_SR_S || line_type == OSO_S1_S) ? SLOW : FAST;
      repeatable = line_type == OSO_SR_S || line_type == OSO_SR_S;
      d = door_create(NULL, speed, OPEN_STAY_OPEN, 0, false, false,repeatable,sd->sector);
      break;
    case OSO_GR_S:
      d = door_create(NULL, SLOW, OPEN_STAY_OPEN, 0, false, true,false,sd->sector);
      break;
    case OWC_PR_S:
    case OWC_PR_F:
    case OWC_WR_S:
    case OWC_WR_F:
    case OWC_W1_S:
    case OWC_W1_F:
      repeatable = !(line_type == OWC_W1_S || line_type == OWC_W1_S);
      speed = (line_type == OWC_PR_S || line_type == OWC_WR_S || line_type == OWC_W1_S) ? SLOW : FAST;
      d = door_create(NULL, speed, OPEN_WAIT_CLOSE, 4000, true, false,repeatable,sd->sector);
      break;
    case OWC_SR_S:
    case OWC_SR_F:
    case OWC_S1_S:
    case OWC_S1_F:
      speed = (line_type == OWC_SR_S || line_type == OWC_S1_S) ? SLOW : FAST;
      repeatable = line_type == OWC_SR_S || line_type == OWC_SR_S;
      d = door_create(NULL, speed, OPEN_WAIT_CLOSE, 4000, true, false,repeatable,sd->sector);
      break;
    case DR_RED_KEY:
    case DR_YELLOW_KEY:
    case DR_BLUE_KEY:
      d = door_create(NULL, SLOW, OPEN_WAIT_CLOSE, 4000, true, true,false,sd->sector);
      break;
    default:
      printf("Linedef with unknown type:  %d\n", line_type);
      exit(1);
  }
  return d;
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

void doors_free(door **doors, int len_doors) {
  for (int i = 0; i < len_doors; i++) {
    free(doors[i]);
  }
  free(doors);
}
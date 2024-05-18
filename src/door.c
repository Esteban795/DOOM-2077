#include "../include/door.h"
#include <stdbool.h>
#include <stdio.h>

door *door_create(uint64_t id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, sector *sector,
                  bool init_state) {
  door *d = malloc(sizeof(door));
  d->id = id;
  d->speed = speed;
  d->function = function;
  d->wait_time = wait_time;
  d->time_elapsed = 0;
  d->state = init_state;
  d->init_state = init_state;
  d->is_switching = false;
  d->low_height = sector->floor_height;
  d->high_height = sector->ceiling_height;
  d->sector = sector;
  d->next_door = NULL;
  return d;
}

// trigger switch for all doors in the list, but they need to not be switching and be in their initial state (closed or open)
void door_trigger_switch(door *d) {
  if (d != NULL) {
    if (!d->is_switching && d->state == d->init_state) {
      d->is_switching = true;
    }
    if (d->next_door != NULL) {
      door_trigger_switch(d->next_door);
    }
  }
}

// Update door state
void door_update(door *d, int DT) {
  if (d->speed == NO_SPEED) { // door is not moving
    return;
  }

  if (d->state != d->init_state) { // update door's internal timer to know if it needs to switch
    d->time_elapsed += DT;
    if (d->time_elapsed >= d->wait_time) { // time's up, switch door
      d->time_elapsed = 0;
      d->is_switching = true;
    }
  }
  if (d->is_switching) {
    double multiplier;
    // if the door is going up, we need to add to the height, if it's going down, we need to subtract
    if (d->state == d->init_state) {
      if (d->init_state) {
        multiplier = -1;
      } else {
        multiplier = 1;
      }
    } else {
      if (d->init_state) {
        multiplier = 1;
      } else {
        multiplier = -1;
      }
    }
    double height_gained =
        multiplier * d->speed * (d->high_height - d->low_height) * DT / 500;
    int new_height = (int)(d->sector->ceiling_height + height_gained);
    d->sector->ceiling_height =
        max(d->low_height,
            min(d->high_height, new_height)); // make sure we don't get under or
                                              // over the target heights

    if (new_height <= d->low_height ||
        new_height >= d->high_height) { // wall is at the top or bottom
      d->is_switching = false;
      d->state = !d->state; // transition is done , switch state
      d->sector->ceiling_height = d->state ? d->high_height : d->low_height;
      if (d->function == OPEN_STAY_OPEN ||
          d->function == CLOSE_STAY_CLOSED) { // one time transition
        d->speed = NO_SPEED;
      }
    }
  }
}

void door_update_height(door *d, int high_height) {
  d->high_height = high_height;
}

void doors_free(door **doors, int len_doors) {
  for (int i = 0; i < len_doors; i++) {
    free(doors[i]);
  }
  free(doors);
}

door *add_door(door *head, door *new_door) {
  if (head == NULL)
    return new_door;
  new_door->next_door = head;
  return new_door;
}

bool is_a_door(u16 l) {
  return l == CSC_SR_S || l == CSC_SR_F || l == CSC_S1_S || l == CSC_S1_F ||
         l == CSC_WR_S || l == CSC_WR_F || l == CSC_W1_S || l == CSC_W1_F ||
         l == CWO_SR_S || l == CWO_S1_S || l == CWO_WR_S || l == CWO_W1_S ||
         l == OSO_P1_S || l == OSO_P1_F || l == OSO_WR_S || l == OSO_WR_F ||
         l == OSO_W1_S || l == OSO_W1_F || l == OSO_SR_S || l == OSO_SR_F ||
         l == OSO_S1_S || l == OSO_S1_F || l == OSO_GR_S || l == OWC_PR_S ||
         l == OWC_PR_F || l == OWC_SR_F || l == OWC_S1_S || l == OWC_S1_F ||
         l == OWC_WR_S || l == OWC_WR_F || l == OWC_W1_S || l == OWC_W1_F ||
         l == DR_RED_KEY || l == DR_YELLOW_KEY || l == DR_BLUE_KEY;
}

bool is_a_direct_door(
    u16 l, u16 sector_tag) { // linedef where a door is RIGHT behind it
  return !(sector_tag != 0 || l == CSC_SR_S || l == CSC_SR_F || l == CSC_S1_S ||
           l == CSC_S1_F || l == CWO_SR_S || l == CWO_S1_S || l == OSO_SR_S ||
           l == OSO_SR_F || l == OSO_S1_S || l == OSO_S1_F || l == OWC_SR_S ||
           l == OWC_SR_F || l == OWC_S1_S || l == OWC_S1_F);
}
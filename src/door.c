#include "../include/door.h"

door* COLLISIONNED_DOOR = NULL;

door *door_create(entity_t *id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, bool is_open,
                  i16 delta_height, sector *sector) {
  door *d = malloc(sizeof(door));
  d->id = id;
  d->speed = speed;
  d->function = function;
  d->wait_time = wait_time;
  d->is_open = is_open;
  d->delta_height = delta_height;
  d->sector = sector;
  return d;
}

void door_timeout(door *d, int DT) {
  d->time_elapsed += DT;
  if (d->time_elapsed > d->wait_time) {
    d->is_switching = true;
    d->time_elapsed = 0;
  }
}

void door_update(door *d, int DT) {
  door_timeout(d, DT);
  if (d->is_switching) {
    int max_height = d->sector->ceiling_height + d->delta_height;
    int min_height = d->sector->floor_height;
    int multiplier = d->is_open ? -1 : 1;
    int new_height = d->sector->ceiling_height + (d->speed / 100 * d->delta_height * DT * multiplier);
    d->sector->ceiling_height = max(min_height, min(max_height, new_height));
    if (new_height < min_height && new_height > max_height) {
      d->is_switching = false;
      d->is_open = !d->is_open;
    }
  }
}
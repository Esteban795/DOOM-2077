#ifndef _LIB_DOOM_COMPONENT_DOOR_STATE_H
#define _LIB_DOOM_COMPONENT_DOOR_STATE_H

#include "../ecs/component.h"
#include <stdbool.h>
#include <stdlib.h>

/// The tag for the door state component.
extern const int COMPONENT_TAG_DOOR_STATE;

/**
 * door_state_ct is a component that represents the current state of a door.
 * It is also linked the door entity (entity_id) to the actual door struct
 * (door_id).
 *
 * Door are either open or closed.
 * Door cannot be added programmatically, they are created by the map loader.
 * The order in which the doors are loaded MUST be the same on the client and
 * server. The `door_id` directly corresponds to the door loading order.
 */
typedef struct {
  /// The tag of the component
  /// This should be COMPONENT_TAG_DOOR_STATE
  int tag;
  /// The id of the door
  ///
  /// This id is used to link the door entity to the actual door struct.
  /// It is determined by the order in which the doors are loaded.
  unsigned int door_id;
  /// The state of the door
  bool is_open;
} door_state_ct;

/// door_state_create creates a new door state component with the given door_id
/// and state.
component_t *door_state_create(unsigned int door_id, bool is_open);

/// door_state_get_id returns the door_id of the given door_state component.
inline unsigned int door_state_get_id(component_t *c) {
  return ((door_state_ct *)c->data)->door_id;
}

/// door_state_is_open returns the state of the given door_state component.
inline bool door_state_is_open(component_t *c) {
  return ((door_state_ct *)c->data)->is_open;
}
#endif
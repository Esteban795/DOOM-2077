#include "../../include/event/door.h"

extern inline door_open_event_t* DoorOpenEvent_new(uint64_t door_id, bool is_lift);
extern inline door_close_event_t* DoorCloseEvent_new(uint64_t door_id, bool is_lift); 
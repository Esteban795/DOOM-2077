#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/player_move.h"

player_move_event_t* player_move_event_new(uint16_t tag, uint64_t entity_id, double x, double y, double z, double angle) {
    player_move_event_t* event = malloc(sizeof(player_move_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->x = x;
    event->y = y;
    event->z = z;
    event->angle = angle;
    return event;
}

player_move_event_t* ClientPlayerMoveEvent_new(uint64_t entity_id, double x, double y, double z, double angle) {
    return player_move_event_new(CLIENT_PLAYER_MOVE_EVENT_TAG, entity_id, x, y, z, angle);
}

player_move_event_t* ServerPlayerMoveEvent_new(uint64_t entity_id, double x, double y, double z, double angle) {
    return player_move_event_new(SERVER_PLAYER_MOVE_EVENT_TAG, entity_id, x, y, z, angle);
}
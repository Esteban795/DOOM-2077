#include "../../include/event/player_fire.h"

player_fire_event_t *PlayerFireEvent_new(uint16_t tag, uint64_t entity_id, int8_t weapon_id)
{
    player_fire_event_t *event = malloc(sizeof(player_fire_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->weapon_id = weapon_id;
    return event;
}

player_fire_event_t *ClientPlayerFireEvent_new(uint64_t entity_id, int8_t weapon_id)
{
    return PlayerFireEvent_new(CLIENT_PLAYER_FIRE_EVENT_TAG, entity_id, weapon_id);
}

player_fire_event_t *ServerPlayerFireEvent_new(uint64_t entity_id, int8_t weapon_id)
{
    return PlayerFireEvent_new(SERVER_PLAYER_FIRE_EVENT_TAG, entity_id, weapon_id);
}
#include "../../include/event/player_health.h"

player_health_event_t* PlayerHealthEvent_new(uint16_t tag, uint64_t entity_id, float hp, float max_hp) {
    player_health_event_t* event = malloc(sizeof(player_health_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->hp = hp;
    event->max_hp = max_hp;
    return event;
}

player_health_event_t* ClientPlayerHealthEvent_new(uint64_t entity_id, float hp, float max_hp) {
    return PlayerHealthEvent_new(CLIENT_PLAYER_HEALTH_EVENT_TAG, entity_id, hp, max_hp);
}

player_health_event_t* ServerPlayerHealthEvent_new(uint64_t entity_id, float hp, float max_hp) {
    return PlayerHealthEvent_new(SERVER_PLAYER_HEALTH_EVENT_TAG, entity_id, hp, max_hp);
}
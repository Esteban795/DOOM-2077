#include "../../include/event/player_heal.h"

player_heal_event_t* PlayerHealEvent_new(uint16_t tag, uint64_t entity_id, float gain) {
    player_heal_event_t* event = malloc(sizeof(player_heal_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->gain = gain;
    return event;
}

player_heal_event_t* ClientPlayerHealEvent_new(uint64_t entity_id, float gain) {
    return PlayerHealEvent_new(CLIENT_PLAYER_HEAL_EVENT_TAG, entity_id, gain);
}

player_heal_event_t* ServerPlayerHealEvent_new(uint64_t entity_id, float gain) {
    return PlayerHealEvent_new(SERVER_PLAYER_HEAL_EVENT_TAG, entity_id, gain);
}
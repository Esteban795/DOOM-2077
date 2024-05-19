#include "../../include/event/player_kill.h"

player_kill_event_t* PlayerKillEvent_new(uint16_t tag, uint64_t entity_id, uint64_t source_entity_id) {
    player_kill_event_t* event = malloc(sizeof(player_kill_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->source_entity_id = source_entity_id;
    return event;
}

player_kill_event_t* ClientPlayerKillEvent_new(uint64_t entity_id, uint64_t source_entity_id) {
    return PlayerKillEvent_new(CLIENT_PLAYER_KILL_EVENT_TAG, entity_id, source_entity_id);
}

player_kill_event_t* ServerPlayerKillEvent_new(uint64_t entity_id,  uint64_t source_entity_id) {
    return PlayerKillEvent_new(SERVER_PLAYER_KILL_EVENT_TAG, entity_id, source_entity_id);
}
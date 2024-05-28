#include "../../include/event/player_damage.h"

player_damage_event_t* PlayerDamageEvent_new(uint16_t tag, uint64_t entity_id, uint64_t source_entity_id, int8_t weapon_id, int damage) {
    player_damage_event_t* event = malloc(sizeof(player_damage_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->source_entity_id = source_entity_id;
    event->weapon_id = weapon_id;
    event->damage = damage;
    return event;
}

player_damage_event_t* ClientPlayerDamageEvent_new(uint64_t entity_id, uint64_t source_entity_id, int8_t weapon_id, int damage) {
    return PlayerDamageEvent_new(CLIENT_PLAYER_DAMAGE_EVENT_TAG, entity_id, source_entity_id, weapon_id, damage);
}

player_damage_event_t* ServerPlayerDamageEvent_new(uint64_t entity_id,  uint64_t source_entity_id, int8_t weapon_id, int damage) {
    return PlayerDamageEvent_new(SERVER_PLAYER_DAMAGE_EVENT_TAG, entity_id, source_entity_id, weapon_id, damage);
}
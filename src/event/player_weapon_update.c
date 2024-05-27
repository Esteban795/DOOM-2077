#include "../../include/event/player_weapon_update.h"

player_weapon_update_event_t *player_weapon_update_event_t_new(uint16_t tag, uint64_t entity_id, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER])
{
    player_weapon_update_event_t *event = malloc(sizeof(player_weapon_update_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    memcpy(event->ammunitions, ammunitions, sizeof(int) * WEAPONS_NUMBER);
    memcpy(event->mags, mags, sizeof(int) * WEAPONS_NUMBER);
    memcpy(event->cooldowns, cooldowns, sizeof(int) * WEAPONS_NUMBER);
    return event;
}

player_weapon_update_event_t *ClientPlayerWeaponUpdateEvent_new(uint64_t entity_id, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER])
{
    return player_weapon_update_event_t_new(CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG, entity_id, ammunitions, mags, cooldowns);
}

player_weapon_update_event_t *ServerPlayerWeaponUpdateEvent_new(uint64_t entity_id, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER])
{
    return player_weapon_update_event_t_new(SERVER_PLAYER_WEAPON_UPDATE_EVENT_TAG, entity_id, ammunitions, mags, cooldowns);
}
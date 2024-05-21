#include "../../../include/system/client/apply_event.h"
#include "../../../include/event/all.h"
#include "../../../include/component/position.h"
#include "../../../include/ecs/world.h"
#include "../../../include/component/health.h"


const system_t APPLY_EVENT_SYSTEM = {
    .fn = apply_event,
};

/*
* The first system in the chain, it applies the modifications to the world based on the event.
* For example, if the event is a player moving, this system will update the player's position in the world.
* This system is intented to be used on the client side, and ensure the world and events are consistent.
*/
int apply_event(world_t* world, event_t* event) {
    entity_t pid;
    switch (event->tag) {
        case CLIENT_PLAYER_MOVE_EVENT_TAG: {
            // On the client side, this event is handled on remote_update.
            break;
        }
        case CLIENT_PLAYER_HEALTH_EVENT_TAG: {
            player_health_event_t* client_player_health_event = (player_health_event_t*)event;
            pid = ENTITY_BY_ID(client_player_health_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_set(health, client_player_health_event->hp);
            health_set_max(health, client_player_health_event->max_hp);
            break;
        }
        case CLIENT_PLAYER_DAMAGE_EVENT_TAG: {
            player_damage_event_t* client_player_damage_event = (player_damage_event_t*)event;
            pid = ENTITY_BY_ID(client_player_damage_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_sub(health, client_player_damage_event->damage);
            break;
        }
        case CLIENT_PLAYER_HEAL_EVENT_TAG: {
            player_heal_event_t* client_player_heal_event = (player_heal_event_t*)event;
            pid = ENTITY_BY_ID(client_player_heal_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_add(health, client_player_heal_event->gain);
            break;
        }
        case CLIENT_PLAYER_KILL_EVENT_TAG: {
            player_kill_event_t* client_player_kill_event = (player_kill_event_t*)event;
            pid = ENTITY_BY_ID(client_player_kill_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_set(health, 0);
            break;
        }
        default:
            break;
    }
    return 0;
}

#include "../../../include/system/server/apply_event.h"
#include "../../../include/event/all.h"
#include "../../../include/component/position.h"
#include "../../../include/ecs/world.h"
#include "../../../include/component/display_name.h"
#include "../../../include/component/health.h"
#include "../../../include/component/statistic.h"
#include "../../../include/collection/vec.h"
#include "../../../include/server/state.h"
#include "../../../include/component/door_state.h"
#include "../../../include/component/lift_state.h"

const system_t APPLY_EVENT_SYSTEM = {
    .fn = apply_event,
};

/*
* The first system in the chain, it applies the modifications to the world based on the event.
* For example, if the event is a player moving, this system will update the player's position in the world.
* This system is intented to be used on the server side, and ensure the world and events are consistent.
*/
int apply_event(world_t* world, event_t* event) {
    switch (event->tag) {
        case SERVER_PLAYER_JOIN_EVENT_TAG: {
            // On join, we want to set their initial position and health.
            server_player_join_event_t* server_player_join_event = (server_player_join_event_t*)event;
            entity_t pid = ENTITY_BY_ID(server_player_join_event->entity_id);
            position_ct* pos = (position_ct*) world_get_component(world, &pid, COMPONENT_TAG_POSITION);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (pos == NULL || health == NULL) return -1; // If the player does not have a position or health, we cannot apply the event, cancel it.
            health_set(health, 100.0);
            health_set_max(health, 100.0);
            break;
        }
        case SERVER_PLAYER_MOVE_EVENT_TAG: {
            player_move_event_t* player_move_event = (player_move_event_t*)event;
            entity_t pid = ENTITY_BY_ID(player_move_event->entity_id);
            position_ct* pos = (position_ct*) world_get_component(world, &pid, COMPONENT_TAG_POSITION);
            if (pos == NULL) return -1; // If the player does not have a position, we cannot apply the event, cancel it.
            pos->x = player_move_event->x;
            pos->y = player_move_event->y;
            pos->z = player_move_event->z;
            pos->angle = player_move_event->angle;
            break;
        }
        case SERVER_PLAYER_DAMAGE_EVENT_TAG: {
            player_damage_event_t* player_damage_event = (player_damage_event_t*)event;
            entity_t pid = ENTITY_BY_ID(player_damage_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_sub(health, player_damage_event->damage);
            if (health_get(health) <= 0) {
                event_t* kill_event = (event_t*) ServerPlayerKillEvent_new(player_damage_event->entity_id, player_damage_event->source_entity_id);
                world_queue_event(world, kill_event);
                return -1; // If the player is dead, it is a kill event that we need to send, so we cancel this event.
            }
            break;
        }
        case SERVER_PLAYER_HEAL_EVENT_TAG: {
            player_heal_event_t* player_heal_event = (player_heal_event_t*)event;
            entity_t pid = ENTITY_BY_ID(player_heal_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_add(health, player_heal_event->gain);
            break;
        }
        case SERVER_PLAYER_HEALTH_EVENT_TAG: {
            player_health_event_t* player_health_event = (player_health_event_t*)event;
            entity_t pid = ENTITY_BY_ID(player_health_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_set(health, player_health_event->hp);
            health_set_max(health, player_health_event->max_hp);
            break;
        }
        case SERVER_PLAYER_KILL_EVENT_TAG: {
            player_kill_event_t* player_kill_event = (player_kill_event_t*)event;
            entity_t pid = ENTITY_BY_ID(player_kill_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) return -1; // If the player does not have health, we cannot apply the event, cancel it.
            health_set(health, 0);

            // Increment the deaths of the player.
            statistic_ct* stats = (statistic_ct*) world_get_component(world, &pid, COMPONENT_TAG_STATISTIC);
            if (stats == NULL) return -1; // If the player does not have stats, we cannot apply the event, cancel it.
            statistic_increment_deaths(stats, 1);

            // Increment the kills of the source player.
            if (player_kill_event->source_entity_id == 0) return 0; // If the source entity is 0 (NONE), then just return, no need to increment kills.
            pid = ENTITY_BY_ID(player_kill_event->source_entity_id);
            stats = (statistic_ct*) world_get_component(world, &pid, COMPONENT_TAG_STATISTIC);
            if (stats == NULL) return -1; // If the player does not have stats, we cannot apply the event, cancel it.
            statistic_increment_kills(stats, 1);

            // Trigger a scoreboard update event.
            char* entries[10] = {0};
            uint16_t deaths[10] = {0};
            uint16_t kills[10] = {0};
            int count = scoreboard_generate(world, entries, deaths, kills);
            event_t* scoreboard_event = (event_t*) ServerScoreboardUpdateEvent_new(count, entries, deaths, kills);
            world_queue_event(world, scoreboard_event);
            break;
        }
        default:
            break;
    }
    return 0;
}

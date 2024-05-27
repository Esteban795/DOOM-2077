#include "../../../include/system/client/apply_event.h"
#include "../../../include/event/all.h"
#include "../../../include/component/position.h"
#include "../../../include/component/display_name.h"
#include "../../../include/component/animation.h"
#include "../../../include/ecs/world.h"
#include "../../../include/component/health.h"
#include "../../../include/ui/linker.h"
#include "../../../include/player.h"
#include "../../../include/shared.h"

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
            player_move_event_t* client_player_move_event = (player_move_event_t*)event;
            pid = ENTITY_BY_ID(client_player_move_event->entity_id);
            animation_ct* animation = (animation_ct*) world_get_component(world, &pid, COMPONENT_TAG_ANIMATION);
            if (animation == NULL) return -1; // If the player does not have an animation, we cannot apply the event, cancel it.
            animation_set(animation, MOVING);
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

            animation_ct* animation = (animation_ct*) world_get_component(world, &pid, COMPONENT_TAG_ANIMATION);
            switch (client_player_kill_event->weapon_id) {
                case 3: {
                    animation_set(animation, EXTREME_DYING);
                    break;
                }
                default:
                    animation_set(animation, DYING);
                    break;
            }
            break;
        }
        case CLIENT_PLAYER_FIRE_EVENT_TAG: {
            player_fire_event_t* client_player_fire_event = (player_fire_event_t*)event;
            pid = ENTITY_BY_ID(client_player_fire_event->entity_id);
            position_ct* position = (position_ct*) world_get_component(world, &pid, COMPONENT_TAG_POSITION);
            if (position == NULL) return 0; // sound are not essential, if the player does not have a position, we can still apply the event.
            double x = position->x;
            double y = position->y;
            switch (client_player_fire_event->weapon_id) {
                case 0: {
                    add_sound_to_play(PUNCH_SOUND,x,y);
                    break;
                }
                case 1: {
                    add_sound_to_play(PISTOL_SOUND,x,y);
                    break;
                }
                case 2: {
                    add_sound_to_play(SHOTGUN_SOUND,x,y); // todo: change to shotgun sound
                    break;
                }
                case 3: {
                    add_sound_to_play(SHOTGUN_SOUND,x,y);
                    break;
                }
                default:
                    break;
            }
            animation_ct* animation = (animation_ct*) world_get_component(world, &pid, COMPONENT_TAG_ANIMATION);
            if (animation == NULL) return -1; // If the player does not have an animation, we cannot apply the event, cancel it.
            animation_set(animation, SHOOTING);
            break;
        }
        case CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG: {
            player_weapon_update_event_t* ev = (player_weapon_update_event_t*)event;
            pid = ENTITY_BY_ID(ev->entity_id); // Always us, the player
            weapon_ct* weapon = (weapon_ct*) world_get_component(world, &pid, COMPONENT_TAG_WEAPON);
            if (weapon == NULL) return -1; // If the player does not have a weapon, we cannot apply the event, cancel it.
            memcpy(weapon->ammunitions, ev->ammunitions, sizeof(int) * WEAPONS_NUMBER);
            memcpy(weapon->mags, ev->mags, sizeof(int) * WEAPONS_NUMBER);
            memcpy(weapon->cooldowns, ev->cooldowns, sizeof(int) * WEAPONS_NUMBER);
            weapon->active_weapon = 0;
            break;
        }
        case CLIENT_PLAYER_CHAT_EVENT_TAG: {
            player_chat_event_t* ev = (player_chat_event_t*)event;

            char fmt_msg[1024 + 128 + 2] = {0};
            int pind = player_find_by_id(SHARED_ENGINE->players, ev->entity_id);
            display_name_ct* name_ct;
            char* name;
            if (pind >= 0) {
                name_ct = (display_name_ct*) world_get_component(world, SHARED_ENGINE->players[pind], COMPONENT_TAG_DISPLAY_NAME);
                name = name_ct->name;
            } else {
                if (ev->entity_id == SHARED_ENGINE->p->entity->id) {
                    name = SHARED_ENGINE->player_name;
                } else {
                    name = "Unknown";
                }
            }
            snprintf(fmt_msg, 1024 + 128 + 1, "%s: %s", name, ev->message);
            UILINK_FEED_CHAT(SHARED_ENGINE->uimodules, fmt_msg);
            break;
        }
        case CLIENT_SERVER_CHAT_EVENT_TAG: {
            server_chat_event_t* ev = (server_chat_event_t*)event;

            char fmt_msg[1024 + 128 + 2] = {0};
            if (ev->is_title) {
                UILINK_FEED_KILL(SHARED_ENGINE->uimodules, ev->message);
            } else {
                if (ev->is_broadcast) {
                    snprintf(fmt_msg, 1024 + 128, "[SERVER] %s", ev->message);
                } else {
                    snprintf(fmt_msg, 1024 + 128, "Server to you: %s", ev->message);
                }
                UILINK_FEED_CHAT(SHARED_ENGINE->uimodules, fmt_msg);
            }
            break;
        }
        case CLIENT_SCOREBOARD_UPDATE_EVENT_TAG: {
            scoreboard_update_event_t* ev = (scoreboard_update_event_t*)event;
            char fmt_msg[256] = {0};
            for (int i = 0; i < 4; i++) {
                if (i < ev->entries_count) {
                    snprintf(fmt_msg, 256, "%i - %s [%d/%d]", i+1, ev->entries[i], ev->kills[i], ev->deaths[i]);
                } else {
                    snprintf(fmt_msg, 256, "%i - free slot", i+1);
                }
                UILINK_SET_SB_N(SHARED_ENGINE->uimodules, i, fmt_msg);
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

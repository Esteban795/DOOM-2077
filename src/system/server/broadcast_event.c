#include <stdio.h>
#include <stdlib.h>

#include "../../../include/server/state.h"
#include "../../../include/ecs/world.h"
#include "../../../include/ecs/entity.h"
#include "../../../include/system/server/broadcast_event.h"
#include "../../../include/component/display_name.h"
#include "../../../include/component/position.h"
#include "../../../include/component/health.h"
#include "../../../include/net/packet/server.h"
#include "../../../include/net/tracked_connection.h"
#include "../../../include/event/all.h"

const system_t BROADCAST_EVENT_SYSTEM = {
    .fn = broadcast_event,
};

int broadcast_event(world_t* world, event_t* event) {
    uint8_t buf[2048];
    int len;
    entity_t pid;
    UDPsocket sock = SERVER_STATE->sock;
    tracked_connection_t* conns = SERVER_STATE->conns;
    UDPpacket* outgoing = SERVER_STATE->outgoing;

    switch (event->tag) {
        case SERVER_PLAYER_CHAT_EVENT_TAG: {
            player_chat_event_t* player_chat_event = (player_chat_event_t*)event;
            pid = ENTITY_BY_ID(player_chat_event->entity_id);
            display_name_ct* display_name = (display_name_ct*) world_get_component(world, &pid, COMPONENT_TAG_DISPLAY_NAME);
            if (display_name == NULL) break;
            printf("%s (%ld)> %s\n", display_name_get(display_name), player_chat_event->entity_id, player_chat_event->message);

            len = server_player_chat(buf, player_chat_event->entity_id, player_chat_event->message);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_SERVER_CHAT_EVENT_TAG: {
            server_chat_event_t* server_chat_event = (server_chat_event_t*)event;
            printf("server > %s\n", server_chat_event->message);

            len = server_server_chat(buf, server_chat_event->message, server_chat_event->is_broadcast, server_chat_event->is_title);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_JOIN_EVENT_TAG: {
            server_player_join_event_t* server_player_join_event = (server_player_join_event_t*)event;
            printf("%s joined the server.\n", server_player_join_event->name);

            // Broadcast the other players!
            char msg[256];
            strncat(msg, server_player_join_event->name, 128);
            strcat(msg, " joined the server.");
            len = server_join(buf, server_player_join_event->entity_id, server_player_join_event->name);
            len += server_server_chat(buf + len, msg, true, false);
            pid = ENTITY_BY_ID(server_player_join_event->entity_id);
            position_ct* pos = (position_ct*) world_get_component(world, &pid, COMPONENT_TAG_POSITION);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (pos == NULL || health == NULL) {
                printf("SERVER_PLAYER_JOIN_EVENT_TAG > Player does not have a position or health component.\n");
            } else {
                len += server_player_move(buf + len, server_player_join_event->entity_id, pos->x, pos->y, pos->z, pos->angle);
                len += server_player_health(buf + len, server_player_join_event->entity_id, health->health, health->max_health);
                bool* doors_states = (bool*) malloc(sizeof(bool) * SERVER_STATE->door_count);
                door_states_generate(world, SERVER_STATE->doors, doors_states, SERVER_STATE->door_count);
                bool* lifts_states = (bool*) malloc(sizeof(bool) * SERVER_STATE->lift_count);
                lift_states_generate(world, SERVER_STATE->lifts, lifts_states, SERVER_STATE->lift_count);
                len += server_door_states(buf + len, SERVER_STATE->door_count, doors_states);
                len += server_lift_states(buf + len, SERVER_STATE->lift_count, lifts_states);
                free(doors_states);
                free(lifts_states);
                char* entries[10] = {0};
                uint16_t deaths[10] = {0};
                uint16_t kills[10] = {0};
                int entity_count = scoreboard_generate(world, entries, deaths, kills);
                len += server_scoreboard_update(buf + len, entity_count, entries, deaths, kills);
            }
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);

            // Send the latest info on the other players (and you at the same time).
            outgoing->len = 0;
            for (int i = 0; i < SERVER_STATE->conn_count; i++) {
                if (conns[i].player_id == server_player_join_event->entity_id) {
                    outgoing->address = conns[i].ip;
                    continue;
                }
                pid = ENTITY_BY_ID(conns[i].player_id);
                display_name_ct* display_name = (display_name_ct*) world_get_component(world, &pid, COMPONENT_TAG_DISPLAY_NAME);
                position_ct* pos = (position_ct*) world_get_component(world, &pid, COMPONENT_TAG_POSITION);
                health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
                if (display_name == NULL) continue;
                outgoing->len += server_join(outgoing->data + outgoing->len, conns[i].player_id, display_name_get(display_name));
                if (pos == NULL) continue;
                outgoing->len += server_player_move(outgoing->data + outgoing->len, conns[i].player_id, pos->x, pos->y, pos->z, pos->angle);
                if (health == NULL) continue;
                outgoing->len += server_player_health(outgoing->data + outgoing->len, conns[i].player_id, health->health, health->max_health);
            }
            SDLNet_UDP_Send(sock, -1, outgoing);
            break;
        }
        case SERVER_PLAYER_QUIT_EVENT_TAG: {
            server_player_quit_event_t* server_player_quit_event = (server_player_quit_event_t*)event;
            printf("%s left the server.\n", server_player_quit_event->name);

            char msg[256];
            strncat(msg, server_player_quit_event->name, 128);
            strcat(msg, " left the server.");
            len = server_quit(buf, server_player_quit_event->entity_id);
            len += server_server_chat(buf + len, msg, true, false);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_MOVE_EVENT_TAG: {
            player_move_event_t* player_move_event = (player_move_event_t*)event;
            // DEBUG:
            // pid = ENTITY_BY_ID(player_move_event->entity_id);
            // display_name_ct* display_name = (display_name_ct*) world_get_component(world, &pid, COMPONENT_TAG_DISPLAY_NAME);
            // if (display_name == NULL) break;
            // printf("%s moved to (%f, %f, %f) at angle %f.\n", display_name_get(display_name), player_move_event->x, player_move_event->y, player_move_event->z, player_move_event->angle);
            len = server_player_move(buf, player_move_event->entity_id, player_move_event->x, player_move_event->y, player_move_event->z, player_move_event->angle);
            broadcast_except(&sock, conns, SERVER_STATE->conn_count, player_move_event->entity_id, buf, len);
            break;
        }
        case SERVER_PLAYER_DAMAGE_EVENT_TAG: {
            player_damage_event_t* player_damage_event = (player_damage_event_t*) event;
            pid = ENTITY_BY_ID(player_damage_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) break;
            len = server_player_damage(buf, player_damage_event->entity_id, player_damage_event->source_entity_id, player_damage_event->damage);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_HEAL_EVENT_TAG: {
            player_heal_event_t* player_heal_event = (player_heal_event_t*) event;
            pid = ENTITY_BY_ID(player_heal_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) break;
            len = server_player_heal(buf, player_heal_event->entity_id, player_heal_event->gain);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_HEALTH_EVENT_TAG: {
            player_health_event_t* player_health_event = (player_health_event_t*) event;
            pid = ENTITY_BY_ID(player_health_event->entity_id);
            health_ct* health = (health_ct*) world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
            if (health == NULL) break;
            len = server_player_health(buf, player_health_event->entity_id, health->health, health->max_health);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_KILL_EVENT_TAG: {
            player_kill_event_t* player_kill_event = (player_kill_event_t*) event;
            entity_t killed = ENTITY_BY_ID(player_kill_event->entity_id);
            entity_t killer = ENTITY_BY_ID(player_kill_event->source_entity_id);
            display_name_ct* killed_name = (display_name_ct*) world_get_component(world, &killed, COMPONENT_TAG_DISPLAY_NAME);
            display_name_ct* killer_name = (display_name_ct*) world_get_component(world, &killer, COMPONENT_TAG_DISPLAY_NAME);
            if (killed_name == NULL) break;
            char death_msg[512] = {0};
            if (killer_name == NULL) {
                strncat(death_msg, display_name_get(killed_name), 128);
                strcat(death_msg, " was killed.");
            } else {
                strncat(death_msg, display_name_get(killed_name), 128);
                strcat(death_msg, " succumbed to the attacks of ");
                strncat(death_msg, display_name_get(killer_name), 128);
            }
            printf("%s\n", death_msg);
            len = server_player_kill(buf, player_kill_event->entity_id, player_kill_event->source_entity_id);
            len += server_server_chat(buf + len, death_msg, true, true);
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_DOOR_OPEN_EVENT_TAG: {
            door_open_event_t* ev = (door_open_event_t*) event;
            if (ev->is_lift) {
                printf("Lift %ld is ascending.\n", ev->door_id);
                len = server_lift_ascend(buf, ev->door_id);
            } else {
                printf("Door %ld is opening.\n", ev->door_id);
                len = server_door_open(buf, ev->door_id);
            }
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_DOOR_CLOSE_EVENT_TAG: {
            door_close_event_t* ev = (door_close_event_t*) event;
            if (ev->is_lift) {
                printf("Lift %ld is descending.\n", ev->door_id);
                len = server_lift_descend(buf, ev->door_id);
            } else {
                printf("Door %ld is closing.\n", ev->door_id);
                len = server_door_close(buf, ev->door_id);
            }
            broadcast(&sock, conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        default:
            printf("Unimplemented event tag: %d\n", event->tag);
            break;
    }
    
    return 0;
}
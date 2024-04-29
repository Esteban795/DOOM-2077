#include <stdio.h>
#include <stdlib.h>

#include "../../../include/server/state.h"
#include "../../../include/ecs/world.h"
#include "../../../include/ecs/entity.h"
#include "../../../include/event/event.h"
#include "../../../include/system/server/broadcast_event.h"
#include "../../../include/component/display_name.h"
#include "../../../include/net/packet/server.h"
#include "../../../include/net/tracked_connection.h"

#include "../../../include/event/server_chat.h"
#include "../../../include/event/server_join.h"
#include "../../../include/event/server_quit.h"
#include "../../../include/event/player_chat.h"
#include "../../../include/event/player_move.h"

const system_t BROADCAST_EVENT_SYSTEM = {
    .fn = broadcast_event,
};

int broadcast_event(world_t* world, event_t* event) {
    uint8_t buf[2048];
    int len;
    entity_t pid;

    switch (event->tag) {
        case SERVER_PLAYER_CHAT_EVENT_TAG: {
            player_chat_event_t* player_chat_event = (player_chat_event_t*)event;
            pid = ENTITY_BY_ID(player_chat_event->entity_id);
            display_name_ct* display_name = (display_name_ct*) world_get_component(world, &pid, COMPONENT_TAG_DISPLAY_NAME);
            printf("%s (%ld)> %s\n", display_name_get(display_name), player_chat_event->entity_id, player_chat_event->message);

            len = server_player_chat(buf, player_chat_event->entity_id, player_chat_event->message);
            broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_SERVER_CHAT_EVENT_TAG: {
            server_chat_event_t* server_chat_event = (server_chat_event_t*)event;
            printf("server > %s\n", server_chat_event->message);

            len = server_server_chat(buf, server_chat_event->message, server_chat_event->is_broadcast, server_chat_event->is_title);
            broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_JOIN_EVENT_TAG: {
            server_player_join_event_t* server_player_join_event = (server_player_join_event_t*)event;
            printf("%s joined the server.\n", server_player_join_event->name);

            char msg[256];
            strncat(msg, server_player_join_event->name, 128);
            strcat(msg, " joined the server.");
            //len = server_join(buf, server_player_join_event->name);
            len = 0;
            len += server_server_chat(buf + len, msg, true, false);
            broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, buf, len);
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
            broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, buf, len);
            break;
        }
        case SERVER_PLAYER_MOVE_EVENT_TAG: {
            player_move_event_t* player_move_event = (player_move_event_t*)event;
            pid = ENTITY_BY_ID(player_move_event->entity_id);
            display_name_ct* display_name = (display_name_ct*) world_get_component(world, &pid, COMPONENT_TAG_DISPLAY_NAME);
            printf("%s moved to (%f, %f, %f) at angle %f.\n", display_name_get(display_name), player_move_event->x, player_move_event->y, player_move_event->z, player_move_event->angle);
            len = server_player_move(buf, player_move_event->entity_id, player_move_event->x, player_move_event->y, player_move_event->z, player_move_event->angle);
            broadcast_except(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, player_move_event->entity_id, buf, len);
            break;
        }
        default:
            printf("Unimplemented event tag: %d\n", event->tag);
            break;
    }
    
    return 0;
}
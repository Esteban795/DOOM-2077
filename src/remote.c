#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/component/display_name.h"
#include "../include/component/health.h"
#include "../include/component/position.h"
#include "../include/component/weapon.h"
#include "../include/engine.h"
#include "../include/net/packet/client.h"
#include "../include/net/packet/server.h"
#include "../include/net/util.h"
#include "../include/player.h"
#include "../include/remote.h"
#include "../include/settings.h"
#include "../include/structs.h"

#include "../include/event/all.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef NETWORK_TICK_MS
// 30 tick per second
#define NETWORK_TICK_MS 33
#endif

#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
#define INSTANT_DIFF_MS(a, b)                                                  \
  ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)
#define INSTANT_ADD_MS(a, ms)                                                  \
  a.tv_sec += ms / 1000;                                                       \
  a.tv_nsec += (ms % 1000) * 1000000;                                          \
  if (a.tv_nsec >= 1000000000) {                                               \
    a.tv_sec++;                                                                \
    a.tv_nsec -= 1000000000;                                                   \
  }

typedef struct timespec Instant;

int remote_init(remote_server_t *server, char *addr, int port, char* player_name) {
  if (strncat(addr, "", 1) == 0) {
    fprintf(stderr,
            "WARN: No SERVER_ADDR found! No connection will be initiated!\n");
    server->addr.port = 0;
    server->connected = -1;
    return 0;
  }
  server->connected = 0;

  printf("Connecting to %s:%d...\n", addr, port);

  // Resolve the host
  if (SDLNet_ResolveHost(&server->addr, addr, port) < 0) {
    fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    return -1;
  }

  // Initialize the UDP socket
  server->socket = SDLNet_UDP_Open(0);
  if (!server->socket) {
    fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    return -1;
  }

  // Send a JOIN packet to the server
  server->packet = SDLNet_AllocPacket(2048);
  if (!server->packet) {
    fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    return -1;
  }
  int len = client_join(server->packet->data, player_name);
  server->packet->address.host = server->addr.host;
  server->packet->address.port = server->addr.port;
  server->packet->len = len;
  if (SDLNet_UDP_Send(server->socket, -1, server->packet) == 0) {
    fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
    return -1;
  }

  // Register the next tick
  INSTANT_NOW((&server->next_tick));
  INSTANT_ADD_MS(server->next_tick, NETWORK_TICK_MS);

  return 0;
}

void remote_disconnect(remote_server_t *r) {
  if (r->connected < 2) {
    return;
  }

  // Send a QUIT packet to the server
  int len = client_quit(r->packet->data);
  r->packet->len = len;
  r->packet->address.host = r->addr.host;
  r->packet->address.port = r->addr.port;
  if (SDLNet_UDP_Send(r->socket, -1, r->packet) == 0) {
    fprintf(stderr, "SDLNet_UDP_Send: %s\n", SDLNet_GetError());
  } else {
    printf("Disconnected from server.\n");
  }

  r->connected = -1;
}

void remote_destroy(remote_server_t *r) {
  if (r->packet != NULL) SDLNet_FreePacket(r->packet);
  if (r->socket != NULL) SDLNet_UDP_Close(r->socket);
  r->socket = NULL;
  r->packet = NULL;
  free(r);
}

int remote_update(engine *e, remote_server_t *r) {
  if (r->connected < 0) {
    return 0;
  }

  // Wait for the next tick
  Instant now;
  INSTANT_NOW(&now);
  int elapsed = INSTANT_DIFF_MS(now, r->next_tick);
  if (elapsed < 0) {
    return 0;
  }
  // Register the next tick
  INSTANT_ADD_MS(r->next_tick, NETWORK_TICK_MS);

  // Receive a packet from the server
  if (SDLNet_UDP_Recv(r->socket, r->packet) > 0) {
    uint8_t sdata[2048] = {0};
    memcpy(sdata, r->packet->data, r->packet->len);
    int offset = 0;

    // Process one message at a time
    char cmd[5] = {0};
    while (offset + 5 < r->packet->len) {
      memcpy(cmd, (char *)sdata + offset, 4);
      uint16_t len = read_uint16be(sdata + offset + 4);

      // DEBUG: printf("Got command: %s\n", cmd);

      if (strncmp(cmd, SERVER_COMMAND_KICK, 4) == 0) {
        char reason_[1024] = {0};
        char *reason = (char *)reason_;
        server_kick_from(sdata + offset, (char **)&reason);
        printf("Kicked by server: %s\n", reason);
        r->connected = -1;
        return 1;
      } else if (strncmp(cmd, SERVER_COMMAND_ACPT, 4) == 0) {
        uint64_t player_id;
        server_acpt_from(sdata + offset, &player_id);
        if (r->connected == 0) {
          printf("Accepted by server as %lu\n", player_id);
          r->connected = 1;
          r->player_id = player_id;
        } else {
          printf("Ignoring server ACPT for %lu.\n", player_id);
        }
      } else if (strncmp(cmd, SERVER_COMMAND_LMAP, 4) == 0) {
        char map_name_[128] = {0};
        char *map_name = (char *)map_name_;
        server_load_map_from(sdata + offset, (char **)&map_name);
        if (wad_is_map_loaded(e->wData)) {
          free_map(e->wData);
        }
        read_map(e, map_name);
        printf("Server loaded map: %s\n", map_name);
      } else if (strncmp(cmd, SERVER_COMMAND_PONG, 4) == 0) {
        uint64_t data;
        server_pong_from(sdata + offset, &data);
        printf("Pong from server: %lu\n", data);
      } else if (strncmp(cmd, SERVER_COMMAND_QUIT, 4) == 0) {
        uint64_t player_id;
        server_quit_from(sdata + offset, &player_id);
        event_t *event = (event_t *)ClientPlayerQuitEvent_new(player_id);
        world_queue_event(e->world, event);
        printf("Player %lu quit\n", player_id);

        // Removing the player from the playerlist.
        // TODO: Remove the player from the ECS.
        int ind = player_find_by_id(e->players, player_id);
        if (ind >= 0) {
          e->players[ind] = NULL;
        }
      } else if (strncmp(cmd, SERVER_COMMAND_JOIN, 4) == 0) {
        uint64_t player_id;
        char player_name_[128] = {0};
        char *player_name = (char *)player_name_;
        server_join_from(sdata + offset, &player_id, (char **)&player_name);
        event_t *event =
            (event_t *)ClientPlayerJoinEvent_new(player_name, player_id);
        world_queue_event(e->world, event);
        printf("Player %s joined as %lu\n", player_name, player_id);

        // If the player is not us, then insert the player in the ECCS
        if (player_id != e->p->entity->id) {
          int ammo[WEAPONS_NUMBER];
          int mags[WEAPONS_NUMBER];
          ammo[0] = -2;
          mags[0] = 0;
          for (int i = 1; i < WEAPONS_NUMBER; i++) {
            ammo[i] = 10;
            mags[i] = 10;
          }

          // Add player to the ECS world
          double coords[3] = {0.0, 0.0, PLAYER_HEIGHT};
          component_t **comps = malloc(sizeof(component_t *) * 4);
          comps[0] = position_create(coords, 180.0);
          comps[1] = health_create(100.0, 100.0);
          comps[2] = weapon_create(ammo,mags);
          comps[3] = display_name_create(player_name);
          entity_t *entity = world_insert_entity(e->world, player_id, comps, 4);
          // If entity == NULL, an entity already exists with this id, replacing
          // it.
          if (entity == NULL) {
            world_remove_entity_by_id(e->world, player_id);
            entity = world_insert_entity(e->world, player_id, comps, 4);
          }
          assert(entity != NULL);

          // Insert the player entity in the recorded playerlist.
          for (int i = 0; i < PLAYER_MAXIMUM; i++) {
            if (e->players[i] == NULL) {
              e->players[i] = entity;
              assert(e->players[i] != NULL);
              break;
            }
          }
          free(comps);
        }
      } else if (strncmp(cmd, SERVER_COMMAND_MOVE, 4) == 0) {
        uint64_t player_id;
        double x, y, z, angle;
        server_player_move_from(sdata + offset, &player_id, &x, &y, &z, &angle);
        event_t *event =
            (event_t *)ClientPlayerMoveEvent_new(player_id, x, y, z, angle);
        world_queue_event(e->world, event);

        // Moving the player
        position_ct *pos = NULL;
        if (player_id == e->p->entity->id) {
          // Rollback / TP my position
          pos = player_get_position(e->p);
        } else {
          // Move the selected player
          int ind = player_find_by_id(e->players, player_id);
          if (ind >= 0) {
            pos = (position_ct *)world_get_component(e->world, e->players[ind],
                                                     COMPONENT_TAG_POSITION);
          }
        }
        if (pos != NULL) {
          pos->last_x = pos->x;
          pos->last_y = pos->y;
          pos->last_z = pos->z;
          pos->last_angle = pos->angle;
          pos->x = x;
          pos->y = y;
          pos->z = z;
          pos->angle = angle;
          pos->was_updated = true;
        }
      } else if (strncmp(cmd, SERVER_COMMAND_CHAT, 4) == 0) {
        uint64_t player_id;
        char message_[1024] = {0};
        char *message = (char *)message_;
        server_player_chat_from(sdata + offset, &player_id, (char **)&message);
        event_t *event =
            (event_t *)ClientPlayerChatEvent_new(player_id, message);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_TELL, 4) == 0) {
        bool is_title, is_broadcast;
        char message_[1024] = {0};
        char *message = (char *)message_;
        server_server_chat_from(sdata + offset, (char **)&message,
                                &is_broadcast, &is_title);
        event_t *event = (event_t *)ClientServerChatEvent_new(
            message, is_broadcast, is_title);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_SCOR, 4) == 0) {
        char **names = NULL;
        uint16_t *deaths = NULL;
        uint16_t *kills = NULL;
        uint16_t entry_count;
        server_scoreboard_update_from(sdata+offset, &entry_count, &names, &deaths, &kills);
        event_t *event = (event_t *)ClientScoreboardUpdateEvent_new(
            entry_count, names, deaths, kills);
        world_queue_event(e->world, event);
        for (int i = 0; i < entry_count; i++) {
          free(names[i]);
        }
        free(deaths);
        free(kills);
        free(names);
      } else if (strncmp(cmd, SERVER_COMMAND_HLTH, 4) == 0) {
        uint64_t player_id;
        float health, max_health;
        server_player_health_from(sdata + offset, &player_id, &health,
                                  &max_health);
        event_t *event = (event_t *)ClientPlayerHealthEvent_new(
            player_id, health, max_health);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_HEAL, 4) == 0) {
        uint64_t player_id;
        float health;
        server_player_heal_from(sdata + offset, &player_id, &health);
        event_t *event =
            (event_t *)ClientPlayerHealEvent_new(player_id, health);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_KILL, 4) == 0) {
        uint64_t player_id, src_player_id;
        server_player_kill_from(sdata + offset, &player_id, &src_player_id);
        event_t *event =
            (event_t *)ClientPlayerKillEvent_new(player_id, src_player_id);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_DAMG, 4) == 0) {
        uint64_t player_id, src_player_id;
        float damage;
        server_player_damage_from(sdata + offset, &player_id, &src_player_id,
                                  &damage);
        event_t *event = (event_t *)ClientPlayerDamageEvent_new(
            player_id, src_player_id, damage);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_DOST, 4) == 0) {
        uint16_t doors_count;
        bool *doors_states;
        server_door_states_from(sdata + offset, &doors_count, &doors_states);
        if (doors_count != e->num_doors) {
          fprintf(stderr, "Mismatched door count: %d != %d\n", doors_count,
                  e->num_doors);
        } else {
          for (int i = 0; i < doors_count; i++) {
            if (e->doors[i]->state != doors_states[i]) {
              door_trigger_switch(e->doors[i]);
            }
          }
        }
        free(doors_states);
      } else if (strncmp(cmd, SERVER_COMMAND_L_ST, 4) == 0) {
        uint16_t lifts_count;
        bool *lifts_states;
        server_door_states_from(sdata + offset, &lifts_count, &lifts_states);
        if (lifts_count != e->len_lifts) {
          fprintf(stderr, "Mismatched lift count: %d != %d\n", lifts_count,
                  e->len_lifts);
        } else {
          for (int i = 0; i < lifts_count; i++) {
            if (e->lifts[i]->state != lifts_states[i]) {
              lift_trigger_switch(e->lifts[i]);
            }
          }
        }
        free(lifts_states);
      } else if (strncmp(cmd, SERVER_COMMAND_OPEN, 4) == 0) {
        uint64_t door_id;
        server_door_open_from(sdata + offset, &door_id);
        if ((int) door_id < e->num_doors && e->doors[door_id]->state == false) {
          door_trigger_switch(e->doors[door_id]);
        }
        event_t *event = (event_t *)ClientDoorOpenEvent_new(door_id, false);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_CLOS, 4) == 0) {
        uint64_t door_id;
        server_door_close_from(sdata + offset, &door_id);
        if ((int) door_id < e->num_doors && e->doors[door_id]->state == true) {
          door_trigger_switch(e->doors[door_id]);
        }
        event_t *event = (event_t *)ClientDoorCloseEvent_new(door_id, false);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_LASC, 4) == 0) {
        uint64_t lift_id;
        server_lift_ascend_from(sdata + offset, &lift_id);
        if ((int) lift_id < e->len_lifts && e->lifts[lift_id]->state == false) {
          lift_trigger_switch(e->lifts[lift_id]);
        }
        event_t *event = (event_t *)ClientDoorOpenEvent_new(lift_id, true);
        world_queue_event(e->world, event);
      } else if (strncmp(cmd, SERVER_COMMAND_LDSC, 4) == 0) {
        uint64_t lift_id;
        server_lift_descend_from(sdata + offset, &lift_id);
        if ((int) lift_id < e->len_lifts && e->lifts[lift_id]->state == true) {
          lift_trigger_switch(e->lifts[lift_id]);
        }
        event_t *event = (event_t *)ClientDoorCloseEvent_new(lift_id, true);
        world_queue_event(e->world, event);
      } else {
        printf("Unknown command: %s\n", cmd);
      }

      offset += (6 + len + 1);
    }
  }

  if (r->connected < 2) {
    // No need to send any more packets if the connection is not fully
    // established!
    return 0;
  }

  // Send the player data for the current frame
  static double x, y, z, angle = 0.0;
  position_ct *pos = player_get_position(e->p);
  if (x == pos->x && y == pos->y && z == pos->z && angle == pos->angle) {
    return 0;
  }
  int len =
      client_move(r->packet->data, position_get_x(pos), position_get_y(pos),
                  position_get_z(pos), position_get_angle(pos));
  r->packet->len = len;
  r->packet->address.host = r->addr.host;
  r->packet->address.port = r->addr.port;
  if (SDLNet_UDP_Send(r->socket, -1, r->packet) == 0) {
    fprintf(stderr, "SDLNet_UDP_Send: move command: %s\n", SDLNet_GetError());
    return -1;
  }

  x = pos->x;
  y = pos->y;
  z = pos->z;
  angle = pos->angle;

  return 0;
}

void __client_door_trigger(engine *e, uint64_t door_id, bool is_lift) {
  if (e->remote->connected < 2) {
    return;
  }

  int len = 0;
  if (is_lift) {
    if (e->lifts[door_id]->state) {
      len = client_lift_descend(e->remote->packet->data, door_id);
    } else {
      len = client_lift_ascend(e->remote->packet->data, door_id);
    }
  } else {
    if (e->doors[door_id]->state) {
      len = client_door_close(e->remote->packet->data, door_id);
    } else {
      len = client_door_open(e->remote->packet->data, door_id);
    }
  }
  e->remote->packet->len = len;
  e->remote->packet->address.host = e->remote->addr.host;
  e->remote->packet->address.port = e->remote->addr.port;
  if (SDLNet_UDP_Send(e->remote->socket, -1, e->remote->packet) == 0) {
    fprintf(stderr, "SDLNet_UDP_Send: trigger command: %s\n",
            SDLNet_GetError());
  }
}

void client_door_trigger(engine *e, uint64_t door_id) {
  if (door_trigger_switch(e->doors[door_id])) {
    __client_door_trigger(e, door_id, false);
  }
}

void client_lift_trigger(engine *e, uint64_t lift_id) {
  if (lift_trigger_switch(e->lifts[lift_id])) {
    __client_door_trigger(e, lift_id, true);
  }
}

void remote_send_chat(engine* e, char *message) {
  remote_server_t *r = e->remote;
  if (r->connected < 2) {
    event_t* ev = (event_t*) ClientPlayerChatEvent_new(0, message);
    world_queue_event(e->world, ev);
    return;
  }

  int len = client_chat(r->packet->data, message);
  r->packet->len = len;
  r->packet->address.host = r->addr.host;
  r->packet->address.port = r->addr.port;
  if (SDLNet_UDP_Send(r->socket, -1, r->packet) == 0) {
    fprintf(stderr, "SDLNet_UDP_Send: chat command: %s\n", SDLNet_GetError());
  }
}
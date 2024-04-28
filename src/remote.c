#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../include/structs.h"
#include "../include/remote.h"
#include "../include/net/packet/client.h"
#include "../include/net/packet/server.h"
#include "../include/net/util.h"
#include "../include/player.h"
#include "../include/component/position.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef PLAYER_USERNAME
#define PLAYER_USERNAME "player"
#endif

#ifndef NETWORK_TICK_MS
// 30 tick per second
#define NETWORK_TICK_MS 33
#endif

#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
#define INSTANT_DIFF_MS(a, b) ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)
#define INSTANT_ADD_MS(a, ms) a.tv_sec += ms / 1000; a.tv_nsec += (ms % 1000) * 1000000; if (a.tv_nsec >= 1000000000) { a.tv_sec++; a.tv_nsec -= 1000000000; }

typedef struct timespec Instant;

int remote_init(remote_server_t* server, char* addr, int port) {
    if (strncat(addr, "", 1) == 0) {
        fprintf(stderr, "WARN: No SERVER_ADDR found! No connection will be initiated!\n");
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
    int len = client_join(server->packet->data, PLAYER_USERNAME);
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

void remote_destroy(remote_server_t* r) {
    SDLNet_FreePacket(r->packet);
    SDLNet_UDP_Close(r->socket);
    r->socket = NULL;
    r->packet = NULL;
    free(r);
}


int remote_update(engine* e, remote_server_t* r) {
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
        char sdata[2048] = {0};
        memcpy(sdata, r->packet->data, r->packet->len);
        int offset = 0;

        // Process one message at a time
        char cmd[5] = {0};
        while (offset + 5 < r->packet->len) {
            memcpy(cmd, sdata + offset, 4);
            uint16_t len = read_uint16be((uint8_t*) (sdata + offset + 4));
            uint8_t* payload = (uint8_t*) (sdata + offset + 6);

            if (strncmp(cmd, SERVER_COMMAND_KICK, 4) == 0) {
                printf("Kicked by server: %s\n", payload);
                return 1;
            } else if (strncmp(cmd, SERVER_COMMAND_ACPT, 4) == 0) {
                uint64_t player_id = read_uint64be(payload);
                if (r->connected == 0) {
                    printf("Accepted by server as %lu\n", player_id);
                    r->connected = 1;
                    r->player_id = player_id;
                } else {
                    printf("Ignoring server ACPT for %lu.\n", player_id);
                }
            } else if (strncmp(cmd, SERVER_COMMAND_PONG, 4) == 0) {
                uint64_t data = read_uint64be(payload);
                printf("Pong from server: %lu\n", data);
            } else if (strncmp(cmd, SERVER_COMMAND_QUIT, 4) == 0) {
                uint64_t player_id = read_uint64be(payload);
                printf("Player %lu quit\n", player_id);
            } else if (strncmp(cmd, SERVER_COMMAND_JOIN, 4) == 0) {
                uint64_t player_id = read_uint64be(payload);
                char* player_name = read_cstring(payload + 8);
                printf("Player %s joined as %lu\n", player_name, player_id);
            } else {
                printf("Unknown command: %s\n", cmd);
            }

            offset += (6 + len);
        }
    }

    if (r->connected < 2) {
        // No need to send any more packets if the connection is not fully established!
        return 0;
    }

    // Send the player data for the current frame
    position_ct* pos = player_get_position(e->p);
    int len = client_move(r->packet->data, position_get_x(pos), position_get_y(pos), position_get_z(pos), position_get_angle(pos));
    r->packet->len = len;
    r->packet->address.host = r->addr.host;
    r->packet->address.port = r->addr.port;
    if (SDLNet_UDP_Send(r->socket, -1, r->packet) == 0) {
        fprintf(stderr, "SDLNet_UDP_Send: move command: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}
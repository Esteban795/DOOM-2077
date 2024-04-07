#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/structs.h"
#include "../include/remote.h"
#include "../include/net/packet/client.h"
#include "../include/net/packet/server.h"
#include "../include/net/util.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef PLAYER_USERNAME
#define PLAYER_USERNAME "player"
#endif

int remote_init(remote_server_t* server, char* addr, int port) {
    if (strncat(addr, "", 1) == 0) {
        fprintf(stderr, "WARN: No SERVER_ADDR found! No connection will be initiated!\n");
        server->addr.port = 0;
        return 0;
    }

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
    if (r->addr.port == 0) {
        // If dest port is 0, we're not connected to a server
        return 0;
    }

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
                printf("Accepted by server as %lu\n", player_id);
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

    // Send the player data for the current frame
    int len = client_move(r->packet->data, e->p->pos.x, e->p->pos.y, e->p->angle);
    r->packet->len = len;
    r->packet->address.host = r->addr.host;
    r->packet->address.port = r->addr.port;
    if (SDLNet_UDP_Send(r->socket, -1, r->packet) == 0) {
        fprintf(stderr, "SDLNet_UDP_Send: move command: %s\n", SDLNet_GetError());
        return -1;
    }

    return 0;
}
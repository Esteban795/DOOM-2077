#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#include "../include/server/state.h"
#include "../include/net/util.h"
#include "../include/net/tracked_connection.h"
#include "../include/net/packet/server.h"
#include "../include/net/packet/client.h"
#include "../include/ecs/world.h"
#include "../include/event/server_chat.h"
#include "../include/event/server_join.h"
#include "../include/event/server_quit.h"
#include "../include/event/player_chat.h"
#include "../include/event/player_move.h"
#include "../include/system/server/broadcast_event.h"

// Interrupt signal handler
//
// Handle in particular SIGINT (Ctrl+C), SIGQUIT, and SIGTERM, for a graceful shutdown.
void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        SERVER_RUNNING = -1;
        break;
    case SIGQUIT:
    case SIGTERM:
        SERVER_RUNNING = -2;
        break;
    default:
        printf("Caught signal %d.\n", sig);
        break;
    }
}

int run_server(uint16_t port)
{
    printf("Initializing SDL_net...\n");
    SDLNet_Init();
    char addrstr[24];

    // Create socket
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return -1;
    }
    UDPsocket server = SDLNet_UDP_Open(port);
    if (server == NULL)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return -1;
    }

    addrtocstr(&ip, addrstr);
    printf("Listening on %s...\n", addrstr);

    // Initialize server state
    SERVER_STATE = (server_state_t *)malloc(sizeof(server_state_t));
    assert(SERVER_STATE != NULL);
    SERVER_STATE->sock = server;
    SERVER_STATE->conn_count = 0;
    SERVER_RUNNING = 1;
    INSTANT_NOW(&SERVER_STATE->last_tick);
    world_init(&SERVER_STATE->world);

    // Register the systems
    world_register_system(&SERVER_STATE->world, BROADCAST_EVENT_SYSTEM.fn);

    // Listen for incoming packets
    SERVER_STATE->incoming = SDLNet_AllocPacket(1024);
    SERVER_STATE->outgoing = SDLNet_AllocPacket(1024);
    assert(SERVER_STATE->incoming != NULL);
    assert(SERVER_STATE->outgoing != NULL);

    Instant cur_time;
    while (SERVER_RUNNING > 0)
    {
        INSTANT_NOW(&cur_time);
        int elapsed = INSTANT_DIFF_MS(cur_time, SERVER_STATE->last_tick);
        // While there's time left in this tick, process incoming packets.
        while (elapsed < SERVER_TICK_MS)
        {
            // Process an incoming packet
            int ready = SDLNet_UDP_Recv(server, SERVER_STATE->incoming);
            if (ready == 1)
            {
                addrtocstr(&SERVER_STATE->incoming->address, addrstr);
                char sdata[2048] = {0};
                memcpy(sdata, (char *)SERVER_STATE->incoming->data, SERVER_STATE->incoming->len);
                printf("incoming packet from %s > %s\n", addrstr, sdata);

                int cursor = 0;
                char cmd[5] = {0};
                int16_t payload_len = 0;
                uint8_t *payload = NULL;
                // For each message in the packet
                while (cursor + 6 <= SERVER_STATE->incoming->len)
                {
                    memcpy(cmd, sdata + cursor, 4);
                    payload_len = read_uint16be((uint8_t *)(sdata + cursor + 4));
                    payload = (uint8_t *)(sdata + cursor + 6);
                    if (cursor + 6 + payload_len + 1 > SERVER_STATE->incoming->len)
                    {
                        printf("Invalid packet: message length exceeds packet length.\n");
                        break;
                    }

                    int conn_i = find_conn_by_ip(SERVER_STATE->conns, SERVER_STATE->conn_count, &SERVER_STATE->incoming->address);

                    if (strncmp(cmd, CLIENT_COMMAND_PING, 4) == 0)
                    {
                        uint64_t data;
                        client_ping_from(payload, &data);
                        int len = server_pong(SERVER_STATE->outgoing->data, data);
                        SERVER_STATE->outgoing->address = SERVER_STATE->incoming->address;
                        SERVER_STATE->outgoing->len = len;
                        if (SDLNet_UDP_Send(server, -1, SERVER_STATE->outgoing) > 0)
                        {
                            printf("Ponged %s.\n", addrstr);
                        }
                    }
                    else if (strncmp(cmd, CLIENT_COMMAND_MOVE, 4) == 0)
                    {
                        double x, y, z, angle;
                        client_move_from(payload, &x, &y, &z, &angle);
                        player_move_event_t* ev = ServerPlayerMoveEvent_new(SERVER_STATE->conns[conn_i].player_id, x, y, z, angle);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_CHAT, 4) == 0) {
                        char* message;
                        client_chat_from(payload, &message);
                        player_chat_event_t* ev = ServerPlayerChatEvent_new(SERVER_STATE->conns[conn_i].player_id, message);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else {
                        printf("Unknown command: %s\n", cmd);
                    }

                    cursor += 6 + payload_len + 1;
                }
            }
            else if (ready < 0)
            {
                printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
            }

            usleep(1000);
            // Update the elapsed time since the last tick
            INSTANT_NOW(&cur_time);
            elapsed = INSTANT_DIFF_MS(cur_time, SERVER_STATE->last_tick);
        }

        // Update the world state
        if (world_queue_length(&SERVER_STATE->world) > 0)
        {
            printf("Processing %d events...\n", world_queue_length(&SERVER_STATE->world));
            world_update(&SERVER_STATE->world);
        }

        // Update the elapsed time since the last tick
        INSTANT_NOW(&SERVER_STATE->last_tick);
    }
    // TODO: Kick all players

    SDLNet_FreePacket(SERVER_STATE->incoming);
    SDLNet_FreePacket(SERVER_STATE->outgoing);

    // Clean-up
    printf("Shutting down...\n");
    world_destroy(&SERVER_STATE->world);
    free(SERVER_STATE);
    SDLNet_UDP_Close(server);
    SDLNet_Quit();
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("DOOM-like server v0.0.1 - HELO!\n");
    if (argc <= 1)
    {
        printf("Insufficient argument: missing port number.\nUsage: server <port>");
        return -1;
    }
    uint16_t port = (uint16_t)atoi(argv[1]);

    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Running server on port %d...\n", port);
    run_server(port);
    printf("Exiting...\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>
#include <time.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef _LIB_DOOM_NET_UTIL_H
#include "../include/net/util.h"
#endif

#ifndef _LIB_DOOM_NET_TRACKETCONNECTION_H
#include "../include/net/tracked_connection.h"
#endif

#ifndef _LIB_DOOM_NET_PACKET_SERVER_H
#include "../include/net/packet/server.h"
#endif

#ifndef _LIB_DOOM_NET_PACKET_CLIENT_H
#include "../include/net/packet/client.h"
#endif

#define MAX_CLIENTS 4
// 20 tick per second
#define SERVER_TICK_MS 50 

#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
#define INSTANT_DIFF_MS(a, b) ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)

typedef struct timespec Instant;

static int SERVER_RUNNING = 1;

int compare_instant(const Instant* a, const Instant* b) {
    if (a->tv_sec < b->tv_sec) return -1;
    if (a->tv_sec > b->tv_sec) return 1;
    if (a->tv_nsec < b->tv_nsec) return -1;
    if (a->tv_nsec > b->tv_nsec) return 1;
    return 0;
}

// Interrupt signal handler
//
// Handle in particular SIGINT (Ctrl+C), SIGQUIT, and SIGTERM, for a graceful shutdown.
void signal_handler(int sig) {
    switch (sig) {
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

int run_server(uint16_t port) {
    printf("Initializing SDL_net...\n");
    SDLNet_Init();
    char addrstr[24];

    // Create socket
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return -1;
    }
    UDPsocket server = SDLNet_UDP_Open(port);
    if (server == NULL) {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return -1;
    }

    addrtocstr(&ip, addrstr);
    printf("Listening on %s...\n", addrstr);

    // Listen for incoming packets
    UDPpacket* incoming = SDLNet_AllocPacket(1024);
    UDPpacket* outgoing = SDLNet_AllocPacket(1024);
    assert(incoming != NULL);
    assert(outgoing != NULL);

    Instant tick_time, cur_time;
    TrackedConnection clients[MAX_CLIENTS];
    int client_count = 0;
    while(SERVER_RUNNING > 0) {
        // New tick!
        INSTANT_NOW(&tick_time);
        INSTANT_NOW(&cur_time);
        int elapsed = INSTANT_DIFF_MS(cur_time, tick_time);
        // While there's time left in this tick, process incoming packets.
        while(elapsed < SERVER_TICK_MS) {
            // Process an incoming packet
            int ready = SDLNet_UDP_Recv(server, incoming);
            if (ready == 1) {
                addrtocstr(&incoming->address, addrstr);
                char sdata[2048] = {0};
                memcpy(sdata, (char*) incoming->data, incoming->len);
                printf("incoming packet from %s > %s\n", addrstr, sdata);

                int cursor = 0;
                char cmd[5] = {0};
                int16_t payload_len = 0;
                uint8_t* payload = NULL;
                // For each message in the packet
                while (cursor + 6 <= incoming->len) {
                    memcpy(cmd, sdata + cursor, 4);
                    payload_len = read_uint16((uint8_t*) (sdata + cursor + 4));
                    payload = (uint8_t*) (sdata + cursor + 6);
                    if (cursor + 6 + payload_len + 1 > incoming->len) {
                        printf("Invalid packet: message length exceeds packet length.\n");
                        break;
                    }

                    if (strncmp(cmd, CLIENT_COMMAND_JOIN, 4) == 0) {
                        char* player_name = read_cstring(payload);
                        printf("Player %s is joining the game.\n", player_name);
                        outgoing->address = incoming->address;
                        outgoing->channel = -1;
                        if (client_count >= MAX_CLIENTS) {
                            printf("Server is full. Rejecting player %s.\n", player_name);
                            outgoing->len = server_kick(outgoing->data, "server_full");
                            if (SDLNet_UDP_Send(server, -1, outgoing) > 0) {
                                printf("Player %s rejected.\n", player_name);
                            }
                            continue;
                        }
                        outgoing->len = server_acpt(outgoing->data, client_count);
                        if (SDLNet_UDP_Send(server, -1, outgoing) > 0) {
                            printf("Player %s joined the game.\n", player_name);
                        }
                        clients[client_count].ip = incoming->address;
                        clients[client_count].player_id = client_count;
                        client_count++;
                    } else if (strncmp(cmd, CLIENT_COMMAND_KATC, 4) == 0) {
                        printf("Keep-alive from %s.\n", addrstr);
                    } else if (strncmp(cmd, CLIENT_COMMAND_PONG, 4) == 0) {
                        uint64_t data = read_uint64(payload);
                        printf("Pong from %s: %lu.\n", addrstr, data);
                    } else if (strncmp(cmd, CLIENT_COMMAND_QUIT, 4) == 0) {
                        if (find_conn_by_ip(clients, client_count, &incoming->address) >= 0) {
                            printf("Player %s is quitting the game.\n", addrstr);
                            outgoing->channel = -1;
                            outgoing->len = server_quit(outgoing->data, client_count);
                            broadcast(&server, clients, client_count, outgoing->data, outgoing->len);
                            int cid = find_conn_by_ip(clients, client_count, &incoming->address);
                            clients[cid] = clients[client_count - 1];
                            client_count--;
                        } else {
                            printf("Unknown client %s intended to quit the game.\n", addrstr);
                        }
                    } else {
                        printf("Unknown command: %s.\n", cmd);
                    }
                }
            } else if (ready < 0) {
                printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
            }

            // Update the elapsed time since the last tick
            INSTANT_NOW(&cur_time);
            elapsed = INSTANT_DIFF_MS(cur_time, tick_time);
        }
    }
    for (int i = 0; i < client_count; i++) {
        TrackedConnection* client = &clients[i];
        addrtocstr(&client->ip, addrstr);
        printf("Closing connection with %s...", addrstr);
        outgoing->address = client->ip;
        outgoing->len = server_kick(outgoing->data, "server_shutting_down");
        if (SDLNet_UDP_Send(server, -1, outgoing) > 0) {
            printf(" Kicked.");
        }
        printf("\n");
    }
    SDLNet_FreePacket(incoming);
    SDLNet_FreePacket(outgoing);

    // Clean-up
    printf("Shutting down...\n");
    SDLNet_UDP_Close(server);
    SDLNet_Quit();
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("DOOM-like server v0.0.1 - HELO!\n");
    if (argc <= 1) {
        printf("Insufficient argument: missing port number.\nUsage: server <port>");
        return -1;
    }
    uint16_t port = (uint16_t) atoi(argv[1]);

    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Running server on port %d...\n", port);
    run_server(port);
    printf("Exiting...\n");
    return 0;
}

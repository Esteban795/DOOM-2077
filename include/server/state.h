#ifndef _LIB_DOOM_SERVER_STATE_H
#define _LIB_DOOM_SERVER_STATE_H

#include "../ecs/world.h"
#include "../net/tracked_connection.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#define MAX_CLIENTS 4

// 20 tick per second
#define SERVER_TICK_MS 33 

#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
#define INSTANT_DIFF_MS(a, b) ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)

typedef struct timespec Instant;

typedef struct {
    world_t world;
    UDPsocket sock;
    tracked_connection_t conns[MAX_CLIENTS];
    int conn_count;
    Instant last_tick;
    int running;
    UDPpacket* incoming;
    UDPpacket* outgoing;
} server_state_t;

static server_state_t* SERVER_STATE;

int compare_instant(const Instant* a, const Instant* b);

#endif
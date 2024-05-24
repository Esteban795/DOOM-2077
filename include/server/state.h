#ifndef _LIB_DOOM_SERVER_STATE_H
#define _LIB_DOOM_SERVER_STATE_H

#include "../ecs/world.h"
#include "../net/tracked_connection.h"
#include "../settings.h"
#include "../wad_data.h"
#include "task.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef PLAYER_MAXIMUM
#define MAX_CLIENTS 4
#endif

#ifdef PLAYER_MAXIMUM
#define MAX_CLIENTS PLAYER_MAXIMUM
#endif

// 60 ticks per second
#define SERVER_TICK_MS 16 

#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
#define INSTANT_DIFF_MS(a, b) ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)

typedef struct timespec Instant;

// The game state
typedef enum {
    GAME_STATE_WAITING,
    GAME_STATE_COOLDOWN,
    GAME_STATE_RUNNING,
    GAME_STATE_ENDING,
} game_state_t;

// The server state
typedef struct {
    // The world ECS
    world_t world;
    // The server socket
    UDPsocket sock;
    // Connection sockets to clients
    tracked_connection_t conns[MAX_CLIENTS];
    int conn_count;
    // The last tick time
    Instant last_tick;
    // The incoming and outgoing packets
    UDPpacket* incoming;
    UDPpacket* outgoing;
    // The WAD data & map name
    wad_data* wad_data;
    char* map_name;
    // The task executor
    task_executor_t task_executor;
    // The game state
    game_state_t game_state;
    entity_t** doors;
    int door_count;
    entity_t** lifts;
    int lift_count;
} server_state_t;

#define SERVER_STATE (&server_state)
extern server_state_t server_state;

extern int SERVER_RUNNING;

int compare_instant(const Instant* a, const Instant* b);

#endif
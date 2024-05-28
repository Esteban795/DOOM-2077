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

/// Server tick-rate in ms  
/// Set to 60 ticks per second
#define SERVER_TICK_MS 16 

/// Set an instant to the current time/date.
#define INSTANT_NOW(t) clock_gettime(CLOCK_MONOTONIC, t)
/// Get the difference in milliseconds between two instants.
#define INSTANT_DIFF_MS(a, b) ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_nsec - b.tv_nsec) / 1000000)

typedef struct timespec Instant;

/// The game state, determining the current state of the party.
typedef enum {
    /// The game is waiting for players to join.
    GAME_STATE_WAITING,
    /// The game is in cooldown, waiting for the game to start.  
    /// This state is used to allow the clients to prepare for the game,
    /// and to allow the server to wait for other players to join.
    GAME_STATE_COOLDOWN,
    /// The game is running.
    GAME_STATE_RUNNING,
    /// The game is ending, or has ended.  
    /// This state is used to allow a small delay before the game restarts, and 
    /// let the clients take a breath before the next game.
    GAME_STATE_ENDING,
} game_state_t;

// The server state
/// @brief Represents the state of the server.
typedef struct {
    /// @brief The server ECS world.
    world_t world;
    /// @brief The server socket.
    UDPsocket sock;
    /// @brief The player list.
    tracked_connection_t conns[MAX_CLIENTS];
    int conn_count;
    /// @brief The last tick time executed.
    Instant last_tick;
    /// @brief The incoming packet, to reuse.
    UDPpacket* incoming;
    /// @brief The outgoing packet, to reuse.
    UDPpacket* outgoing;
    /// @brief The WAD data loaded.
    wad_data* wad_data;
    /// @brief The current loaded map's name.
    char* map_name;
    // The task executor
    task_executor_t task_executor;
    // The game state
    game_state_t game_state;
    // Door-mappings and lift-mappings
    /// @brief The entities representing the doors.
    entity_t** doors;
    int door_count;
    /// @brief The entities representing the lifts.
    entity_t** lifts;
    int lift_count;
} server_state_t;

/// @brief Server state handle.
#define SERVER_STATE (&server_state)
extern server_state_t server_state;

extern int SERVER_RUNNING;

int compare_instant(const Instant* a, const Instant* b);

#endif
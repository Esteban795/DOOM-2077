#ifndef _LIB_DOOM_NET_TRACKETCONNECTION_H
#define _LIB_DOOM_NET_TRACKETCONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

/// @brief Represents a connected player.
///
/// This struct is used to keep track of the connected players and their network correspondance.
typedef struct {
    /// @brief The IP address and port of the player.
    IPaddress ip;
    /// @brief The player's entity ID.
    uint64_t player_id;
} tracked_connection_t;

/// @brief broadcast a message to all connected players.
/// @param sock the socket to send the message.
/// @param connections the list of connected players.
/// @param num_connections the number of connected players.
/// @param buf the message to send.
/// @param len the length of the message.
/// @return the number of packets sucessfully sent.
int broadcast(UDPsocket* sock, tracked_connection_t* connections, int num_connections, uint8_t* buf, int len);

/// @brief broadcast a message to all connected players except one.
/// @param sock the socket to send the message.
/// @param connections the list of connected players.
/// @param num_connections the number of connected players.
/// @param player_id the player to exclude.
/// @param buf the message to send.
/// @param len the length of the message.
/// @return the number of packets sucessfully sent.
int broadcast_except(UDPsocket* sock, tracked_connection_t* connections, int num_connections, uint64_t player_id, uint8_t* buf, int len);

/// @brief find a connection by IP address.
/// @param connections the list of connected players.
/// @param num_connections the number of connected players.
/// @param ip the IP address to search for.
/// @return the index of the connection, or -1 if not found.
int find_conn_by_ip(tracked_connection_t* connections, int num_connections, IPaddress* ip);

/// @brief find a connection by player ID.
/// @param connections the list of connected players.
/// @param num_connections the number of connected players.
/// @param id the player ID to search for.
/// @return the index of the connection, or -1 if not found.
int find_conn_by_id(tracked_connection_t* connections, int num_connections, uint64_t id);

#endif
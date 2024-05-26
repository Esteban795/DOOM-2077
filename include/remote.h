#ifndef REMOTE_H
#define REMOTE_H

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#include <SDL2/SDL_net.h>
#endif

#include "structs.h"

typedef struct RemoteServer remote_server_t;
typedef struct Engine engine;

// Initialize the remote connection to the server
// Returns 0 on success, -1 on failure
int remote_init(remote_server_t *server, char *addr, int port, char* player_name);

// Destroy the remote connection to the server
// 
// This function should be called when the client is done with the server,
// to free the resources allocated by remote_init. It will close the socket
// and free the server structure.
// Returns 0 on success, -1 on failure
void remote_destroy(remote_server_t* server);

// Disconnect from the server
// By sending a QUIT packet to the server
void remote_disconnect(remote_server_t* r);

// Update the remote connection to the server
//
// This function should be called every frame to check for incoming packets
// from the server. If it receives a packet from the server, it will handle it.
// Returns 0 on success, -1 on failure, 1 if the server has kicked the client.
int remote_update(engine* e, remote_server_t* r);

// Trigger a door, sync it with the server if necessary
void client_door_trigger(engine* e, uint64_t door_id);

// Trigger a lift, sync it with the server if necessary
void client_lift_trigger(engine* e, uint64_t lift_id);

/// Send a chat message to the server
///
/// This function will send a chat message to the server, which will broadcast it to all clients.
///
/// \remark If the client is not connected to a server, this function will trigger a chat event locally.
void remote_send_chat(engine* e, char *message);
#endif
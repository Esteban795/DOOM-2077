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

typedef struct {
    IPaddress ip;
    uint64_t player_id;
} tracked_connection_t;

int broadcast(UDPsocket* sock, tracked_connection_t* connections, int num_connections, uint8_t* buf, int len);

int find_conn_by_ip(tracked_connection_t* connections, int num_connections, IPaddress* ip);

int find_conn_by_id(tracked_connection_t* connections, int num_connections, uint64_t id);

#endif
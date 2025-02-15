#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "../../include/net/tracked_connection.h"
#include "../../include/net/util.h"

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

int broadcast(UDPsocket* sock, tracked_connection_t* connections, int num_connections, uint8_t* buf, int len) {
    UDPpacket** packets = SDLNet_AllocPacketV(num_connections, len);
    assert(packets != NULL);
    for (int i = 0; i < num_connections; i++) {
        packets[i]->channel = -1;
        packets[i]->address = connections[i].ip;
        memcpy(packets[i]->data, buf, len);
        packets[i]->len = len;
    }
    int result = SDLNet_UDP_SendV(*sock, packets, num_connections);
    SDLNet_FreePacketV(packets);
    return result;
}

int broadcast_except(UDPsocket* sock, tracked_connection_t* connections, int num_connections, uint64_t player_id, uint8_t* buf, int len) {
    UDPpacket** packets = SDLNet_AllocPacketV(num_connections, len);
    assert(packets != NULL);
    for (int i = 0; i < num_connections; i++) {
        packets[i]->channel = -1;
        packets[i]->address = connections[i].ip;
        memcpy(packets[i]->data, buf, len);
        packets[i]->len = len;
    }

    int idx = find_conn_by_id(connections, num_connections, player_id);
    int result;
    if (idx >= 0) {
        // Swap the last packet with the one we want to exclude
        UDPpacket* tmp = packets [idx];
        packets[idx] = packets[num_connections - 1];
        packets[num_connections - 1] = tmp;
        result = SDLNet_UDP_SendV(*sock, packets, num_connections - 1);
    } else {
        result = SDLNet_UDP_SendV(*sock, packets, num_connections);
    }
    SDLNet_FreePacketV(packets);
    return result;
}

int find_conn_by_ip(tracked_connection_t* connections, int num_connections, IPaddress* ip) {
    for (int i = 0; i < num_connections; i++) {
        if (cmp_addr(&connections[i].ip, ip) == 0) {
            return i;
        }
    }
    return -1;
}

int find_conn_by_id(tracked_connection_t* connections, int num_connections, uint64_t id) {
    for (int i = 0; i < num_connections; i++) {
        if (connections[i].player_id == id) {
            return i;
        }
    }
    return -1;
}
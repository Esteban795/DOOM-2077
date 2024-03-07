#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

#ifndef _LIB_DOOM_NET_UTIL_H
#include "../include/net/util.h"
#endif

#define MAX_CLIENTS 4

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
    UDPpacket* incoming = SDLNet_AllocPacket(2048);
    for(;;) {
        int ready = SDLNet_UDP_Recv(server, incoming);
        if (ready == 1) {
            addrtocstr(&incoming->address, addrstr);
            char sdata[2048] = {0};
            strncpy(sdata, (char*) incoming->data, incoming->len);
            printf("incoming packet from %s > %s\n", addrstr, sdata);

            // Echo back to sender
            int sent = SDLNet_UDP_Send(server, -1, incoming);
            if (sent < 0) {
                printf("SDLNet_UDP_Send: %s\n", SDLNet_GetError());
            }
        } else if (ready < 0) {
            printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
        }
    }
    SDLNet_FreePacket(incoming);

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
    printf("Running server on port %d...\n", port);
    run_server(port);
    printf("Exiting...\n");
    return 0;
}

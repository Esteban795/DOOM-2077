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

int run_server(uint16_t port) {
    printf("Initializing SDL_net...\n");
    SDLNet_Init();

    // Open socket
    IPaddress ip;
    TCPsocket server;
    if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {
        printf("ERR SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return -1;
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        printf("ERR SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return -1;
    }
    char addrstr[24];
    addrtocstr(&ip, addrstr);
    printf("Listening on %s\n", addrstr);

    while(true) {}

    // Clean-up
    printf("Shutting down...\n");
    SDLNet_TCP_Close(server);
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

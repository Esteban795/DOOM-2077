#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

int main(int argc, char const *argv[])
{
    printf("DOOM-like server v0.0.1 - HELO!\n");
    if (argc <= 1) {
        printf("Insufficient argument: missing port number.\nUsage: server <port>");
        return -1;
    }
    int port = atoi(argv[1]);
    printf("Listening on port %d...\n", port);

    printf("Exiting...\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_NET_UTIL_H
#include "../../include/net/util.h"
#endif

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

void addrtocstr(IPaddress* ip, char str[24]) {
    uint8_t* addr = (uint8_t*) (&ip->host);
    uint16_t port = SDLNet_Read16(&ip->port);
    sprintf(str, "%d.%d.%d.%d:%d", addr[0], addr[1], addr[2], addr[3], port);
}
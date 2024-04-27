#ifndef _LIB_DOOM_SERVER_OUTQUEUE_H
#define _LIB_DOOM_SERVER_OUTQUEUE_H

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

int queue_command(IPAddress* addr, uint8_t* buf, int len);

#endif
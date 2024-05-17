#ifndef _LIB_DOOM_NET_UTIL_H
#define _LIB_DOOM_NET_UTIL_H

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

void addrtocstr(IPaddress* ip, char str[24]);
int cmp_addr(IPaddress* ip1, IPaddress* ip2);

void write_uint8be(uint8_t* buf, uint8_t data);
void write_uint16be(uint8_t* buf, uint16_t data);
void write_uint32be(uint8_t* buf, uint32_t data);
void write_uint64be(uint8_t* buf, uint64_t data);

int write_cstring(uint8_t* buf, char* data);

uint8_t read_uint8be(uint8_t* buf);
uint16_t read_uint16be(uint8_t* buf);
uint32_t read_uint32be(uint8_t* buf);
uint64_t read_uint64be(uint8_t* buf);

int8_t  read_int8be(uint8_t* buf);
int16_t read_int16be(uint8_t* buf);
int32_t read_int32be(uint8_t* buf);
int64_t read_int64be(uint8_t* buf);

char* read_cstring(uint8_t* buf);

char* read_copy_string(uint8_t* buf, int len);

#endif
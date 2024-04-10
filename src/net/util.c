#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/net/util.h"

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

int cmp_addr(IPaddress* ip1, IPaddress* ip2) {
    if (ip1->host == ip2->host && ip1->port == ip2->port) {
        return 0;
    }
    return 1;
}

void write_uint8be(uint8_t* buf, uint8_t data) {
    buf[0] = data;
}

void write_uint16be(uint8_t* buf, uint16_t data) {
    buf[0] = data >> 8;
    buf[1] = data & 0xFF;
}

void write_uint32be(uint8_t* buf, uint32_t data) {
    buf[0] = data >> 24;
    buf[1] = data >> 16 & 0xFF;
    buf[2] = data >> 8  & 0xFF;
    buf[3] = data       & 0xFF;
}

void write_uint64be(uint8_t* buf, uint64_t data) {
    buf[0] = data >> 56;
    buf[1] = data >> 48 & 0xFF;
    buf[2] = data >> 40 & 0xFF;
    buf[3] = data >> 32 & 0xFF;
    buf[4] = data >> 24 & 0xFF;
    buf[5] = data >> 16 & 0xFF;
    buf[6] = data >> 8  & 0xFF;
    buf[7] = data       & 0xFF;
}

int write_cstring(uint8_t* buf, char* data) {
    int len = strlen(data);
    memcpy(buf, data, len + 1);
    return len + 1;
}

uint8_t read_uint8be(uint8_t* buf) {
    return buf[0];
}

uint16_t read_uint16be(uint8_t* buf) {
    uint16_t data = (uint16_t) buf[1];
    data |= ((uint16_t) buf[0]) << 8;
    return data;
}

uint32_t read_uint32be(uint8_t* buf) {
    uint32_t data = (uint32_t) buf[3];
    data |= ((uint32_t) buf[2]) << 8;
    data |= ((uint32_t) buf[1]) << 16;
    data |= ((uint32_t) buf[0]) << 24;
    return data;
}

uint64_t read_uint64be(uint8_t* buf) {
    uint64_t data = (uint64_t) buf[7];
    data |= ((uint64_t) buf[6]) << 8;
    data |= ((uint64_t) buf[5]) << 16;
    data |= ((uint64_t) buf[4]) << 24;
    data |= ((uint64_t) buf[3]) << 32;
    data |= ((uint64_t) buf[2]) << 40;
    data |= ((uint64_t) buf[1]) << 48;
    data |= ((uint64_t) buf[0]) << 56;
    return data;
}

char* read_cstring(uint8_t* buf) {
    return (char*) buf;
}

char* read_copy_string(uint8_t* buf, int len) {
    char* str = malloc(len + 1);
    memcpy(str, buf, len);
    str[len] = '\0';
    return str;
}
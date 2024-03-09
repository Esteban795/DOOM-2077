#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_NET_PACKET_SERVER_H
#include "../../../include/net/packet/server.h"
#endif

#ifndef _LIB_DOOM_NET_UTIL_H
#include "../../../include/net/util.h"
#endif

const char* SERVER_COMMAND_ACPT = "ACPT";
const char* SERVER_COMMAND_JOIN = "JOIN";
const char* SERVER_COMMAND_KICK = "KICK";
const char* SERVER_COMMAND_PONG = "PONG";
const char* SERVER_COMMAND_QUIT = "QUIT";

int server_acpt(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, SERVER_COMMAND_ACPT, 4);
    write_uint16(buf+4, 8); 
    write_uint64(buf+6, player_id);
    buf[14] = '\n'; 
    return 4 + 2 + 8 + 1;
}

int server_join(uint8_t* buf, uint64_t player_id, char* player_name) {
    memcpy(buf, SERVER_COMMAND_JOIN, 4);
    write_uint64(buf+6, player_id);
    int clen = write_cstring(buf + 14, player_name);
    write_uint16(buf+4, 8 + clen);
    buf[14 + clen] = '\n';
    return 4 + 2 + 8 + clen + 1;
}

int server_kick(uint8_t* buf, char* reason) {
    memcpy(buf, SERVER_COMMAND_KICK, 4);
    int clen = write_cstring(buf + 6, reason);
    write_uint16(buf+4, clen);
    buf[6 + clen] = '\n';
    return 4 + 2 + clen + 1;
}

int server_pong(uint8_t* buf, uint64_t data) {
    memcpy(buf, SERVER_COMMAND_PONG, 4);
    write_uint16(buf + 4, 8);
    write_uint64(buf + 6, data);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}

int server_quit(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, SERVER_COMMAND_QUIT, 4);
    write_uint16(buf + 4, 8);
    write_uint64(buf + 6, player_id);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}
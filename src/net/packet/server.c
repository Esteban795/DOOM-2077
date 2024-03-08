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

const char* COMMAND_ACPT = "ACPT";
const char* COMMAND_JOIN = "JOIN";
const char* COMMAND_KICK = "KICK";
const char* COMMAND_PONG = "PONG";
const char* COMMAND_QUIT = "QUIT";

int server_acpt(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, COMMAND_ACPT, 4);
    write_uint64(buf, player_id);
    buf[12] = '\n'; 
    return 4 + 8 + 1;
}

int server_join(uint8_t* buf, uint64_t player_id, char* player_name) {
    memcpy(buf, COMMAND_JOIN, 4);
    write_uint64(buf, player_id);
    int clen = write_cstring(buf + 12, player_name);
    buf[12 + clen] = '\n';
    return 4 + 8 + clen + 1;
}

int server_kick(uint8_t* buf, char* reason) {
    memcpy(buf, COMMAND_KICK, 4);
    int clen = write_cstring(buf + 4, reason);
    buf[4 + clen] = '\n';
    return 4 + clen + 1;
}

int server_pong(uint8_t* buf, uint64_t data) {
    memcpy(buf, COMMAND_PONG, 4);
    write_uint64(buf, data);
    buf[12] = '\n';
    return 4 + 8 + 1;
}

int server_quit(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, COMMAND_QUIT, 4);
    write_uint64(buf, player_id);
    buf[12] = '\n';
    return 4 + 8 + 1;
}
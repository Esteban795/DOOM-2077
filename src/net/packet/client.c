#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_NET_PACKET_CLIENT_H
#include "../../../include/net/packet/client.h"
#endif

#ifndef _LIB_DOOM_NET_UTIL_H
#include "../../../include/net/util.h"
#endif

const char* COMMAND_JOIN = "JOIN";
const char* COMMAND_KATC = "KATC";
const char* COMMAND_PONG = "PING";
const char* COMMAND_QUIT = "QUIT";

int client_join(uint8_t* buf, char* player_name) {
    memcpy(buf, COMMAND_JOIN, 4);
    int clen = write_cstring(buf + 4, player_name);
    buf[4 + clen] = '\n';
    return 4 + clen + 1;
}

int client_keep_alive(uint8_t* buf) {
    memcpy(buf, COMMAND_KATC, 4);
    buf[4] = '\n';
    return 4 + 1;
}

int client_ping(uint8_t* buf, uint64_t data) {
    memcpy(buf, COMMAND_PONG, 4);
    write_uint64(buf, data);
    buf[12] = '\n';
    return 4 + 8 + 1;
}

int client_quit(uint8_t* buf) {
    memcpy(buf, COMMAND_QUIT, 4);
    buf[4] = '\n';
    return 4 + 1;
}
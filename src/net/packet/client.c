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

const char* CLIENT_COMMAND_JOIN = "JOIN";
const char* CLIENT_COMMAND_KATC = "KATC";
const char* CLIENT_COMMAND_PONG = "PING";
const char* CLIENT_COMMAND_QUIT = "QUIT";

int client_join(uint8_t* buf, char* player_name) {
    memcpy(buf, CLIENT_COMMAND_JOIN, 4);
    int clen = write_cstring(buf + 6, player_name);
    write_uint16be(buf + 4, clen);
    buf[6 + clen] = '\n';
    return 4 + 2 + clen + 1;
}

int client_keep_alive(uint8_t* buf) {
    memcpy(buf, CLIENT_COMMAND_KATC, 4);
    write_uint16be(buf + 4, 0);
    buf[6] = '\n';
    return 4 + 2 + 1;
}

int client_ping(uint8_t* buf, uint64_t data) {
    memcpy(buf, CLIENT_COMMAND_PONG, 4);
    write_uint16be(buf + 4, 8);
    write_uint64be(buf + 6, data);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}

int client_quit(uint8_t* buf) {
    memcpy(buf, CLIENT_COMMAND_QUIT, 4);
    write_uint16be(buf + 4, 0);
    buf[6] = '\n';
    return 4 + 2 + 1;
}
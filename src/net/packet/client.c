#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "../../../include/net/packet/client.h"
#include "../../../include/net/util.h"

const char* CLIENT_COMMAND_JOIN = "JOIN";
const char* CLIENT_COMMAND_KATC = "KATC";
const char* CLIENT_COMMAND_PING = "PING";
const char* CLIENT_COMMAND_QUIT = "QUIT";
const char* CLIENT_COMMAND_MOVE = "MOVE";
const char* CLIENT_COMMAND_CHAT = "CHAT";

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
    memcpy(buf, CLIENT_COMMAND_PING, 4);
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

int client_move(uint8_t* buf, double x, double y, double z, double angle) {
    memcpy(buf, CLIENT_COMMAND_MOVE, 4);
    write_uint16be(buf + 4, 8*3);
    write_uint64be(buf + 6,  (uint64_t) (x*1000));
    write_uint64be(buf + 14, (uint64_t) (y*1000));
    write_uint64be(buf + 22, (uint64_t) (z*1000));
    write_uint64be(buf + 30, (uint64_t) (angle*1000));
    buf[38] = '\n';
    return 4 + 2 + 8*4 + 1;
}

int client_chat(uint8_t* buf, char* message) {
    memcpy(buf, CLIENT_COMMAND_CHAT, 4);
    int clen = write_cstring(buf + 6, message);
    write_uint16be(buf + 4, clen);
    buf[6 + clen] = '\n';
    return 4 + 2 + clen + 1;
}

int client_join_from(uint8_t* buf, char** player_name) {
    int clen = read_uint16be(buf + 4);
    *player_name = malloc(clen * sizeof(char));
    memcpy(*player_name, buf + 6, clen);
    *player_name[clen-1] = '\0';
    return 4 + 2 + clen + 1;
}

int client_ping_from(uint8_t* buf, uint64_t* data) {
    *data = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int client_move_from(uint8_t* buf, double* x, double* y, double* z, double* angle) {
    *x = ((double) read_int64be(buf + 6)) / 1000.0;
    *y = ((double) read_int64be(buf + 6 + 8)) / 1000.0;
    *z = ((double) read_int64be(buf + 6 + (8*2))) / 1000.0;
    *angle = ((double) read_int64be(buf + 6 + (8*3))) / 1000.0;
    return 4 + 2 + 8*4 + 1;

}

int client_chat_from(uint8_t* buf, char** message) {
    int clen = read_uint16be(buf + 4);
    *message = malloc(clen * sizeof(char));
    memcpy(*message, buf + 6, clen);
    *message[clen-1] = '\0';
    return 4 + 2 + clen + 1;
}

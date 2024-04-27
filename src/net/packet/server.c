#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../../include/net/packet/server.h"
#include "../../../include/net/util.h"

const char* SERVER_COMMAND_ACPT = "ACPT";
const char* SERVER_COMMAND_JOIN = "JOIN";
const char* SERVER_COMMAND_KICK = "KICK";
const char* SERVER_COMMAND_PONG = "PONG";
const char* SERVER_COMMAND_QUIT = "QUIT";
const char* SERVER_COMMAND_CHAT = "CHAT";
const char* SERVER_COMMAND_TELL = "TELL";
const char* SERVER_COMMAND_SCOR = "SCOR";


int server_acpt(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, SERVER_COMMAND_ACPT, 4);
    write_uint16be(buf+4, 8); 
    write_uint64be(buf+6, player_id);
    buf[14] = '\n'; 
    return 4 + 2 + 8 + 1;
}

int server_join(uint8_t* buf, uint64_t player_id, char* player_name) {
    memcpy(buf, SERVER_COMMAND_JOIN, 4);
    write_uint64be(buf+6, player_id);
    int clen = write_cstring(buf + 14, player_name);
    write_uint16be(buf+4, 8 + clen);
    buf[14 + clen] = '\n';
    return 4 + 2 + 8 + clen + 1;
}

int server_kick(uint8_t* buf, char* reason) {
    memcpy(buf, SERVER_COMMAND_KICK, 4);
    int clen = write_cstring(buf + 6, reason);
    write_uint16be(buf+4, clen);
    buf[6 + clen] = '\n';
    return 4 + 2 + clen + 1;
}

int server_pong(uint8_t* buf, uint64_t data) {
    memcpy(buf, SERVER_COMMAND_PONG, 4);
    write_uint16be(buf + 4, 8);
    write_uint64be(buf + 6, data);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}

int server_quit(uint8_t* buf, uint64_t player_id) {
    memcpy(buf, SERVER_COMMAND_QUIT, 4);
    write_uint16be(buf + 4, 8);
    write_uint64be(buf + 6, player_id);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}

int server_player_move(uint8_t* buf, uint64_t player_id, double x, double y, double z, double angle) {
    memcpy(buf, SERVER_COMMAND_MOVE, 4);
    write_uint16be(buf + 4, 8*5);
    write_uint64be(buf + 6, player_id);
    write_uint64be(buf + 14, (uint64_t) (x*1000));
    write_uint64be(buf + 22, (uint64_t) (y*1000));
    write_uint64be(buf + 30, (uint64_t) (z*1000));
    write_uint64be(buf + 38, (uint64_t) (angle*1000));
    buf[46] = '\n';
    return 4 + 2 + 8*5 + 1;
}

int server_player_chat(uint8_t* buf, uint64_t player_id, char* message) {
    memcpy(buf, SERVER_COMMAND_CHAT, 4);
    int clen = write_cstring(buf + 6 + 8, message);
    write_uint16be(buf + 4, 8 + clen);
    write_uint64be(buf + 6, player_id);
    buf[14 + clen] = '\n';
    return 4 + 2 + 8 + clen + 1;
}

int server_server_chat(uint8_t* buf, char* message, bool is_broadcast, bool is_title) {
    memcpy(buf, SERVER_COMMAND_TELL, 4);
    uint8_t flags = 0;
    if (is_broadcast) {
        flags |= 0x01;
    }
    if (is_title) {
        flags |= 0x02;
    }
    write_uint8be(buf + 6, flags);
    int clen = write_cstring(buf + 7, message);
    write_uint16be(buf + 4, 1 + clen);
    buf[7 + clen] = '\n';
    return 4 + 2 + 1 + clen + 1;
}

int server_scoreboard_update(uint8_t* buf, uint16_t entries_count, char** names, uint16_t* deaths, uint16_t* kills) {
    memcpy(buf, SERVER_COMMAND_SCOR, 4);
    write_uint16be(buf + 6, entries_count);
    int plen = 2;
    for (int i = 0; i < entries_count; i++) {
        plen += write_cstring(buf + 8 + plen, names[i]);
    }
    for (int i = 0; i < entries_count; i++) {
        write_uint16be(buf + 8 + plen, deaths[i]);
        plen += 2;
    }
    for (int i = 0; i < entries_count; i++) {
        write_uint16be(buf + 8 + plen, kills[i]);
        plen += 2;
    }
    write_uint16be(buf + 4, plen);
    buf[8 + plen] = '\n';
    return 4 + 2 + plen + 1;
}

int server_acpt_from(uint8_t* buf, uint64_t* player_id) {
    *player_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_join_from(uint8_t* buf, uint64_t* player_id, char** player_name) {
    *player_id = read_uint64be(buf + 6);
    int clen = read_uint16be(buf + 4) - 8;
    *player_name = malloc(clen * sizeof(char));
    memcpy(*player_name, buf + 14, clen);
    *player_name[clen-1] = '\0';
    return 4 + 2 + 8 + clen + 1;

}

int server_kick_from(uint8_t* buf, char** reason) {
    int clen = read_uint16be(buf + 4);
    *reason = malloc(clen * sizeof(char));
    memcpy(*reason, buf + 6, clen);
    *reason[clen-1] = '\0';
    return 4 + 2 + clen + 1;
}

int server_pong_from(uint8_t* buf, uint64_t* data) {
    *data = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_quit_from(uint8_t* buf, uint64_t* player_id) {
    *player_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_player_move_from(uint8_t* buf, uint64_t* player_id, double* x, double* y, double* z, double* angle) {
    *player_id = read_uint64be(buf + 6);
    *x = ((double) read_int64be(buf + 14)) / 1000.0;
    *y = ((double) read_int64be(buf + 22)) / 1000.0;
    *z = ((double) read_int64be(buf + 30)) / 1000.0;
    *angle = ((double) read_int64be(buf + 38)) / 1000.0;
    return 4 + 2 + 8*5 + 1;
}

int server_player_chat_from(uint8_t* buf, uint64_t* player_id, char** message) {
    *player_id = read_uint64be(buf + 6);
    int clen = read_uint16be(buf + 4) - 8;
    *message = malloc(clen * sizeof(char));
    memcpy(*message, buf + 14, clen);
    *message[clen-1] = '\0';
    return 4 + 2 + 8 + clen + 1;
}

int server_server_chat_from(uint8_t* buf, char** message, bool* is_broadcast, bool* is_title) {
    int clen = read_uint16be(buf + 4) - 1;
    *is_broadcast = (buf[6] & 0x01) != 0;
    *is_title = (buf[6] & 0x02) != 0;
    *message = malloc(clen * sizeof(char));
    memcpy(*message, buf + 7, clen);
    *message[clen-1] = '\0';
    return 4 + 2 + 1 + clen + 1;
}

int server_scoreboard_update_from(uint8_t* buf, uint16_t* entries_count, char*** names, uint16_t** deaths, uint16_t** kills) {
    *entries_count = read_uint16be(buf + 6);
    int plen = 2;
    *names = malloc(*entries_count * sizeof(char*));
    for (int i = 0; i < *entries_count; i++) {
        int clen = strnlen((char*) buf + 8 + plen, 128);
        (*names)[i] = malloc(clen * sizeof(char));
        (*names)[i][clen-1] = '\0';
        plen += clen;
    }
    *deaths = malloc(*entries_count * sizeof(uint16_t));
    for (int i = 0; i < *entries_count; i++) {
        (*deaths)[i] = read_uint16be(buf + 8 + plen);
        plen += 2;
    }
    *kills = malloc(*entries_count * sizeof(uint16_t));
    for (int i = 0; i < *entries_count; i++) {
        (*kills)[i] = read_uint16be(buf + 8 + plen);
        plen += 2;
    }
    return 4 + 2 + plen + 1;
}
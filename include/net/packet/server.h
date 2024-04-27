#ifndef _LIB_DOOM_NET_PACKET_SERVER_H
#define _LIB_DOOM_NET_PACKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

extern const char* SERVER_COMMAND_ACPT;
extern const char* SERVER_COMMAND_JOIN;
extern const char* SERVER_COMMAND_KICK;
extern const char* SERVER_COMMAND_PONG;
extern const char* SERVER_COMMAND_QUIT;
extern const char* SERVER_COMMAND_MOVE;
extern const char* SERVER_COMMAND_CHAT;
extern const char* SERVER_COMMAND_TELL;
extern const char* SERVER_COMMAND_SCOR;

int server_acpt(uint8_t* buf, uint64_t player_id);
int server_join(uint8_t* buf, uint64_t player_id, char* player_name);
int server_kick(uint8_t* buf, char* reason);
int server_pong(uint8_t* buf, uint64_t data);
int server_quit(uint8_t* buf, uint64_t player_id);
int server_player_move(uint8_t* buf, uint64_t player_id, double x, double y, double z, double angle);
int server_player_chat(uint8_t* buf, uint64_t player_id, char* message);
int server_server_chat(uint8_t* buf, char* message, bool is_broadcast, bool is_title);
int server_scoreboard_update(uint8_t* buf, uint16_t entries_count, char** names, uint16_t* deaths, uint16_t* kills);

int server_acpt_from(uint8_t* buf, uint64_t* player_id);
int server_join_from(uint8_t* buf, uint64_t* player_id, char** player_name);
int server_kick_from(uint8_t* buf, char** reason);
int server_pong_from(uint8_t* buf, uint64_t* data);
int server_quit_from(uint8_t* buf, uint64_t* player_id);
int server_player_move_from(uint8_t* buf, uint64_t* player_id, double* x, double* y, double* z, double* angle);
int server_player_chat_from(uint8_t* buf, uint64_t* player_id, char** message);
int server_server_chat_from(uint8_t* buf, char** message, bool* is_broadcast, bool* is_title);
int server_scoreboard_update_from(uint8_t* buf, uint16_t* entries_count, char*** names, uint16_t** deaths, uint16_t** kills);
#endif
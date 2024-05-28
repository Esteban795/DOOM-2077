#ifndef _LIB_DOOM_NET_PACKET_CLIENT_H
#define _LIB_DOOM_NET_PACKET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

extern const char* CLIENT_COMMAND_JOIN;
extern const char* CLIENT_COMMAND_KATC;
extern const char* CLIENT_COMMAND_PING;
extern const char* CLIENT_COMMAND_QUIT;
extern const char* CLIENT_COMMAND_MOVE;
extern const char* CLIENT_COMMAND_CHAT;
extern const char* CLIENT_COMMAND_OPEN;
extern const char* CLIENT_COMMAND_CLOS;
extern const char* CLIENT_COMMAND_LASC;
extern const char* CLIENT_COMMAND_LDSC;
extern const char* CLIENT_COMMAND_FIRE;
extern const char* CLIENT_COMMAND_DAMG;

int client_join(uint8_t* buf, char* player_name);
int client_keep_alive(uint8_t* buf);
int client_ping(uint8_t* buf, uint64_t data);
int client_quit(uint8_t* buf);
int client_move(uint8_t* buf, double x, double y, double z, double angle);
int client_chat(uint8_t* buf, char* message);
int client_door_open(uint8_t* buf, uint64_t door_id);
int client_door_close(uint8_t* buf, uint64_t door_id);
int client_lift_ascend(uint8_t* buf, uint64_t door_id);
int client_lift_descend(uint8_t* buf, uint64_t door_id);
int client_fire(uint8_t* buf, int8_t weapon_id);
int client_damage(uint8_t* buf, uint64_t player_id, int8_t weapon_id, float damage);

int client_join_from(uint8_t* buf, char** player_name);
int client_ping_from(uint8_t* buf, uint64_t* data);
int client_move_from(uint8_t* buf, double* x, double* y, double* z, double* angle);
int client_chat_from(uint8_t* buf, char** message);
int client_door_open_from(uint8_t* buf, uint64_t* door_id);
int client_door_close_from(uint8_t* buf, uint64_t* door_id);
int client_lift_ascend_from(uint8_t* buf, uint64_t* door_id);
int client_lift_descend_from(uint8_t* buf, uint64_t* door_id);
int client_fire_from(uint8_t* buf, int8_t* weapon_id);
int client_damage_from(uint8_t* buf, uint64_t* player_id, int8_t* weapon_id, float* damage);
#endif
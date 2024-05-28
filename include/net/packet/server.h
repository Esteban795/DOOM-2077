#ifndef _LIB_DOOM_NET_PACKET_SERVER_H
#define _LIB_DOOM_NET_PACKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../component/weapon.h"

extern const char *SERVER_COMMAND_ACPT;
extern const char *SERVER_COMMAND_JOIN;
extern const char *SERVER_COMMAND_KICK;
extern const char *SERVER_COMMAND_PONG;
extern const char *SERVER_COMMAND_QUIT;
extern const char *SERVER_COMMAND_MOVE;
extern const char *SERVER_COMMAND_CHAT;
extern const char *SERVER_COMMAND_TELL;
extern const char *SERVER_COMMAND_SCOR;
extern const char *SERVER_COMMAND_DAMG;
extern const char *SERVER_COMMAND_HEAL;
extern const char *SERVER_COMMAND_HLTH;
extern const char *SERVER_COMMAND_KILL;
extern const char *SERVER_COMMAND_LMAP;
extern const char *SERVER_COMMAND_OPEN;
extern const char *SERVER_COMMAND_CLOS;
extern const char *SERVER_COMMAND_DOST;
extern const char *SERVER_COMMAND_LASC;
extern const char *SERVER_COMMAND_LDSC;
extern const char *SERVER_COMMAND_L_ST;
extern const char *SERVER_COMMAND_GSTA;
extern const char *SERVER_COMMAND_GEND;
extern const char *SERVER_COMMAND_FIRE;
extern const char *SERVER_COMMAND_WEAP;

int server_acpt(uint8_t *buf, uint64_t player_id);
int server_join(uint8_t *buf, uint64_t player_id, char *player_name);
int server_kick(uint8_t *buf, char *reason);
int server_pong(uint8_t *buf, uint64_t data);
int server_quit(uint8_t *buf, uint64_t player_id);
int server_player_move(uint8_t *buf, uint64_t player_id, double x, double y, double z, double angle);
int server_player_chat(uint8_t *buf, uint64_t player_id, char *message);
int server_server_chat(uint8_t *buf, char *message, bool is_broadcast, bool is_title);
int server_scoreboard_update(uint8_t *buf, uint16_t entries_count, char **names, uint16_t *deaths, uint16_t *kills);
int server_player_damage(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id, float damage);
int server_player_heal(uint8_t *buf, uint64_t player_id, float gain);
int server_player_health(uint8_t *buf, uint64_t player_id, float health, float max_health);
int server_player_kill(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id);
int server_load_map(uint8_t *buf, char *map_name);
int server_door_open(uint8_t *buf, uint64_t door_id);
int server_door_close(uint8_t *buf, uint64_t door_id);
int server_door_states(uint8_t *buf, uint16_t doors_count, bool *doors_states);
int server_lift_ascend(uint8_t *buf, uint64_t lift_id);
int server_lift_descend(uint8_t *buf, uint64_t lift_id);
int server_lift_states(uint8_t *buf, uint16_t lifts_count, bool* lifts_states);
int server_game_start(uint8_t *buf, int16_t countdown);
int server_game_end(uint8_t *buf, int16_t countdown);
int server_player_fire(uint8_t *buf, uint64_t player_id, int8_t weapon_id);
int server_player_weapon_update(uint8_t *buf, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER]);

int server_acpt_from(uint8_t *buf, uint64_t *player_id);
int server_join_from(uint8_t *buf, uint64_t *player_id, char **player_name);
int server_kick_from(uint8_t *buf, char **reason);
int server_pong_from(uint8_t *buf, uint64_t *data);
int server_quit_from(uint8_t *buf, uint64_t *player_id);
int server_player_move_from(uint8_t *buf, uint64_t *player_id, double *x, double *y, double *z, double *angle);
int server_player_chat_from(uint8_t *buf, uint64_t *player_id, char **message);
int server_server_chat_from(uint8_t *buf, char **message, bool *is_broadcast, bool *is_title);
int server_scoreboard_update_from(uint8_t *buf, uint16_t *entries_count, char ***names, uint16_t **deaths, uint16_t **kills);
int server_player_damage_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t *weapon_id, float *damage);
int server_player_heal_from(uint8_t *buf, uint64_t *player_id, float *gain);
int server_player_health_from(uint8_t *buf, uint64_t *player_id, float *health, float *max_health);
int server_player_kill_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t *weapon_id);
int server_load_map_from(uint8_t *buf, char **map_name);
int server_door_open_from(uint8_t *buf, uint64_t *door_id);
int server_door_close_from(uint8_t *buf, uint64_t *door_id);
int server_door_states_from(uint8_t *buf, uint16_t *doors_count, bool **doors_states);
int server_lift_ascend_from(uint8_t *buf, uint64_t *lift_id);
int server_lift_descend_from(uint8_t *buf, uint64_t *lift_id);
int server_lift_states_from(uint8_t *buf, uint16_t *lifts_count, bool** lifts_states);
int server_game_start_from(uint8_t *buf, int16_t *countdown);
int server_game_end_from(uint8_t *buf, int16_t *countdown);
int server_player_fire_from(uint8_t *buf, uint64_t *player_id, int8_t *weapon_id);
int server_player_weapon_update_from(uint8_t *buf, int *ammunitions, int *mags, int *cooldowns);
#endif
#ifndef _LIB_DOOM_NET_PACKET_SERVER_H
#define _LIB_DOOM_NET_PACKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../component/weapon.h"

/** \file
 * This file contains the definitions for the server packets.  
 * All bytes send to the server should be defined in those functions.
 * 
 * A packet is always defined as follows:  
 * * First by a command of 4 chars, defined as a constant name `SERVER_COMMAND_CHAR`
 * * A into_network function that takes a buffer and the data to be sent, and returns the size of the buffer written.
 * * A from_network function that takes a buffer and the data to be received, and returns the size of the buffer read.
*/

/// @brief The command for the server accept packet, whose command is "ACPT".
extern const char *SERVER_COMMAND_ACPT;
/// @brief The command for the server join packet, whose command is "JOIN".
extern const char *SERVER_COMMAND_JOIN;
/// @brief The command for the server kick packet, whose command is "KICK".
extern const char *SERVER_COMMAND_KICK;
/// @brief The command for the server pong packet, whose command is "PONG".
extern const char *SERVER_COMMAND_PONG;
/// @brief The command for the server quit packet, whose command is "QUIT".
extern const char *SERVER_COMMAND_QUIT;
/// @brief The command for the server player move packet, whose command is "MOVE".
extern const char *SERVER_COMMAND_MOVE;
/// @brief The command for the server player chat packet, whose command is "CHAT".
extern const char *SERVER_COMMAND_CHAT;
/// @brief The command for the server server chat packet, whose command is "TELL".
extern const char *SERVER_COMMAND_TELL;
/// @brief The command for the server scoreboard update packet, whose command is "SCOR".
extern const char *SERVER_COMMAND_SCOR;
/// @brief The command for the server player damage packet, whose command is "DAMG".
extern const char *SERVER_COMMAND_DAMG;
/// @brief The command for the server player heal packet, whose command is "HEAL".
extern const char *SERVER_COMMAND_HEAL;
/// @brief The command for the server player health packet, whose command is "HLTH".
extern const char *SERVER_COMMAND_HLTH;
/// @brief The command for the server player kill packet, whose command is "KILL".
extern const char *SERVER_COMMAND_KILL;
/// @brief The command for the server load map packet, whose command is "LMAP".
extern const char *SERVER_COMMAND_LMAP;
/// @brief The command for the server door open packet, whose command is "OPEN".
extern const char *SERVER_COMMAND_OPEN;
/// @brief The command for the server door close packet, whose command is "CLOS".
extern const char *SERVER_COMMAND_CLOS;
/// @brief The command for the server door states packet, whose command is "DOST".
extern const char *SERVER_COMMAND_DOST;
/// @brief The command for the server lift ascend packet, whose command is "LASC".
extern const char *SERVER_COMMAND_LASC;
/// @brief The command for the server lift descend packet, whose command is "LDSC".
extern const char *SERVER_COMMAND_LDSC;
/// @brief The command for the server lift states packet, whose command is "LIST".
extern const char *SERVER_COMMAND_L_ST;
extern const char *SERVER_COMMAND_FIRE;
extern const char *SERVER_COMMAND_WEAP;

/// @brief server_acpt creates a new server accept packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id attributed to the player that is joining the server.
/// @return length of the packet.
int server_acpt(uint8_t *buf, uint64_t player_id);

/// @brief server_join creates a new server join packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id attributed to the player that is joining the server.
/// @param player_name the name of the player that is joining the server.
///                    the name should be at most 127 characters long and NUL-terminated.
/// @return length of the packet.
int server_join(uint8_t *buf, uint64_t player_id, char *player_name);

/// @brief server_kick creates a new server kick packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param reason the reason for the kick.
///               the reason should be at most 1023 characters long and NUL-terminated.
/// @return length of the packet.
int server_kick(uint8_t *buf, char *reason);

/// @brief server_pong creates a new server pong packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param data the 8-byte data to be sent to the client.
/// @return length of the packet.
int server_pong(uint8_t *buf, uint64_t data);

/// @brief server_quit creates a new server quit packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is quitting the server.
/// @return length of the packet.
int server_quit(uint8_t *buf, uint64_t player_id);

/// @brief server_player_move creates a new server player move packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is moving.
/// @param x the x position of the player.
/// @param y the y position of the player.
/// @param z the z position of the player.
/// @param angle the angle (in degrees) of the player.
/// @return length of the packet.
int server_player_move(uint8_t *buf, uint64_t player_id, double x, double y, double z, double angle);

/// @brief server_player_chat creates a new server player chat packet.
/// @remark The message should be at most 1023 characters long and NUL-terminated.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is chatting.
/// @param message the message to be sent to the server.
/// @return length of the packet.
int server_player_chat(uint8_t *buf, uint64_t player_id, char *message);

/// @brief server_server_chat creates a new server server chat packet.
/// @remark The message should be at most 1023 characters long and NUL-terminated.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param message the message to be sent to the server.
/// @param is_broadcast whether the message is a broadcast message.
/// @param is_title whether the message is a title message.
/// @return length of the packet.
int server_server_chat(uint8_t *buf, char *message, bool is_broadcast, bool is_title);

/// @brief server_scoreboard_update creates a new server scoreboard update packet.
///
/// @remark The number of entries should be at most 10.
/// @remark The names should be at most 127 characters long and NUL-terminated.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param entries_count the number of entries in the scoreboard.
/// @param names the names of the players in the scoreboard.
/// @param deaths the number of deaths of the players in the scoreboard.
/// @param kills the number of kills of the players in the scoreboard.
/// @return length of the packet.
int server_scoreboard_update(uint8_t *buf, uint16_t entries_count, char **names, uint16_t *deaths, uint16_t *kills);

/// @brief server_player_damage creates a new server player damage packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is taking damage.
/// @param src_player_id the player_id of the player that is dealing damage.
/// @param weapon_id the id of the weapon used.
/// @param damage the amount of damage taken.
/// @return length of the packet.
int server_player_damage(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id, float damage);

/// @brief server_player_heal creates a new server player heal packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is healing.
/// @param gain the amount of health gained.
/// @return length of the packet.
int server_player_heal(uint8_t *buf, uint64_t player_id, float gain);

/// @brief server_player_health creates a new server player health packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is having its health updated.
/// @param health the current health of the player.
/// @param max_health the maximum health of the player.
/// @return length of the packet.
int server_player_health(uint8_t *buf, uint64_t player_id, float health, float max_health);

/// @brief server_player_kill creates a new server player kill packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_id the player_id of the player that is being killed.
/// @param src_player_id the player_id of the player that is killing.
/// @param weapon_id the id of the weapon used.
/// @return length of the packet.
int server_player_kill(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id);

/// @brief server_load_map creates a new server load map packet.
/// @remark The map_name should be at most 127 characters long and NUL-terminated.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param map_name the name of the map to be loaded.
/// @return length of the packet.
int server_load_map(uint8_t *buf, char *map_name);

/// @brief server_door_open creates a new server door open packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the door to be opened.
/// @return length of the packet.
int server_door_open(uint8_t *buf, uint64_t door_id);

/// @brief server_door_close creates a new server door close packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the door to be closed.
/// @return length of the packet.
int server_door_close(uint8_t *buf, uint64_t door_id);

/// @brief server_door_states creates a new server door states packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param doors_count the number of doors in the map.
/// @param doors_states the states of the doors in the map.
/// @return length of the packet.
int server_door_states(uint8_t *buf, uint16_t doors_count, bool *doors_states);

/// @brief server_lift_ascend creates a new server lift ascend packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param lift_id the id of the lift to be ascended.
/// @return length of the packet.
int server_lift_ascend(uint8_t *buf, uint64_t lift_id);

/// @brief server_lift_descend creates a new server lift descend packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param lift_id the id of the lift to be descended.
/// @return length of the packet.
int server_lift_descend(uint8_t *buf, uint64_t lift_id);

/// @brief server_lift_states creates a new server lift states packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param lifts_count the number of lifts in the map.
/// @param lifts_states the states of the lifts in the map.
/// @return length of the packet.
int server_lift_states(uint8_t *buf, uint16_t lifts_count, bool *lifts_states);
int server_player_fire(uint8_t *buf, uint64_t player_id, int8_t weapon_id);
int server_player_weapon_update(uint8_t *buf, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER]);

/// @brief server_acpt_from reads a server accept packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id the buffer where the player_id will be written.
/// @return the length of the packet read.
int server_acpt_from(uint8_t *buf, uint64_t *player_id);

/// @brief server_join_from reads a server join packet.
/// @remark `player_name` should be a valid char buffer of at least 128 chars.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id the buffer where the player_id will be written.
/// @param player_name the buffer where the player_name will be written.
/// @return the length of the packet read.
int server_join_from(uint8_t *buf, uint64_t *player_id, char **player_name);

/// @brief server_kick_from reads a server kick packet.
/// @remark `reason` should be a valid char buffer of at least 1024 chars.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param reason the buffer where the reason will be written.
/// @return the length of the packet read.
int server_kick_from(uint8_t *buf, char **reason);

/// @brief server_pong_from reads a server pong packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param data the buffer where the data will be written.
/// @return the length of the packet read.
int server_pong_from(uint8_t *buf, uint64_t *data);

/// @brief server_quit_from reads a server quit packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is quitting the server.
/// @return the length of the packet read.
int server_quit_from(uint8_t *buf, uint64_t *player_id);

/// @brief server_player_move_from reads a server player move packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is moving.
/// @param x x buffer of the player that is moving.
/// @param y y buffer of the player that is moving.
/// @param z z buffer of the player that is moving.
/// @param angle angle buffer of the player that is moving.
/// @return the length of the packet read.
int server_player_move_from(uint8_t *buf, uint64_t *player_id, double *x, double *y, double *z, double *angle);

/// @brief server_player_chat_from reads a server player chat packet.
/// @remark `message` should be a valid char buffer of at least 1024 chars.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is chatting.
/// @param message the buffer where the message will be written.
/// @return the length of the packet read.
int server_player_chat_from(uint8_t *buf, uint64_t *player_id, char **message);

/// @brief server_server_chat_from reads a server server chat packet.
/// @remark `message` should be a valid char buffer of at least 1024 chars.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param message the buffer where the message will be written.
/// @param is_broadcast is this message a broadcast message?
/// @param is_title is this message a title message?
/// @return the length of the packet read.
int server_server_chat_from(uint8_t *buf, char **message, bool *is_broadcast, bool *is_title);

/// @brief server_scoreboard_update_from reads a server scoreboard update packet.
/// @remark `names`, `deaths` and `kills` should be valid buffers of at least 10 elements.
/// @remark `names` items must be freed by the caller.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param entries_count the buffer where the number of entries will be written.
/// @param names the buffer where the names will be written.
/// @param deaths the buffer where the deaths will be written.
/// @param kills the buffer where the kills will be written.
/// @return the length of the packet read.
int server_scoreboard_update_from(uint8_t *buf, uint16_t *entries_count, char ***names, uint16_t **deaths, uint16_t **kills);

/// @brief server_player_damage_from reads a server player damage packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is taking damage.
/// @param src_player_id src_player_id buffer of the player that is dealing damage.
/// @param damage damage buffer of the player that is taking damage.
/// @param weapon_id the id buffer of the weapon used.
/// @return the length of the packet read.
int server_player_damage_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t *weapon_id, float *damage);

/// @brief server_player_heal_from reads a server player heal packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is healing.
/// @param gain gain buffer of the player that is healing.
/// @return the length of the packet read.
int server_player_heal_from(uint8_t *buf, uint64_t *player_id, float *gain);

/// @brief server_player_health_from reads a server player health packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is having its health updated.
/// @param health health buffer of the player that is having its health updated.
/// @param max_health max_health buffer of the player that is having its health updated.
/// @return the length of the packet read.
int server_player_health_from(uint8_t *buf, uint64_t *player_id, float *health, float *max_health);

/// @brief server_player_kill_from reads a server player kill packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_id player_id buffer of the player that is being killed.
/// @param src_player_id src_player_id buffer of the player that is killing.
/// @param weapon_id the id buffer of the weapon used.

/// @return the length of the packet read.
int server_player_kill_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t *weapon_id);

/// @brief server_load_map_from reads a server load map packet.
/// @remark `map_name` should be a valid char buffer of at least 128 chars.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param map_name the buffer where the map_name will be written.
/// @return the length of the packet read.
int server_load_map_from(uint8_t *buf, char **map_name);

/// @brief server_door_open_from reads a server door open packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the buffer where the door_id will be written.
/// @return the length of the packet read.
int server_door_open_from(uint8_t *buf, uint64_t *door_id);

/// @brief server_door_close_from reads a server door close packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the buffer where the door_id will be written.
/// @return the length of the packet read.
int server_door_close_from(uint8_t *buf, uint64_t *door_id);

/// @brief server_door_states_from reads a server door states packet
/// @remark `doors_states` should be freed by the caller.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param doors_count the buffer where the number of doors will be written.
/// @param doors_states the buffer where the doors_states will be written.
/// @return the length of the packet read.
int server_door_states_from(uint8_t *buf, uint16_t *doors_count, bool **doors_states);

/// @brief server_lift_ascend_from reads a server lift ascend packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param lift_id the buffer where the lift_id will be written.
/// @return the length of the packet read.
int server_lift_ascend_from(uint8_t *buf, uint64_t *lift_id);

/// @brief server_lift_descend_from reads a server lift descend packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param lift_id the buffer where the lift_id will be written.
/// @return the length of the packet read.
int server_lift_descend_from(uint8_t *buf, uint64_t *lift_id);

/// @brief server_lift_states_from reads a server lift states packet
/// @remark `lifts_states` should be freed by the caller.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param lifts_count the buffer where the number of lifts will be written.
/// @param lifts_states the buffer where the lifts_states will be written.
/// @return the length of the packet read.
int server_lift_states_from(uint8_t *buf, uint16_t *lifts_count, bool **lifts_states);
int server_player_fire_from(uint8_t *buf, uint64_t *player_id, int8_t *weapon_id);
int server_player_weapon_update_from(uint8_t *buf, int *ammunitions, int *mags, int *cooldowns);
#endif
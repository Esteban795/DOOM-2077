#ifndef _LIB_DOOM_NET_PACKET_CLIENT_H
#define _LIB_DOOM_NET_PACKET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/** \file
 * This file contains the definitions for the client packets.  
 * All bytes send to the server should be defined in those functions.
 * 
 * A packet is always defined as follows:  
 * * First by a command of 4 chars, defined as a constant name `CLIENT_COMMAND_CHAR`
 * * A into_network function that takes a buffer and the data to be sent, and returns the size of the buffer written.
 * * A from_network function that takes a buffer and the data to be received, and returns the size of the buffer read.
*/

/// @brief The command for the client join packet, whose command is "JOIN".
extern const char* CLIENT_COMMAND_JOIN;
/// @brief The command for the client keep alive packet, whose command is "KATC".
extern const char* CLIENT_COMMAND_KATC;
/// @brief The command for the client ping packet, whose command is "PING".
extern const char* CLIENT_COMMAND_PING;
/// @brief The command for the client quit packet, whose command is "QUIT".
extern const char* CLIENT_COMMAND_QUIT;
/// @brief The command for the client player move packet, whose command is "MOVE".
extern const char* CLIENT_COMMAND_MOVE;
/// @brief The command for the client player chat packet, whose command is "CHAT".
extern const char* CLIENT_COMMAND_CHAT;
/// @brief The command for the client door open packet, whose command is "OPEN".
extern const char* CLIENT_COMMAND_OPEN;
/// @brief The command for the client door close packet, whose command is "CLOS".
extern const char* CLIENT_COMMAND_CLOS;
/// @brief The command for the client lift ascend packet, whose command is "LASC".
extern const char* CLIENT_COMMAND_LASC;
/// @brief The command for the client lift descend packet, whose command is "LDSC".
extern const char* CLIENT_COMMAND_LDSC;
extern const char* CLIENT_COMMAND_FIRE;
extern const char* CLIENT_COMMAND_DAMG;

/// @brief client_join creates a new client join packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param player_name the name of the player that is joining the server.
/// @return length of the packet.
int client_join(uint8_t* buf, char* player_name);

/// @brief client_keep_alive creates a new client keep alive packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @return length of the packet.
int client_keep_alive(uint8_t* buf);

/// @brief client_ping creates a new client ping packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param data the 8-byte data to be sent to the server.
/// @return length of the packet.
int client_ping(uint8_t* buf, uint64_t data);

/// @brief client_quit creates a new client quit packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @return length of the packet.
int client_quit(uint8_t* buf);

/// @brief client_move creates a new client move packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param x the x position of the player.
/// @param y the y position of the player.
/// @param z the z position of the player.
/// @param angle the angle (in degrees) of the player.
/// @return length of the packet.
int client_move(uint8_t* buf, double x, double y, double z, double angle);

/// @brief client_chat creates a new client chat packet.
/// @remark The message should be at most 1023 characters long and NUL-terminated.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param message the message to be sent to the server.
/// @return length of the packet.
int client_chat(uint8_t* buf, char* message);

/// @brief client_door_open creates a new client door open packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the door to be opened.
/// @return length of the packet.
int client_door_open(uint8_t* buf, uint64_t door_id);

/// @brief client_door_close creates a new client door close packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the door to be closed.
/// @return length of the packet.
int client_door_close(uint8_t* buf, uint64_t door_id);

/// @brief client_lift_ascend creates a new client lift ascend packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the lift to be ascended.
/// @return length of the packet.
int client_lift_ascend(uint8_t* buf, uint64_t door_id);

/// @brief client_lift_descend creates a new client lift descend packet.
/// @param buf the outgoing packet buffer, where the packet will be written.
/// @param door_id the id of the lift to be descended.
/// @return length of the packet.
int client_lift_descend(uint8_t* buf, uint64_t door_id);
int client_fire(uint8_t* buf, int8_t weapon_id);
int client_damage(uint8_t* buf, uint64_t player_id, int8_t weapon_id, float damage);

/// @brief client_join_from reads a client join packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param player_name the name of the player that is joining the server.
///                    This must point to a valid char buffer of at least 128 chars.
/// @return length of the packet.
int client_join_from(uint8_t* buf, char** player_name);

/// @brief client_ping_from reads a client ping packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param data the 8-byte data received from the server.
/// @return length of the packet.
int client_ping_from(uint8_t* buf, uint64_t* data);

/// @brief client_move_from reads a client move packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param x the x position of the player.
/// @param y the y position of the player.
/// @param z the z position of the player.
/// @param angle the angle (in degrees) of the player.
/// @return length of the packet.
int client_move_from(uint8_t* buf, double* x, double* y, double* z, double* angle);

/// @brief client_chat_from reads a client chat packet.
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param message the message received from the server.
///                This must point to a valid char buffer of at least 1024 chars.
/// @return length of the packet.
int client_chat_from(uint8_t* buf, char** message);

/// @brief client_door_open_from reads a client door open packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the id of the door that is being opened.
/// @return length of the packet.
int client_door_open_from(uint8_t* buf, uint64_t* door_id);

/// @brief client_door_close_from reads a client door close packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the id of the door that is being closed.
/// @return length of the packet.
int client_door_close_from(uint8_t* buf, uint64_t* door_id);

/// @brief client_lift_ascend_from reads a client lift ascend packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the id of the lift that is being ascended.
/// @return length of the packet.
int client_lift_ascend_from(uint8_t* buf, uint64_t* door_id);

/// @brief client_lift_descend_from reads a client lift descend packet
/// @param buf the incoming packet buffer, where the packet will be read.
/// @param door_id the id of the lift that is being descended.
/// @return length of the packet.
int client_lift_descend_from(uint8_t* buf, uint64_t* door_id);
int client_fire_from(uint8_t* buf, int8_t* weapon_id);
int client_damage_from(uint8_t* buf, uint64_t* player_id, int8_t* weapon_id, float* damage);
#endif
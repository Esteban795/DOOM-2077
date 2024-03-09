#define _LIB_DOOM_NET_PACKET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

extern const char* CLIENT_COMMAND_JOIN;
extern const char* CLIENT_COMMAND_KATC;
extern const char* CLIENT_COMMAND_PONG;
extern const char* CLIENT_COMMAND_QUIT;

int client_join(uint8_t* buf, char* player_name);
int client_keep_alive(uint8_t* buf);
int client_ping(uint8_t* buf, uint64_t data);
int client_quit(uint8_t* buf);
// TODO: client_player_list
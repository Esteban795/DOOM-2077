#define _LIB_DOOM_NET_PACKET_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

int server_acpt(uint8_t* buf, uint64_t player_id);
int server_join(uint8_t* buf, uint64_t player_id, char* player_name);
int server_kick(uint8_t* buf, char* reason);
int server_pong(uint8_t* buf, uint64_t data);
int server_quit(uint8_t* buf, uint64_t player_id);
// TODO: server_player_list

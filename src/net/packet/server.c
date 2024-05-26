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
const char* SERVER_COMMAND_MOVE = "MOVE";
const char *SERVER_COMMAND_DAMG = "DAMG";
const char *SERVER_COMMAND_HEAL = "HEAL";
const char *SERVER_COMMAND_HLTH = "HLTH";
const char *SERVER_COMMAND_KILL = "KILL";
const char *SERVER_COMMAND_LMAP = "LMAP";
const char *SERVER_COMMAND_OPEN = "OPEN";
const char *SERVER_COMMAND_CLOS = "CLOS";
const char *SERVER_COMMAND_DOST = "DOST";
const char *SERVER_COMMAND_LASC = "LASC";
const char *SERVER_COMMAND_LDSC = "LDSC";
const char *SERVER_COMMAND_L_ST = "L_ST";
const char *SERVER_COMMAND_FIRE = "FIRE";
const char *SERVER_COMMAND_WEAP = "WEAP";

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
        plen += write_cstring(buf + 6 + plen, names[i]);
    }
    for (int i = 0; i < entries_count; i++) {
        write_uint16be(buf + 6 + plen, deaths[i]);
        plen += 2;
    }
    for (int i = 0; i < entries_count; i++) {
        write_uint16be(buf + 6 + plen, kills[i]);
        plen += 2;
    }
    write_uint16be(buf + 4, plen);
    buf[6 + plen] = '\n';
    return 4 + 2 + plen + 1;
}

int server_player_damage(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id, float damage) {
    memcpy(buf, SERVER_COMMAND_DAMG, 4);
    write_uint16be(buf + 4, 8 + 8 + 4);
    write_uint64be(buf + 6, player_id);
    write_uint64be(buf + 14, src_player_id);
    write_uint8be(buf + 22, weapon_id);
    write_uint32be(buf + 23, damage*1000);
    buf[27] = '\n';
    return 4 + 2 + 8 + 8 + 1 + 4 + 1;
}

int server_player_heal(uint8_t *buf, uint64_t player_id, float gain) {
    memcpy(buf, SERVER_COMMAND_HEAL, 4);
    write_uint16be(buf + 4, 8 + 4);
    write_uint64be(buf + 6, player_id);
    write_uint32be(buf + 14, gain*1000);
    buf[18] = '\n';
    return 4 + 2 + 8 + 4 + 1;
}

int server_player_health(uint8_t *buf, uint64_t player_id, float health, float max_health) {
    memcpy(buf, SERVER_COMMAND_HLTH, 4);
    write_uint16be(buf + 4, 8 + 4 + 4);
    write_uint64be(buf + 6, player_id);
    write_uint32be(buf + 14, health*1000);
    write_uint32be(buf + 18, max_health*1000);
    buf[22] = '\n';
    return 4 + 2 + 8 + 4 + 4 + 1;
}

int server_player_kill(uint8_t *buf, uint64_t player_id, uint64_t src_player_id, int8_t weapon_id) {
    memcpy(buf, SERVER_COMMAND_KILL, 4);
    write_uint64be(buf + 6, player_id);
    write_uint64be(buf + 14, src_player_id);
    write_uint8be(buf + 22, weapon_id);
    buf[23] = '\n';
    write_uint16be(buf + 4, 8 + 8 + 1);
    return 4 + 2 + 8 + 8 + 1 + 1;
}

int server_load_map(uint8_t* buf, char* map_name) {
    if (strnlen(map_name, 128) >= 128) {
        printf("ERROR: server_load_map > Map name is too long.\n");
        return 0;
    }
    memcpy(buf, SERVER_COMMAND_LMAP, 4);
    int clen = write_cstring(buf + 6, map_name);
    write_uint16be(buf + 4, clen);
    buf[6 + clen] = '\n';
    return 4 + 2 + clen + 1;
}

inline int server_door_state_change(uint8_t* buf, const char* command, uint64_t door_id) {
    memcpy(buf, command, 4);
    write_uint16be(buf + 4, 8);
    write_uint64be(buf + 6, door_id);
    buf[14] = '\n';
    return 4 + 2 + 8 + 1;
}

int server_door_open(uint8_t *buf, uint64_t door_id) {
    return server_door_state_change(buf, SERVER_COMMAND_OPEN, door_id);
}

int server_door_close(uint8_t *buf, uint64_t door_id) {
    return server_door_state_change(buf, SERVER_COMMAND_CLOS, door_id);
}

int server_door_states(uint8_t *buf, uint16_t doors_count, bool* doors_states) {
    memcpy(buf, SERVER_COMMAND_DOST, 4);
    write_uint16be(buf + 4, doors_count);
    for (int i = 0; i < doors_count; i++) {
        buf[6 + i] = doors_states[i] ? 1 : 0;
    }
    buf[6 + doors_count] = '\n';
    return 4 + 2 + doors_count + 1;
}

int server_lift_ascend(uint8_t *buf, uint64_t lift_id) {
    return server_door_state_change(buf, SERVER_COMMAND_LASC, lift_id);
}

int server_lift_descend(uint8_t *buf, uint64_t lift_id) {
    return server_door_state_change(buf, SERVER_COMMAND_LDSC, lift_id);
}

int server_lift_states(uint8_t *buf, uint16_t lifts_count, bool* lifts_states) {
    memcpy(buf, SERVER_COMMAND_L_ST, 4);
    write_uint16be(buf + 4, lifts_count);
    for (int i = 0; i < lifts_count; i++) {
        buf[6 + i] = lifts_states[i] ? 1 : 0;
    }
    buf[6 + lifts_count] = '\n';
    return 4 + 2 + lifts_count + 1;
}

int server_player_fire(uint8_t *buf, uint64_t player_id, int8_t weapon_id) {
    memcpy(buf, SERVER_COMMAND_FIRE, 4);
    write_uint16be(buf + 4, 8 + 1);
    write_uint64be(buf + 6, player_id);
    write_uint8be(buf + 14, weapon_id);
    buf[15] = '\n';
    return 4 + 2 + 8 + 1 + 1;
}

int server_player_weapon_update(uint8_t *buf, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER]) {
    memcpy(buf, SERVER_COMMAND_WEAP, 4);
    write_uint16be(buf + 4, 4*WEAPONS_NUMBER*3);
    for (int i = 0; i < WEAPONS_NUMBER; i++) {
        write_uint32be(buf + 6 + (i*3*4), ammunitions[i]);
        write_uint32be(buf + 6 + (i*3*4) + 4, mags[i]);
        write_uint32be(buf + 6 + (i*3*4) + 8, cooldowns[i]);
    }
    buf[6 + (WEAPONS_NUMBER*3*4)] = '\n';
    return 4 + 2 + 4*WEAPONS_NUMBER*3 + 1;
}

int server_acpt_from(uint8_t* buf, uint64_t* player_id) {
    *player_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_join_from(uint8_t* buf, uint64_t* player_id, char** player_name) {
    *player_id = read_uint64be(buf + 6);
    int clen = read_uint16be(buf + 4) - 8;
    int clen_ = clen;
    if (clen >= 128) {
        clen_ = 128;
    }
    memcpy(*player_name, buf + 14, clen_);
    (*player_name)[clen_-1] = '\0';
    return 4 + 2 + 8 + clen + 1;

}

int server_kick_from(uint8_t* buf, char** reason) {
    int clen = read_uint16be(buf + 4);
    int clen_ = clen;
    if (clen >= 1024) {
        clen_ = 1024;
    }
    memcpy(*reason, buf + 6, clen_);
    (*reason)[clen_-1] = '\0';
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
    int clen_ = clen;
    if (clen >= 1024) {
        clen_ = 1024;
    }
    memcpy(*message, buf + 14, clen_);
    (*message)[clen_-1] = '\0';
    return 4 + 2 + 8 + clen + 1;
}

int server_server_chat_from(uint8_t* buf, char** message, bool* is_broadcast, bool* is_title) {
    int clen = read_uint16be(buf + 4) - 1;
    int clen_ = clen;
    if (clen >= 1024) {
        clen_ = 1024;
    }
    *is_broadcast = (buf[6] & 0x01) != 0;
    *is_title = (buf[6] & 0x02) != 0;
    memcpy(*message, buf + 7, clen_);
    (*message)[clen_-1] = '\0';
    return 4 + 2 + 1 + clen + 1;
}

int server_scoreboard_update_from(uint8_t* buf, uint16_t* entries_count, char*** names, uint16_t** deaths, uint16_t** kills) {
    *entries_count = read_uint16be(buf + 6);
    int plen = 2;
    *names = malloc(*entries_count * sizeof(char*));
    for (int i = 0; i < *entries_count; i++) {
        int clen = strnlen((char*) buf + 6 + plen, 127);
        (*names)[i] = malloc((clen+1) * sizeof(char));
        memcpy((*names)[i], buf + 6 + plen, clen);
        (*names)[i][clen] = '\0';
        plen += clen + 1;
    }
    *deaths = malloc(*entries_count * sizeof(uint16_t));
    for (int i = 0; i < *entries_count; i++) {
        (*deaths)[i] = read_uint16be(buf + 6 + plen);
        plen += 2;
    }
    *kills = malloc(*entries_count * sizeof(uint16_t));
    for (int i = 0; i < *entries_count; i++) {
        (*kills)[i] = read_uint16be(buf + 6 + plen);
        plen += 2;
    }
    return 4 + 2 + plen + 1;
}

int server_player_damage_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t* weapon_id, float *damage) {
    *player_id = read_uint64be(buf + 6);
    *src_player_id = read_uint64be(buf + 14);
    *weapon_id = read_int8be(buf + 22);
    *damage = ((float) read_int32be(buf + 23)) / 1000.0;
    return 4 + 2 + 8 + 8 + 1 + 4 + 1;
}

int server_player_heal_from(uint8_t *buf, uint64_t *player_id, float *gain) {
    *player_id = read_uint64be(buf + 6);
    *gain = ((float) read_int32be(buf + 14)) / 1000.0;
    return 4 + 2 + 8 + 4 + 1;
}

int server_player_health_from(uint8_t *buf, uint64_t *player_id, float *health, float *max_health) {
    *player_id = read_uint64be(buf + 6);
    *health = ((float) read_int32be(buf + 14)) / 1000.0;
    *max_health = ((float) read_int32be(buf + 18)) / 1000.0;
    return 4 + 2 + 8 + 4 + 4 + 1;
}

int server_player_kill_from(uint8_t *buf, uint64_t *player_id, uint64_t *src_player_id, int8_t* weapon_id) {
    *player_id = read_uint64be(buf + 6);
    *src_player_id = read_uint64be(buf + 14);
    *weapon_id = read_int8be(buf + 22);
    return 4 + 2 + 8 + 8 + 1 + 1;
}

int server_load_map_from(uint8_t* buf, char** map_name) {
    int clen = read_uint16be(buf + 4);
    int clen_ = clen;
    if (clen >= 128) {
        clen_ = 128;
    }
    memcpy(*map_name, buf + 6, clen_-1);
    (*map_name)[clen_-1] = '\0';
    return 4 + 2 + clen + 1;
}

int server_door_open_from(uint8_t *buf, uint64_t *door_id) {
    *door_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_door_close_from(uint8_t *buf, uint64_t *door_id) {
    *door_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_door_states_from(uint8_t *buf, uint16_t *doors_count, bool** doors_states) {
    *doors_count = read_uint16be(buf + 4);
    *doors_states = malloc(*doors_count * sizeof(bool));
    for (int i = 0; i < *doors_count; i++) {
        (*doors_states)[i] = buf[6 + i] != 0;
    }
    return 4 + 2 + *doors_count + 1;
}

int server_lift_ascend_from(uint8_t *buf, uint64_t *lift_id) {
    *lift_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_lift_descend_from(uint8_t *buf, uint64_t *lift_id) {
    *lift_id = read_uint64be(buf + 6);
    return 4 + 2 + 8 + 1;
}

int server_lift_states_from(uint8_t *buf, uint16_t *lifts_count, bool** lifts_states) {
    *lifts_count = read_uint16be(buf + 4);
    *lifts_states = malloc(*lifts_count * sizeof(bool));
    for (int i = 0; i < *lifts_count; i++) {
        (*lifts_states)[i] = buf[6 + i] != 0;
    }
    return 4 + 2 + *lifts_count + 1;
}

int server_player_fire_from(uint8_t *buf, uint64_t *player_id, int8_t *weapon_id) {
    *player_id = read_uint64be(buf + 6);
    *weapon_id = read_uint8be(buf + 14);
    return 4 + 2 + 8 + 1 + 1;
}

int server_player_weapon_update_from(uint8_t *buf, int *ammunitions, int *mags, int *cooldowns) {
    for (int i = 0; i < WEAPONS_NUMBER; i++) {
        ammunitions[i] = read_int32be(buf + 6 + (i*3*4));
        mags[i] = read_int32be(buf + 6 + (i*3*4) + 4);
        cooldowns[i] = read_int32be(buf + 6 + (i*3*4) + 8);
    }
    return 4 + 2 + 4*WEAPONS_NUMBER*3 + 1;
}
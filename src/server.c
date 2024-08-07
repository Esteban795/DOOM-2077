#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif


#include "../include/server/state.h"
#include "../include/server/player.h"
#include "../include/net/util.h"
#include "../include/net/tracked_connection.h"
#include "../include/net/packet/server.h"
#include "../include/net/packet/client.h"
#include "../include/ecs/world.h"
#include "../include/event/all.h"
#include "../include/system/server/active.h"
#include "../include/WAD/wad_data.h"
#include "../include/server/door.h"

// Interrupt signal handler
//
// Handle in particular SIGINT (Ctrl+C), SIGQUIT, and SIGTERM, for a graceful shutdown.
void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        SERVER_RUNNING = -1;
        break;
    case SIGQUIT:
    case SIGTERM:
        SERVER_RUNNING = -2;
        break;
    default:
        printf("Caught signal %d.\n", sig);
        break;
    }
}

int run_server(uint16_t port)
{
    printf("Initializing SDL_net...\n");
    SDLNet_Init();
    char addrstr[24];

    // Create socket
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return -1;
    }
    UDPsocket server = SDLNet_UDP_Open(port);
    if (server == NULL)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return -1;
    }

    addrtocstr(&ip, addrstr);
    printf("Listening on %s...\n", addrstr);

    // Initialize server state
    assert(SERVER_STATE != NULL);
    SERVER_STATE->sock = server;
    SERVER_STATE->conn_count = 0;
    SERVER_RUNNING = 1;
    SERVER_STATE->game_state = GAME_STATE_WAITING;
    INSTANT_NOW(&SERVER_STATE->last_tick);
    world_init(&SERVER_STATE->world);
    task_executor_init(&SERVER_STATE->task_executor);

    // Register the systems
    world_register_active_systems(&SERVER_STATE->world);

    // Load the WAD data and map
    SERVER_STATE->map_name = "MAP01";
    SERVER_STATE->wad_data = server_load_wad("maps/DOOM1-NET.WAD", SERVER_STATE->map_name);
    if (SERVER_STATE->wad_data == NULL)
    {
        printf("Failed to load WAD data.\n");
        return -1;
    }
    SERVER_STATE->doors = server_world_load_doors(&SERVER_STATE->world, SERVER_STATE->wad_data, &SERVER_STATE->door_count);
    SERVER_STATE->lifts = server_world_load_lifts(&SERVER_STATE->world, SERVER_STATE->wad_data, &SERVER_STATE->lift_count);

    // Listen for incoming packets
    SERVER_STATE->incoming = SDLNet_AllocPacket(4096);
    SERVER_STATE->outgoing = SDLNet_AllocPacket(4096);
    assert(SERVER_STATE->incoming != NULL);
    assert(SERVER_STATE->outgoing != NULL);

    Instant cur_time;
    while (SERVER_RUNNING > 0)
    {
        INSTANT_NOW(&cur_time);
        int elapsed = INSTANT_DIFF_MS(cur_time, SERVER_STATE->last_tick);

        uint64_t entity_bin[32] = {0};
        int entity_bin_count = 0;
        // While there's time left in this tick, process incoming packets.
        while (elapsed < SERVER_TICK_MS)
        {
            // Process an incoming packet
            int ready = SDLNet_UDP_Recv(server, SERVER_STATE->incoming);
            if (ready == 1)
            {
                addrtocstr(&SERVER_STATE->incoming->address, addrstr);
                uint8_t sdata[2048] = {0};
                memcpy(sdata, SERVER_STATE->incoming->data, SERVER_STATE->incoming->len);
                //printf("incoming packet from %s > %s\n", addrstr, sdata);

                int cursor = 0;
                char cmd[5] = {0};
                int16_t payload_len = 0;
                // For each message in the packet
                while (cursor + 6 <= SERVER_STATE->incoming->len)
                {
                    memcpy(cmd, (char*) (sdata + cursor), 4);
                    payload_len = read_uint16be(sdata + cursor + 4);
                    if (cursor + 6 + payload_len + 1 > SERVER_STATE->incoming->len)
                    {
                        printf("Invalid packet: message length exceeds packet length.\n");
                        break;
                    }

                    int conn_i = find_conn_by_ip(SERVER_STATE->conns, SERVER_STATE->conn_count, &SERVER_STATE->incoming->address);

                    if (strncmp(cmd, CLIENT_COMMAND_PING, 4) == 0)
                    {
                        uint64_t data;
                        client_ping_from(sdata + cursor, &data);
                        int len = server_pong(SERVER_STATE->outgoing->data, data);
                        SERVER_STATE->outgoing->address = SERVER_STATE->incoming->address;
                        SERVER_STATE->outgoing->len = len;
                        SDLNet_UDP_Send(server, -1, SERVER_STATE->outgoing);
                    }
                    else if (strncmp(cmd, CLIENT_COMMAND_MOVE, 4) == 0)
                    {
                        double x, y, z, angle;
                        client_move_from(sdata + cursor, &x, &y, &z, &angle);
                        player_move_event_t* ev = ServerPlayerMoveEvent_new(SERVER_STATE->conns[conn_i].player_id, x, y, z, angle);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_CHAT, 4) == 0) {
                        char message_[128] = {0};
                        char* message = (char*) message_;
                        client_chat_from(sdata + cursor, &message);
                        player_chat_event_t* ev = ServerPlayerChatEvent_new(SERVER_STATE->conns[conn_i].player_id, message);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_JOIN, 4) == 0)  {
                        char name_[128] = {0};
                        char* name = (char*) name_;
                        client_join_from(sdata + cursor, &name);
                        if (SERVER_STATE->conn_count < MAX_CLIENTS) {
                            SERVER_STATE->conns[SERVER_STATE->conn_count].ip = SERVER_STATE->incoming->address;
                            SERVER_STATE->conns[SERVER_STATE->conn_count].player_id = server_create_player(&SERVER_STATE->world, name)->id;
                            server_player_join_event_t* ev = ServerPlayerJoinEvent_new(SERVER_STATE->conns[SERVER_STATE->conn_count].player_id, name);
                            world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                            SERVER_STATE->conn_count++;

                            // Send the ACPT message
                            int len = server_acpt(SERVER_STATE->outgoing->data, SERVER_STATE->conns[SERVER_STATE->conn_count - 1].player_id);
                            len += server_load_map(SERVER_STATE->outgoing->data + len, SERVER_STATE->map_name);
                            SERVER_STATE->outgoing->address = SERVER_STATE->incoming->address;
                            SERVER_STATE->outgoing->len = len;
                            if (SDLNet_UDP_Send(server, -1, SERVER_STATE->outgoing) > 0)
                            {
                                printf("Accepted %s as %lu.\n", name, SERVER_STATE->conns[SERVER_STATE->conn_count - 1].player_id);
                            }
                        } else {
                            printf("%s tried to join but the server is full!\n", name);
                        }
                    } else if (strncmp(cmd, CLIENT_COMMAND_QUIT, 4) == 0) {
                        entity_t pid = ENTITY_BY_ID(SERVER_STATE->conns[conn_i].player_id);
                        display_name_ct* display_name = (display_name_ct*) world_get_component(&SERVER_STATE->world, &pid, COMPONENT_TAG_DISPLAY_NAME);
                        server_player_quit_event_t* ev = ServerPlayerQuitEvent_new(SERVER_STATE->conns[conn_i].player_id, display_name_get(display_name));
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);         

                        // Add the player to the entity bin
                        if (entity_bin_count < 32) {
                            entity_bin[entity_bin_count] = SERVER_STATE->conns[conn_i].player_id;           
                            entity_bin_count++;
                        } else {
                            printf("Entity bin is full! Leaking the entity...\n");
                        }
                        
                        // Remove the connection
                        SERVER_STATE->conns[conn_i] = SERVER_STATE->conns[SERVER_STATE->conn_count - 1];
                        SERVER_STATE->conn_count--;
                    } else if (strncmp(cmd, CLIENT_COMMAND_OPEN, 4) == 0) {
                        uint64_t door_id;
                        client_door_open_from(sdata + cursor, &door_id);
                        door_open_event_t* ev = ServerDoorOpenEvent_new(door_id, false);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_CLOS, 4) == 0) {
                        uint64_t door_id;
                        client_door_close_from(sdata + cursor, &door_id);
                        door_close_event_t* ev = ServerDoorCloseEvent_new(door_id, false);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_LASC, 4) == 0) {
                        uint64_t door_id;
                        client_lift_ascend_from(sdata + cursor, &door_id);
                        door_open_event_t* ev = ServerDoorOpenEvent_new(door_id, true);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_LDSC, 4) == 0) {
                        uint64_t door_id;
                        client_lift_descend_from(sdata + cursor, &door_id);
                        door_close_event_t* ev = ServerDoorCloseEvent_new(door_id, true);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_DAMG, 4) == 0) {
                        uint64_t target_id;
                        int8_t weapon_id;
                        float damage;
                        client_damage_from(sdata + cursor, &target_id, &weapon_id, &damage);
                        player_damage_event_t* ev = ServerPlayerDamageEvent_new(target_id, SERVER_STATE->conns[conn_i].player_id, weapon_id, damage);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else if (strncmp(cmd, CLIENT_COMMAND_FIRE, 4) == 0) {
                        int8_t weapon_id;
                        client_fire_from(sdata + cursor, &weapon_id);
                        player_fire_event_t* ev = ServerPlayerFireEvent_new(SERVER_STATE->conns[conn_i].player_id, weapon_id);
                        world_queue_event(&SERVER_STATE->world, (event_t*) ev);
                    } else {
                        printf("Unknown command: %s\n", cmd);
                    }

                    cursor += 6 + payload_len + 1;
                }
            }
            else if (ready < 0)
            {
                printf("SDLNet_UDP_Recv: %s\n", SDLNet_GetError());
            }

            usleep(1000);
            // Update the elapsed time since the last tick
            INSTANT_NOW(&cur_time);
            elapsed = INSTANT_DIFF_MS(cur_time, SERVER_STATE->last_tick);
        }

        // Run the scheduled tasks
        task_executor_run(&SERVER_STATE->task_executor, &cur_time);

        // Update the world state
        if (world_queue_length(&SERVER_STATE->world) > 0)
        {
            // printf("Processing %d events...\n", world_queue_length(&SERVER_STATE->world));
            world_update(&SERVER_STATE->world);
        }

        // Time to clean-up the entity bin
        for (int i = 0; i < entity_bin_count; i++) {
            world_remove_entity_by_id(&SERVER_STATE->world, entity_bin[i]);
        }

        // Update the elapsed time since the last tick
        INSTANT_NOW(&SERVER_STATE->last_tick);
    }
    printf("Shutting down...\n");

    // Kill remaining players
    for (int i = 0; i < SERVER_STATE->conn_count; i++) {
        SERVER_STATE->outgoing->len = server_kick(SERVER_STATE->outgoing->data, "Server shutting down.");
        SERVER_STATE->outgoing->address = SERVER_STATE->conns[i].ip;
        if (SDLNet_UDP_Send(server, -1, SERVER_STATE->outgoing) > 0)
        {
            printf("Kicked %s.\n", addrstr);
        }
        world_remove_entity_by_id(&SERVER_STATE->world, SERVER_STATE->conns[i].player_id);
    }
    SERVER_STATE->conn_count = 0;

    usleep(100000);

    // Clean-up
    printf("Cleaning-up...\n");
    server_world_unload_doors(&SERVER_STATE->world, SERVER_STATE->doors, SERVER_STATE->door_count);
    server_world_unload_lifts(&SERVER_STATE->world, SERVER_STATE->lifts, SERVER_STATE->lift_count);
    world_destroy(&SERVER_STATE->world);
    server_free_wad(SERVER_STATE->wad_data);
    task_executor_destroy(&SERVER_STATE->task_executor);
    SDLNet_UDP_Close(server);
    SDLNet_FreePacket(SERVER_STATE->incoming);
    SDLNet_FreePacket(SERVER_STATE->outgoing);
    SDLNet_Quit();
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("DOOM-like server v0.0.1 - HELO!\n");
    if (argc <= 1)
    {
        printf("Insufficient argument: missing port number.\nUsage: server <port>");
        return -1;
    }
    uint16_t port = (uint16_t)atoi(argv[1]);

    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Running server on port %d...\n", port);
    run_server(port);
    printf("Exiting...\n");
    return 0;
}

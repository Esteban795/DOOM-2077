#include "../../../include/system/server/game_managing.h"
#include "../../../include/event/all.h"
#include "../../../include/ecs/world.h"
#include "../../../include/server/state.h"
#include "../../../include/server/task.h"
#include "../../../include/ecs/entity.h"
#include "../../../include/net/packet/server.h"
#include "../../../include/component/weapon.h"
#include "../../../include/component/health.h"
#include "../../../include/component/position.h"
#include "../../../include/component/statistic.h"
#include "../../../include/spawnpoints.h"
#include "../../../include/event/scoreboard_update.h"

const system_t GAME_MANAGING_SYSTEM = {
    .fn = game_managing,
};

// Game cooldown data, used by the game cooldown task.
typedef struct {
    // Seconds left until the game starts.
    int countdown;
    // When this tasks must be executed.
    struct timespec execute_at;
} game_cooldown_data_t;

void game_start_cooldown_task(void* data);
void game_end_task(void* data);
void game_restart_task(void* _data);

// Game start cooldown task, this task is responsible for the countdown before the game starts.
void game_start_cooldown_task(void* data) {
    if (SERVER_STATE->game_state != GAME_STATE_COOLDOWN) {
        return;
    }
    game_cooldown_data_t* gcd = (game_cooldown_data_t*)data;
    game_start_event_t* game_start_event = ServerGameStartEvent_new(gcd->countdown);
    world_queue_event(&SERVER_STATE->world, (event_t*)game_start_event);
    if (gcd->countdown == 0) {
        // Start the game.
        SERVER_STATE->game_state = GAME_STATE_RUNNING;
        game_cooldown_data_t* new_gcd = malloc(sizeof(game_cooldown_data_t));
        new_gcd->countdown = SERVER_GAME_DURATION - 5;
        new_gcd->execute_at = gcd->execute_at;
        new_gcd->execute_at.tv_sec += SERVER_GAME_DURATION - 5;
        task_t task = task_new(game_end_task, new_gcd, new_gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    } else {
        // Decrement the countdown.
        game_cooldown_data_t* new_gcd = malloc(sizeof(game_cooldown_data_t));
        new_gcd->countdown = gcd->countdown - 1;
        new_gcd->execute_at = gcd->execute_at;
        new_gcd->execute_at.tv_sec += 1;
        task_t task = task_new(game_start_cooldown_task, new_gcd, new_gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    }
}

// Game end task, this task is responsible for ending the game.
void game_end_task(void* data) {
    if (SERVER_STATE->game_state != GAME_STATE_RUNNING) {
        return;
    }
    game_cooldown_data_t* gcd = (game_cooldown_data_t*)data;
    game_end_event_t* game_end_event = ServerGameEndEvent_new(gcd->countdown);
    world_queue_event(&SERVER_STATE->world, (event_t*)game_end_event);
    if (gcd->countdown == 0) {
        SERVER_STATE->game_state = GAME_STATE_ENDING;
        // Restart the game after a cooldown.
        char msg[128];
        snprintf(msg, 128, "Restarting in %d seconds...", SERVER_GAME_COOLDOWN_BEFORE_RESTART);
        server_chat_event_t* chat_event = ServerServerChatEvent_new(msg, true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
        struct timespec execute_at = gcd->execute_at;
        execute_at.tv_sec += SERVER_GAME_COOLDOWN_BEFORE_RESTART;
        task_t task = task_new(game_restart_task, NULL, execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    } else {
        // Decrement the countdown.
        game_cooldown_data_t* new_gcd = malloc(sizeof(game_cooldown_data_t));
        new_gcd->countdown = gcd->countdown - 1;
        new_gcd->execute_at = gcd->execute_at;
        new_gcd->execute_at.tv_sec += 1;
        task_t task = task_new(game_end_task, new_gcd, new_gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    }
}

// Game restart task, this task is responsible for restarting the game after a reasonable cooldown.
void game_restart_task(void* _data) {
    if (SERVER_STATE->game_state != GAME_STATE_ENDING) {
        return;
    }
    if (SERVER_STATE->conn_count >= SERVER_MINIMUM_PLAYERS) {
        SERVER_STATE->game_state = GAME_STATE_COOLDOWN;
        game_cooldown_data_t* gcd = malloc(sizeof(game_cooldown_data_t));
        gcd->countdown = 5;
        INSTANT_NOW(&gcd->execute_at);
        task_t task = task_new(game_start_cooldown_task, gcd, gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    } else {
        SERVER_STATE->game_state = GAME_STATE_WAITING;
        server_chat_event_t* chat_event = ServerServerChatEvent_new("Not enough players to restart a game...", true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
    }
}


/*
* Game managing system, this system is responsible for managing the launch and end of the game.
*/
int game_managing(world_t* world, event_t* event) {
    switch (event->tag) {
        case SERVER_PLAYER_JOIN_EVENT_TAG: {
            if (SERVER_STATE->game_state != GAME_STATE_WAITING) {
                return 0;
            }
            if (SERVER_STATE->conn_count >= SERVER_MINIMUM_PLAYERS) {
                SERVER_STATE->game_state = GAME_STATE_COOLDOWN;
                // event_t* game_start_event = event_new(SERVER_GAME_START_IN_EVENT_TAG, NULL);
                // world_push_event(world, game_start_event);
                game_cooldown_data_t* gcd = malloc(sizeof(game_cooldown_data_t));
                gcd->countdown = 5;
                INSTANT_NOW(&gcd->execute_at);
                task_t task = task_new(game_start_cooldown_task, gcd, gcd->execute_at);
                task_executor_add(&SERVER_STATE->task_executor, task);
            }
            break;
        }
        case SERVER_PLAYER_QUIT_EVENT_TAG: {
            if (SERVER_STATE->game_state == GAME_STATE_COOLDOWN && SERVER_STATE->conn_count < SERVER_MINIMUM_PLAYERS) {
                SERVER_STATE->game_state = GAME_STATE_WAITING;
                server_chat_event_t* chat_event = ServerServerChatEvent_new("Game aborted! Not enough players...", true, true);
                world_queue_event(world, (event_t*)chat_event);
            } else if (SERVER_STATE->game_state == GAME_STATE_RUNNING && SERVER_STATE->conn_count <= 1) {
                SERVER_STATE->game_state = GAME_STATE_ENDING;
                server_chat_event_t* chat_event = ServerServerChatEvent_new("Game ended! Not enough players still in game.", true, true);
                world_queue_event(world, (event_t*)chat_event);
            } else if (SERVER_STATE->game_state == GAME_STATE_ENDING && SERVER_STATE->conn_count < 1) {
                SERVER_STATE->game_state = GAME_STATE_WAITING;
            }
            break;
        }
        case SERVER_GAME_START_EVENT_TAG: {
            uint8_t buf_all[2048] = {0};
            int len_all = 0;
            uint8_t* buf = SERVER_STATE->outgoing->data;
            int len = 0;

            game_start_event_t* ev = (game_start_event_t*)event;
            if (ev->countdown == 0) {
                for (int i = 0; i < SERVER_STATE->conn_count; i++) {
                    uint64_t pid = SERVER_STATE->conns[i].player_id;
                    entity_t player = ENTITY_BY_ID(pid);
                    len = 0;

                    // Reset the weapons of all players.
                    weapon_ct* weapon = (weapon_ct*)world_get_component(world, &player, COMPONENT_TAG_WEAPON);
                    if (weapon == NULL) {
                        continue;
                    }
                    weapon->ammunitions[0] = -2;
                    weapon->mags[0] = 0;
                    for (int j = 1; j < WEAPONS_NUMBER; j++) {
                        weapon->ammunitions[j] = 10;
                        weapon->mags[j] = 10;
                    }
                    len += server_player_weapon_update(buf + len, weapon->ammunitions, weapon->mags, weapon->cooldowns);

                    // Reset the health of all players.
                    health_ct* health = (health_ct*)world_get_component(world, &player, COMPONENT_TAG_HEALTH);
                    if (health == NULL) {
                        continue;
                    }
                    health->health = 100.0;
                    health->max_health = 100.0;
                    len_all += server_player_health(buf_all + len_all, pid, health->health, health->max_health);

                    // Reset the position of all players.
                    position_ct* position = (position_ct*)world_get_component(world, &player, COMPONENT_TAG_POSITION);
                    if (position == NULL) {
                        continue;
                    }
                    spawnpoint sp = get_random_spawnpoint();
                    position->x = sp.x;
                    position->y = sp.y;
                    position->z = sp.z;
                    position->angle = sp.angle;
                    printf("Player %lu spawned at (%f, %f, %f) with angle %f\n", pid, position->x, position->y, position->z, position->angle);
                    len_all += server_player_move(buf_all + len_all, pid, position->x, position->y, position->z, position->angle);

                    // Reset the statistics of all players.
                    statistic_ct* statistic = (statistic_ct*)world_get_component(world, &player, COMPONENT_TAG_STATISTIC);
                    if (statistic == NULL) {
                        continue;
                    }
                    statistic->deaths = 0;
                    statistic->kills = 0;

                    // Send the player packet
                    SERVER_STATE->outgoing->len = len;
                    int pind = find_conn_by_id(SERVER_STATE->conns, SERVER_STATE->conn_count, pid);
                    if (pind >= 0) {
                        SERVER_STATE->outgoing->address = SERVER_STATE->conns[pind].ip;
                        SDLNet_UDP_Send(SERVER_STATE->sock, -1, SERVER_STATE->outgoing);
                    }
                }
                
                // Reset the scoreboard.
                char* entries[10] = {0};
                uint16_t deaths[10] = {0};
                uint16_t kills[10] = {0};
                int count = scoreboard_generate(world, entries, deaths, kills);
                event_t* scoreboard_event = (event_t*) ServerScoreboardUpdateEvent_new(count, entries, deaths, kills);
                world_queue_event(world, scoreboard_event);

                // Send the broadcast packet
                broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count, buf_all, len_all);
            }
            break;
        }
        case SERVER_GAME_END_EVENT_TAG: {
            break;
        }
        default:
            break;
    }
    return 0;
}

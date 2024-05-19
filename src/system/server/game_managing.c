#include "../../../include/system/server/game_managing.h"
#include "../../../include/event/all.h"
#include "../../../include/ecs/world.h"
#include "../../../include/server/state.h"
#include "../../../include/server/task.h"

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

// Game start cooldown task, this task is responsible for the countdown before the game starts.
void game_start_cooldown_task(void* data) {
    if (SERVER_STATE->game_state != GAME_STATE_COOLDOWN) {
        return;
    }
    game_cooldown_data_t* gcd = (game_cooldown_data_t*)data;
    if (gcd->countdown == 0) {
        SERVER_STATE->game_state = GAME_STATE_RUNNING;
        server_chat_event_t* chat_event = ServerServerChatEvent_new("The game is starting!", true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
        game_cooldown_data_t* new_gcd = malloc(sizeof(game_cooldown_data_t));
        new_gcd->countdown = SERVER_GAME_DURATION - 5;
        new_gcd->execute_at = gcd->execute_at;
        new_gcd->execute_at.tv_sec += SERVER_GAME_DURATION - 5;
        task_t task = task_new(game_end_task, new_gcd, new_gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
    } else {
        char message[64];
        snprintf(message, 64, "The game is starting in %d seconds...", gcd->countdown);
        server_chat_event_t* chat_event = ServerServerChatEvent_new(message, true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
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
    if (gcd->countdown == 0) {
        SERVER_STATE->game_state = GAME_STATE_ENDING;
        server_chat_event_t* chat_event = ServerServerChatEvent_new("The game is ending!", true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
    } else {
        char message[64];
        snprintf(message, 64, "The game is ending in %d seconds...", gcd->countdown);
        server_chat_event_t* chat_event = ServerServerChatEvent_new(message, true, true);
        world_queue_event(&SERVER_STATE->world, (event_t*)chat_event);
        game_cooldown_data_t* new_gcd = malloc(sizeof(game_cooldown_data_t));
        new_gcd->countdown = gcd->countdown - 1;
        new_gcd->execute_at = gcd->execute_at;
        new_gcd->execute_at.tv_sec += 1;
        task_t task = task_new(game_end_task, new_gcd, new_gcd->execute_at);
        task_executor_add(&SERVER_STATE->task_executor, task);
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
            }
            break;
        }
        // case SERVER_GAME_START_IN_EVENT_TAG: {
        //     break;
        // }
        // case SERVER_GAME_END_IN_EVENT_TAG: {
        //     break;
        // }
        default:
            break;
    }
    return 0;
}

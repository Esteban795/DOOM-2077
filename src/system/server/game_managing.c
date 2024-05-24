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

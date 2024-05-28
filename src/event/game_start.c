#include "../../include/event/game_start.h"

game_start_event_t* game_start_event_new(uint16_t tag, int16_t countdown) {
    game_start_event_t* event = malloc(sizeof(game_start_event_t));
    event->tag = tag;
    event->countdown = countdown;
    return event;
}

game_start_event_t* ClientGameStartEvent_new(int16_t countdown) {
    return game_start_event_new(CLIENT_GAME_START_EVENT_TAG, countdown);
}

game_start_event_t* ServerGameStartEvent_new(int16_t countdown) {
    return game_start_event_new(SERVER_GAME_START_EVENT_TAG, countdown);
}
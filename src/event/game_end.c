#include "../../include/event/game_end.h"

game_end_event_t* game_end_event_new(uint16_t tag, int16_t countdown) {
    game_end_event_t* event = malloc(sizeof(game_end_event_t));
    event->tag = tag;
    event->countdown = countdown;
    return event;
}

game_end_event_t* ClientGameEndEvent_new(int16_t countdown) {
    return game_end_event_new(CLIENT_GAME_END_EVENT_TAG, countdown);
}

game_end_event_t* ServerGameEndEvent_new(int16_t countdown) {
    return game_end_event_new(SERVER_GAME_END_EVENT_TAG, countdown);
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/player_chat.h"

player_chat_event_t* player_chat_event_new(uint16_t tag, uint64_t entity_id, char* message) {
    player_chat_event_t* event = malloc(sizeof(player_chat_event_t));
    event->tag = tag;
    event->entity_id = entity_id;
    event->message = message;
    event->message_length = strlen(message);
    return event;
}

player_chat_event_t* ClientPlayerChatEvent_new(uint64_t entity_id, char* message) {
   return player_chat_event_new(CLIENT_PLAYER_CHAT_EVENT_TAG, entity_id, message);
}

player_chat_event_t* ServerPlayerChatEvent_new(uint64_t entity_id, char* message) {
    return player_chat_event_new(SERVER_PLAYER_CHAT_EVENT_TAG, entity_id, message);
}
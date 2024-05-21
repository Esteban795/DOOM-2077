#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/server_chat.h"

server_chat_event_t* server_chat_event_new(uint16_t tag, char* message, bool is_broadcast, bool is_title) {
    server_chat_event_t* event = malloc(sizeof(server_chat_event_t));
    event->tag = tag;
    strncpy(event->message, message, 1023);
    event->message[1023] = '\0';
    event->message_length = strlen(event->message);
    event->is_broadcast = is_broadcast;
    event->is_title = is_title;
    return event;
}

server_chat_event_t* ClientServerChatEvent_new(char* message, bool is_broadcast, bool is_title) {
    return server_chat_event_new(CLIENT_SERVER_CHAT_EVENT_TAG, message, is_broadcast, is_title);
}

server_chat_event_t* ServerServerChatEvent_new(char* message, bool is_broadcast, bool is_title) {
    return server_chat_event_new(SERVER_SERVER_CHAT_EVENT_TAG, message, is_broadcast, is_title);
}

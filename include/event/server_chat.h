#ifndef _LIB_DOOM_EVENT_SERVERCHAT_H
#define _LIB_DOOM_EVENT_SERVERCHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_SERVER_CHAT_EVENT_TAG 0x00c1
#define SERVER_SERVER_CHAT_EVENT_TAG (CLIENT_SERVER_CHAT_EVENT_TAG | 0x8000)

// server_chat_event_t is an event that is triggered when the server sends a message in the chat.
typedef struct {
    uint16_t tag;
    // The message is a null-terminated string.
    char message[1024];
    // length of the message.
    uint16_t message_length;
    // is this a broadcast message?
    bool is_broadcast;
    // should the message be displayed as a title bar?
    bool is_title;
} server_chat_event_t;

// ClientServerChatEvent_new creates a new ClientServerChatEvent.
server_chat_event_t* ClientServerChatEvent_new(char* message, bool is_broadcast, bool is_title);

// ServerServerChatEvent_new creates a new ServerServerChatEvent.
server_chat_event_t* ServerServerChatEvent_new(char* message, bool is_broadcast, bool is_title);

#endif
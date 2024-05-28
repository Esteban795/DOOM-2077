#ifndef _LIB_DOOM_EVENT_SERVERCHAT_H
#define _LIB_DOOM_EVENT_SERVERCHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the client server chat event.
#define CLIENT_SERVER_CHAT_EVENT_TAG 0x00c1
/// Event tag for the server server chat event.
#define SERVER_SERVER_CHAT_EVENT_TAG (CLIENT_SERVER_CHAT_EVENT_TAG | 0x8000)

/// server_chat_event_t is an event that is triggered when the server sends a message in the chat.
typedef struct {
    /// @brief The tag of the event.  
    /// This should be CLIENT_SERVER_CHAT_EVENT_TAG or SERVER_SERVER_CHAT_EVENT_TAG
    uint16_t tag;
    /// The message is a null-terminated string.
    /// \remark The message should be at most 1023 characters long.
    char message[1024];
    /// length of the message.
    uint16_t message_length;
    /// is this a broadcast message?
    bool is_broadcast;
    /// should the message be displayed as a title bar?
    bool is_title;
} server_chat_event_t;

/// ClientServerChatEvent_new creates a new ClientServerChatEvent.
///
/// \param message message that the server sent, it should be at most 1023 characters long.
/// \param is_broadcast is this a broadcast message?
/// \param is_title should the message be displayed as a title bar?
server_chat_event_t* ClientServerChatEvent_new(char* message, bool is_broadcast, bool is_title);

/// ServerServerChatEvent_new creates a new ServerServerChatEvent.
///
/// \param message message that the server sent, it should be at most 1023 characters long.
/// \param is_broadcast is this a broadcast message?
/// \param is_title should the message be displayed as a title bar?
server_chat_event_t* ServerServerChatEvent_new(char* message, bool is_broadcast, bool is_title);

#endif
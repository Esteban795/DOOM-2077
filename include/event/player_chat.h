#ifndef _LIB_DOOM_EVENT_PLAYERCHAT_H
#define _LIB_DOOM_EVENT_PLAYERCHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the client player chat event.
#define CLIENT_PLAYER_CHAT_EVENT_TAG 0x00c0
/// Event tag for the server player chat event.
#define SERVER_PLAYER_CHAT_EVENT_TAG (CLIENT_PLAYER_CHAT_EVENT_TAG | 0x8000)

/// player_chat_event_t is an event that is triggered when a player sends a message in the chat.
typedef struct {
    /// @brief The tag of the event.  
    /// This should be CLIENT_PLAYER_CHAT_EVENT_TAG or SERVER_PLAYER_CHAT_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player.
    uint64_t entity_id;
    /// message that the player sent.  
    /// The message is a null-terminated string.
    ///
    /// \remark The message should be at most 1023 characters long.
    char message[1024];
    /// length of the message.
    uint16_t message_length;
} player_chat_event_t;

/// ClientPlayerChatEvent_new creates a new ClientPlayerChatEvent.
///
/// \param entity_id entity_id of the sender.
/// \param message message that the player sent, it should be at most 1023 characters long.
/// \return a new PlayerChatEvent.
player_chat_event_t* ClientPlayerChatEvent_new(uint64_t entity_id, char* message);

/// ServerPlayerChatEvent_new creates a new ServerPlayerChatEvent.
///
/// \param entity_id entity_id of the sender.
/// \param message message that the player sent, it should be at most 1023 characters long.
/// \return a new PlayerChatEvent.
player_chat_event_t* ServerPlayerChatEvent_new(uint64_t entity_id, char* message);

#endif
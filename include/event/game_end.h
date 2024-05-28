#ifndef _LIB_DOOM_EVENT_GAMEEND_H
#define _LIB_DOOM_EVENT_GAMEEND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// CLIENT_GAME_END_EVENT_TAG is the tag for the client-side game end event.
#define CLIENT_GAME_END_EVENT_TAG 0x0004
/// SERVER_GAME_END_EVENT_TAG is the tag for the server-side game end event.
#define SERVER_GAME_END_EVENT_TAG (CLIENT_GAME_END_EVENT_TAG | 0x8000)

/// game_end_event_t is an event that is triggered when the game is about to ends.  
/// It features a countdown time in seconds of the remaining time before the game ends.  
/// If the countdown time is 0, the game has ended.  
/// If the countdown time is negative, the countdown has been cancelled.
typedef struct {
    uint16_t tag;
    /// countdown time in seconds before the game ends.  
    /// If the countdown time is 0, the game has ended.  
    /// If the countdown time is negative, the countdown has been cancelled.
    int16_t countdown;
} game_end_event_t;

// ClientGameEndEvent_new creates a new ClientGameEndEvent.
/// @param countdown is the countdown time in seconds before the game ends.
///                  if the countdown time is 0, the game has started.
///                  if the countdown time is negative, the countdown has been cancelled.
game_end_event_t* ClientGameEndEvent_new(int16_t countdown);

// ServerGameEndEvent_new creates a new ServerGameEndEvent.
/// @param countdown is the countdown time in seconds before the game ends.
///                  if the countdown time is 0, the game has started.
///                  if the countdown time is negative, the countdown has been cancelled.
game_end_event_t* ServerGameEndEvent_new(int16_t countdown);

#endif
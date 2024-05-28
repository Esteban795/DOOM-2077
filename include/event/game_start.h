#ifndef _LIB_DOOM_EVENT_GAMESTART_H
#define _LIB_DOOM_EVENT_GAMESTART_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// CLIENT_GAME_START_EVENT_TAG is the tag for the client-side game start event.
#define CLIENT_GAME_START_EVENT_TAG 0x0003
/// SERVER_GAME_START_EVENT_TAG is the tag for the server-side game start event.
#define SERVER_GAME_START_EVENT_TAG (CLIENT_GAME_START_EVENT_TAG | 0x8000)

/// game_start_event_t is an event that is triggered when the game is about to start.  
/// It features a countdown time in seconds of the remaining time before the game starts.  
/// If the countdown time is 0, the game has started.  
/// If the countdown time is negative, the countdown has been cancelled.
///
/// The goal of the countdown time is to allow the clients to prepare for the game, and 
/// to allow the server to wait for all clients to be ready before starting the game.
typedef struct {
    /// tag is the event tag.
    /// It is either CLIENT_GAME_START_EVENT_TAG or SERVER_GAME_START_EVENT_TAG.
    uint16_t tag;
    /// countdown time in seconds before the game starts.  
    /// If the countdown time is 0, the game has started.
    int16_t countdown;
} game_start_event_t;

/// ClientGameStartEvent_new creates a new ClientGameStartEvent.
/// @param countdown is the countdown time in seconds before the game starts.
///                  if the countdown time is 0, the game has started.
///                  if the countdown time is negative, the countdown has been cancelled.
game_start_event_t* ClientGameStartEvent_new(int16_t countdown);

// ServerGameStartEvent_new creates a new ServerGameStartEvent.
/// @param countdown is the countdown time in seconds before the game starts.
///                  if the countdown time is 0, the game has started.
///                  if the countdown time is negative, the countdown has been cancelled.
game_start_event_t* ServerGameStartEvent_new(int16_t countdown);

#endif
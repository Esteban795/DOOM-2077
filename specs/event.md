\page event Event System

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 27/04/2024       | 28/05/2024       | 1.0     | Release           |

# Introduction

This set uses events to perform actions and modify the states of ECS components. Most of these events are also retransmitted over the
network to be synchronized between clients.

An event generally exists in a server and a client form, which may not contain the same elements, but one is usually
materialized by the other after synchronization.
An event is generally linked to a particular command found in \ref network.

# Implemented events

## Global events

* [x] 0x0001 - PlayerJoinEvent {CS} - a player has joined the party
* [x] 0x0002 - PlayerQuitEvent {CS} - a player has quitted the party
* [x] 0x0003 - GameStartsInEvent {S} - the game will begin in XXs (0 -> game launches)
* [x] 0x0004 - GameEndsInEvent {S} - the game will ends in XXs (0 -> game stops)

## Player-specific events

* [x] 0x0040 - PlayerMoveEvent {CS} - a player is moving to a position
* [x] 0x0041 - PlayerDamageEvent {CS} - a player takes damages
* [x] 0x0042 - PlayerHealEvent {CS} - a player has been healed
* [x] 0x0043 - PlayerHealthUpdateEvent {CS} - a player's health has been changed.
* [x] 0x0044 - PlayerKilledEvent {CS} - a player has been killed
* [x] 0x0045 - PlayerFireEvent {CS} - a player fires a weapon
* [x] 0x0046 - PlayerWeaponUpdateEvent {S} - set the ammunitions, mags and cooldowns of weapons owned by a player

## Door/Elevator events

* 0x0080 - DoorStateEvent {S} - set the current state of the doors and lifts
* [x] 0x0081 - DoorOpenEvent {CS} - a player is opening a door (lift ascending)
* [x] 0x0082 - DoorCloseEvent {CS} - a player is closing a door (lift descending) 

## Communication events

* [x] 0x00c0 - PlayerChatEvent {CS} - a player sent a message in the chat
* [x] 0x00c1 - ServerChatEvent {CS} - the server sent a messsage in the chat
* [x] 0x00c2 - ScoreboardUpdateEvent {C} - the scoreboard should be updated.
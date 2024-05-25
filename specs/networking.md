\page network Network Protocol

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 07/03/2024       | 25/05/2024       | RC1     | Release Candidate |


# Introduction

The goal of this document is to document the exact way the game client is communicating with the game server. It must allow someone unfamiliar with the project but familiar with such protocol to completely reimplement the protocol for its own client or server and to expect it to be compatible with the original game.

## Basic aspect

This protocol is not expected to be readable by human, however it must be sufficiently easy to debug it. Therefore, this protocol is expected to be reasonably readeable as Python byte-string.

The protocol is built upon **messages**. Each message contains a unique **command**, a 4 char/byte string, immediately followed by the **length** of the payload. The **payload** is all useful information that is necessary to execute a command. Some message are expected to have no payload, a variable payload, or a fixed size payload based on the message's command. Finally, all messages is ended by a newline `\n` character.

The implementation of this protocol assumes the underlying transmission protocol is UDP. **Please be aware, a UDP packet might contain serveral messages.** A UDP packet is expected to be **at most 1024 byte-long**.

### Quick recap

A message is of the following form:

```
--------------------------------------------------------------------|
| COMMAND (4-bytes) | LENGTH (uint16) | PAYLOAD (length bytes) | \n |
--------------------------------------------------------------------|
```

### Technicalities

* All integers, signed or not, are represented in network byte order (big-endianness).

* Floats are not used in the network protocol. If a float is needed, it is represented as a integer value
premultiplied by a constant. For instance, player coordinate are represented as mm (multiplied by 1000).

* Unless explicitly mentioned, strings are not NUL-terminated.

* A char is considered as an alias for a uint8.

* Command identifiers are **NOT** shared between client and server. A identical identifier can be use for two distinct usage.

# Client->Server Messages

## JOIN - Client Join

**Description**: A `JOIN` message is the first message a client should send to the server. It indicates to the server that it wants to connect.
A `JOIN` message can be acknowledged by an `ACPT` message, meaning the client is now connected to the server and will receive future data. 
The server can also respond to the request by a `KICK` message that indicates the reason for the refusal. After a reasonable amount of time (2s) without response the client can send a new `JOIN` request.

**Args:**

* `name`, the name of the client. This client will be known by the server and the other clients by this name.

The `name` argument is a NUL-terminated string, whose length cannot exceed 100(+1) chars.

## KATC - Keep Alive

**Description**: Keep-alive message to tell the server the client is still there and listening for any response. Indeed, the server can close the connection if the client do not send any message for 20s. No response is expected.

**Args:**

No argument is provided.

## PING - ping

**Description**: A message to calculate the ping between client and server. This packet is special as it is handled once it is received and not at the end of the server tick. A `PONG` message is expected to be sent back with the same data.

**Args:**

* `data`: 8 byte-long data, to identify uniquely the exchange.

## QUIT - Client Quit

**Description**: A `QUIT` message is the last message a client should send to a server, it indicates that the client is disconnecting from the server and that it will closes its connection. A `QUIT` message is generally acknowledged by a `QUIT` message from the server. It is not necessary to await it.

**Args:**

No argument is provided.

## MOVE - Player Move

**Description**: A `MOVE` message announces the player change in absolute coordinates (and angle).

**Args:**
* X coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Y coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Z coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Angle - double as a int64, expressed in mm (premultiplied by 1000)

Example: Player is in (x: 4.567, y: -128.140, z: 47.000, a: 97.000), the arguments of the move message are the following: (x: 4567, y: -128140, z: 47000, a: 97000).

**Len:** 32 bytes

## CHAT - Player Chat

**Description**: Send a message in the chat

**Args:**
* `msg` (cstring), the chat message as a NUL-terminated string.


# Server->Client Messages

## ACPT - Accept Player

**Description**: An `ACPT` message sent as a reply to a `JOIN` request. The client is now considered part of the players on the server, and should expect more exchanges.

**Args:**

* `player-id`, a 8 byte-long unique identifier for a player. It will be used as a reference for all other messages.

## JOIN - Player Join

**Description**: A `JOIN` message is sent to all players currently on the server when a new player is joining the game.

**Args:**

* `player-id`, a 8 byte-long unique identifier for a player. It will be used as a reference for all other messages.

* `name`, the name of the new player.

The `name` argument is a NUL-terminated string, whose length cannot exceed 100(+1) chars.

## KICK - Kick Player

**Description**: A `KICK` message is a (spontaneous or not) message that indicates that the server do not want to continue the connection with the client.
The player, if they were part of the game, is now forced to leave.

**Args:**

* `reason`, the reason (if any) of the kick.

The `reason` argument is a NUL-terminated string, whose length cannot exceed 255(+1) chars.

## PONG - pong

**Description**: A message to calculate the ping between client and server, it is a response to a `PING` message.

**Args:**

* `data`: 8 byte-long data, the data of the `PING` request.

## QUIT - Player Quit

**Description**: A `QUIT` message is sent to all players currently on the server when a player is quitting the game.

**Args:**

* `player-id`, the 8 byte-long unique identifier of the player.

## MOVE - Player Move

**Description**: Indicate the move of a player

**Args :**

* `player_id` (uint64), the unique entity_id of the player moving
* X coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Y coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Z coordinate - double as a int64, expressed in mm (premultiplied by 1000)
* Angle - double as a int64, expressed in mm (premultiplied by 1000)

## CHAT - Player Chat

**Description**: A player sent a message in the chat

**Args:**
* `player-id`, the 8 byte-long unique identifier of the player.
* `msg` (cstring), the chat message as a NUL-terminated string.

## TELL - Server Chat

**Description**: The server sent a message in the chat

**Args:**
* Flags (uint8-bitflag)
* * 0x01: is_broadcast -> is this a broadcast message?
* * 0x02: is_title -> is this chat a title message?
* `msg` (cstring), the chat message as a NUL-terminated string.

## SCOR - Scoreboard Update

**Description**: The scoreboard has been updated

**Args:**
* Entry count (uint16): number of entries.
* Names (array of C-String): Names of the player in the order of the scoreboard (asc), separated by NUL.
* Deaths (array of uint16): Number of deaths of the player in the order of the scoreboard (asc).
* Kills (array of uint16): Number of kills of the player in the order of the scoreboard (asc).*

## DAMG - Player Damage

**Description**: A player has been affected by something and has lost health.

**Args:**
* `player_id`, the 8 byte-long unique identifier of the player affected.
* `src_player_id`, the 8-byte-long UID of the player that caused the damages (or 0 if none).
* `damage`: the health points lost by the player.

## KILL - Player Kill

**Description**: A player has been killed by something.

**Args:**
* `player_id`, the 8 byte-long unique identifier of the player affected.
* `src_player_id`, the 8-byte-long UID of the player that caused the damages (or 0 if none).

## HEAL - Player Heal

**Description**: A player is healing.

**Args:**
* `player_id`, the 8 byte-long unique identifier of the player affected.
* `gain`: the health points earned by the player.

## HLTH - Player Health Update

**Description**: The health of a player has changed

**Args:**
* `player_id`, the 8 byte-long unique identifier of the player affected.
* `health`: the health points of the player.
* `max_health`: the maximum health, the player can have.

## LMAP - Load Map

**Description:** The server requests the client to load the following map.

**Args:**:
* `map_name` (cstring), a NUL-terminated string identifing the map in the WAP.

## OPEN / CLOS - Open / Close doors 

**Description:** A player is opening/closing a door

**Args:**:
* `door_id` (uint64): unique index of the door opening/closing 

## LASC / LDSC - Lift Ascend / Descend

**Description:** A lift is ascending / descending

**Args:**:
* `lift_id` (uint64): unique index of the lift ascending/descending

## DOST / L_ST - Door States / Lift States

**Description:** States of all doors/lifts (generally sent at join)

**Args:**:
* `door_count`/`lift_count` (uint16): the number of items in the following array
* `states` (uint8 array as bool array): the state of each door : 1 for Open/High and 0 for Closed/Low

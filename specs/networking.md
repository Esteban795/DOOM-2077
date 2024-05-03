# Network Protocol

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 07/03/2024       | 27/04/2024       | alpha   | Outdated          |


## Introduction

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

* Unless explicitly mentioned, strings are not NUL-terminated.

* A char is considered as an alias for a uint8.

* Command identifiers are **NOT** shared between client and server. A identical identifier can be use for two distinct usage.

## Client->Server Messages

### JOIN - Client Join

**Description**: A `JOIN` message is the first message a client should send to the server. It indicates to the server that it wants to connect.
A `JOIN` message can be acknowledged by an `ACPT` message, meaning the client is now connected to the server and will receive future data. 
The server can also respond to the request by a `KICK` message that indicates the reason for the refusal. After a reasonable amount of time (2s) without response the client can send a new `JOIN` request.

**Args:**

* `name`, the name of the client. This client will be known by the server and the other clients by this name.

The `name` argument is a NUL-terminated string, whose length cannot exceed 100(+1) chars.

### KATC - Keep Alive

**Description**: Keep-alive message to tell the server the client is still there and listening for any response. Indeed, the server can close the connection if the client do not send any message for 20s. No response is expected.

**Args:**

No argument is provided.

### PING - ping

**Description**: A message to calculate the ping between client and server. This packet is special as it is handled once it is received and not at the end of the server tick. A `PONG` message is expected to be sent back with the same data.

**Args:**

* `data`: 8 byte-long data, to identify uniquely the exchange.

### QUIT - Client Quit

**Description**: A `QUIT` message is the last message a client should send to a server, it indicates that the client is disconnecting from the server and that it will closes its connection. A `QUIT` message is generally acknowledged by a `QUIT` message from the server. It is not necessary to await it.

**Args:**

No argument is provided.

### CPL_ - Player List

**Description**: Get the list of players currently online.

**Args:**

No argument is provided.

## Server->Client Messages

### ACPT - Accept Player

**Description**: An `ACPT` message sent as a reply to a `JOIN` request. The client is now considered part of the players on the server, and should expect more exchanges.

**Args:**

* `player-id`, a 8 byte-long unique identifier for a player. It will be used as a reference for all other messages.

### JOIN - Player Join

**Description**: A `JOIN` message is sent to all players currently on the server when a new player is joining the game.

**Args:**

* `player-id`, a 8 byte-long unique identifier for a player. It will be used as a reference for all other messages.

* `name`, the name of the new player.

The `name` argument is a NUL-terminated string, whose length cannot exceed 100(+1) chars.

### KICK - Kick Player

**Description**: A `KICK` message is a (spontaneous or not) message that indicates that the server do not want to continue the connection with the client.
The player, if they were part of the game, is now forced to leave.

**Args:**

* `reason`, the reason (if any) of the kick.

The `reason` argument is a NUL-terminated string, whose length cannot exceed 255(+1) chars.

### Pong - pong

**Description**: A message to calculate the ping between client and server, it is a response to a `PING` message.

**Args:**

* `data`: 8 byte-long data, the data of the `PING` request.

### QUIT - Player Quit

**Description**: A `QUIT` message is sent to all players currently on the server when a player is quitting the game.

**Args:**

* `player-id`, the 8 byte-long unique identifier of the player.

### SPL_ - Player List

**Description**: Response to a `CPL_` request, it includes the complete list of all the players currently connected to the server.

**Args :**

* `player_list`, a list of players :
* * `player_list.length`: number of players
* * `player_list[i]`, a player representation :
* * * `player_list[i].id`, 8 byte-long unique identifier of the player
* * * `player_list[i].name_length`, uint8, the length of the player's name
* * * `player_list[i].name`, string, the name of the player (**warn: this string is not NUL-terminated!**)


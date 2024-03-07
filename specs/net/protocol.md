# Network Protocol of our DOOM-like

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 07/03/2023       | 07/03/2023       | alpha   | Experimental      |


## Introduction

The goal of this document is to document the exact way the game client is communicating with the game server. It must allow someone unfamiliar with the project but familiar with such protocol to completely reimplement the protocol for its own client or server and to expect it to be compatible with the original game.

## Basic aspect

This protocol is not expected to be readable by human, however it must be sufficiently easy to debug it. Therefore, this protocol is expected to be reasonably readeable as Python byte-string.

The protocol is built upon **messages**. Each message contains a unique **command**, a 4 char/byte string, immediately followed by the **length** of the payload. The **payload** is all useful information that is necessary to execute a command. Some message are expected to have no payload, a variable payload, or a fixed size payload based on the message's command. Finally, all messages is ended by a newline `\n` character.

The implementation of this protocol assumes the underlying transmission protocol is UDP. **Please be aware, a UDP packet might contain serveral messages.** A UDP packet is expected to be **at most 2048 byte-long**.

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
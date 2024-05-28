#ifndef _LIB_DOOM_NET_UTIL_H
#define _LIB_DOOM_NET_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#define WITHOUT_SDL
#include <SDL2/SDL_net.h>
#endif

/// @brief Represents an IP address and port as a string.
void addrtocstr(IPaddress* ip, char str[24]);
/// @brief Compares two IP addresses.
int cmp_addr(IPaddress* ip1, IPaddress* ip2);

/// @brief Writes a uint8_t to a buffer in big-endian format.
void write_uint8be(uint8_t* buf, uint8_t data);
/// @brief Writes a uint16_t to a buffer in big-endian format.
void write_uint16be(uint8_t* buf, uint16_t data);
/// @brief Writes a uint32_t to a buffer in big-endian format.
void write_uint32be(uint8_t* buf, uint32_t data);
/// @brief Writes a uint64_t to a buffer in big-endian format.
void write_uint64be(uint8_t* buf, uint64_t data);

/// @brief Writes NUL-terminated string to a buffer.
int write_cstring(uint8_t* buf, char* data);

/// @brief Reads a uint8_t from a buffer in big-endian format.
uint8_t read_uint8be(uint8_t* buf);
/// @brief Reads a uint16_t from a buffer in big-endian format.
uint16_t read_uint16be(uint8_t* buf);
/// @brief Reads a uint32_t from a buffer in big-endian format.
uint32_t read_uint32be(uint8_t* buf);
/// @brief Reads a uint64_t from a buffer in big-endian format.
uint64_t read_uint64be(uint8_t* buf);

/// @brief Reads a int8_t from a buffer in big-endian format.
int8_t  read_int8be(uint8_t* buf);
/// @brief Reads a int16_t from a buffer in big-endian format.
int16_t read_int16be(uint8_t* buf);
/// @brief Reads a int32_t from a buffer in big-endian format.
int32_t read_int32be(uint8_t* buf);
/// @brief Reads a int64_t from a buffer in big-endian format.
int64_t read_int64be(uint8_t* buf);

/// @brief Reads a NUL-terminated string from a buffer.
char* read_cstring(uint8_t* buf);

/// @brief Reads at most len chars of a NUL-terminated string from a buffer.
char* read_copy_string(uint8_t* buf, int len);

#endif
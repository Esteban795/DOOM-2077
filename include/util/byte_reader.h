#ifndef BYTE_READER_H
#define BYTE_READER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef int8_t i8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint32_t u32;
typedef uint8_t u8;
typedef unsigned char byte;

// LUMP OFFSETS IN DOOM WAD
#define THINGS 1
#define LINEDEFS 2
#define SIDEDEFS 3
#define VERTEXES 4
#define SEGS 5
#define SSECTORS 6
#define NODES 7
#define SECTORS 8
#define REJECT 9
#define BLOCKMAP 10

byte read_1_byte(FILE *f, int offset);

byte *read_bytes(FILE *f, int offset, int num_bytes);

u8 read_u8(FILE *f, int offset);

i16 read_i16(FILE *f, int offset);

u16 read_u16(FILE *f, int offset);

i32 read_i32(FILE *f, int offset);

u8 read_u8(FILE *f, int offset);

u32 read_u32(FILE *f, int offset);

char *read_string(FILE *f, int offset, int num_bytes);

char *read_texture_name(FILE *f, int offset, int length);
#endif

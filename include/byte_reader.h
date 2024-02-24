#ifndef BYTE_READER_H
#define BYTE_READER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef unsigned char byte;

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

i16 read_i16(FILE *f, int offset);

u16 read_u16(FILE *f, int offset);

i32 read_i32(FILE *f, int offset);

char *read_string(FILE *f, int offset, int num_bytes);

void read_texture_name(FILE *f, int offset, int8_t *texture_name);
#endif

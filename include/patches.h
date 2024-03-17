#ifndef PATCHES_H
#define PATCHES_H

#include "byte_reader.h"
#include <stdint.h>
#include "color.h"
#include <stdbool.h>

#define NO_PIXELS 0xFF

// https://doomwiki.org/wiki/Picture_format

struct PatchHeader {
    i16 width;
    i16 height;
    i16 x_offset;
    i16 y_offset;
    uint32_t* column_offsets;
};

typedef struct PatchHeader patch_header;

struct PatchColumn {
    u8 top_delta;
    u8 length;
    u8 padding_pre;
    u8* data;
    u8 padding_post;
};

typedef struct PatchColumn patch_column;


typedef struct Patch {
    color* palette;
    patch_header header;
    patch_column* columns;
} patch;

#endif
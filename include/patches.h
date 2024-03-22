#ifndef PATCHES_H
#define PATCHES_H

#include "byte_reader.h"
#include <stdint.h>
#include "color.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

#define NO_PIXELS 0xFF

#define PATCHES_START "S_START"
#define PATCHES_END "S_END"
// https://doomwiki.org/wiki/Picture_format

struct PatchHeader {
    i16 width;
    i16 height;
    i16 x_offset;
    i16 y_offset;
    u32* column_offsets;
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

struct Patch {
    char* patchname;
    color* palette;
    patch_header header;
    patch_column* columns;
    SDL_Texture* patch_img;
};

typedef struct Patch patch;

void display_patches(SDL_Renderer* renderer, patch* patches, int patch_count);

void patches_free(patch *patches, int patch_count);

patch* get_patches(SDL_Renderer* renderer,lump* directory, header* header, FILE* f, color* palette,int* patch_count);
#endif
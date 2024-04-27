#ifndef PATCHES_H
#define PATCHES_H

#include "byte_reader.h"
#include "color.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdint.h>

#define NO_PIXELS 0xFF

#define PATCHES_START "S_START"
#define PATCHES_END "S_END"

#define TRANSPARENT_COLOR                                                      \
  SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0, 0, 0, 0)


// https://doom.fandom.com/wiki/TEXTURE1_and_TEXTURE2
// https://doomwiki.org/wiki/TEXTURE1_and_TEXTURE2

struct PatchHeader {
  i16 width;
  i16 height;
  i16 x_offset;
  i16 y_offset;
  u32 *column_offsets;
};

typedef struct PatchHeader patch_header;

struct PatchColumn {
  u8 top_delta;
  u8 length;
  u8 padding_pre;
  u8 *data;
  u8 padding_post;
};

typedef struct PatchColumn patch_column;

struct Patch {
  int nb_columns;
  char *patchname;
  color *palette;
  patch_header header;
  patch_column *columns;
  SDL_Texture *tex;
  Uint32 *pixels;
};

typedef struct Patch patch;

struct PatchMap {
  i16 left_offset;
  i16 top_offset;
  u16 patch_index;
  u16 step_dir;  // unused
  u16 color_map; // unused
};

typedef struct PatchMap patch_map;

patch_map read_patch_map(FILE *f, int offset);

void display_patches(SDL_Renderer *renderer, patch *patches, int patch_count);

void sprites_free(patch *patches, int patch_count);

void textures_patches_free(patch *patches, int patch_count);

patch *get_sprites(SDL_Renderer *renderer, lump *directory, header *header,
                   FILE *f, color *palette, int *patch_count);

patch *get_texture_patches(SDL_Renderer *rendererer, lump *directory,
                           header *header, FILE *f, color *palette,
                           int *len_textures_patches);

patch *get_patch_from_name(patch *patches, int len_patches, char *name);
#endif
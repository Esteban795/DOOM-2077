#ifndef PATCHES_H
#define PATCHES_H

#include "byte_reader.h"
#include "color.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define NO_PIXELS 0xFF

#define PATCHES_START "S_START"
#define PATCHES_END "S_END"

#define TRANSPARENT_COLOR                                                      \
  SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0, 0, 0, 0)

// https://doomwiki.org/wiki/Picture_format
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
  SDL_Texture *patch_img;
};

typedef struct Patch patch;

struct TextureHeader {
  u32 num_textures;
  u32 *texture_map_offsets;
  u32 *texture_data_offsets;
};

typedef struct TextureHeader texture_header;

struct PatchMap {
  i16 left_offset;
  i16 top_offset;
  u16 patch_index;
  u16 step_dir; // unused 
  u16 color_map; // unused
};

typedef struct PatchMap patch_map;

struct TextureMap {
  char* name;
  bool masked;
  u16 width;
  u16 height;
  u32 column_dir; //obsolete and unused but it still lies in WAD files
  u16 patch_count;
  patch_map* patch_maps;
  int len_patch_maps;
};

typedef struct TextureMap texture_map;

void display_patches(SDL_Renderer *renderer, patch *patches, int patch_count);

void sprites_free(patch *patches, int patch_count);

void textures_patches_free(patch *patches, int patch_count);

patch *get_sprites(SDL_Renderer *renderer, lump *directory, header *header,
                   FILE *f, color *palette, int *patch_count);

patch *get_texture_patches(SDL_Renderer *renderer, lump *directory,
                           header *header, FILE *f, color *palette,
                           int *len_textures_patches);
#endif
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

// /!\ FORMAT USED BY PIXELS EVERYWHERE IS RGBA_8888 


// https://doomwiki.org/wiki/Picture_format
// https://doom.fandom.com/wiki/TEXTURE1_and_TEXTURE2
// https://doomwiki.org/wiki/TEXTURE1_and_TEXTURE2

struct PatchHeader {
  i16 width;  // width of the patch
  i16 height; // height of the patch
  i16 x_offset; // left offset of the patch (related to original doom screen)
  i16 y_offset; // top offset of the patch (related to the original doom screen)
  u32 *column_offsets;
};

typedef struct PatchHeader patch_header;

struct PatchColumn {
  u8 top_delta; // determines how many transparent pixels are from the top of the
               // column
  u8 length;   // length of the column
  u8 padding_pre; // unused
  u8 *data; // rgb data of the column
  u8 padding_post; // unused
};

typedef struct PatchColumn patch_column;


/*
Why is there nb_columns in the patch struct ?
If you have a column that starts at the top of the sprite and goes down 1/3 of it, then stops, then re-starts
it would be counted as 1 columns in the patch_header, but it would be 2 columns in the patch_column.
*/
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

void sprites_free(patch *patches, int patch_count);

void textures_patches_free(patch *patches, int patch_count);

patch *get_sprites(SDL_Renderer *renderer, lump *directory, header *header,
                   FILE *f, color *palette, int *patch_count);

patch *get_texture_patches(SDL_Renderer *renderer, lump *directory,
                           header *header, FILE *f, color *palette,
                           int *len_textures_patches);

patch *get_patch_from_name(patch *patches, int len_patches, char *name);
#endif
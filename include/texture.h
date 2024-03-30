#ifndef TEXTURE_H
#define TEXTURE_H

#include "byte_reader.h"
#include "patch.h"
#include <SDL2/SDL_render.h>

struct TextureMap {
  char *name;
  bool masked;
  u16 width;
  u16 height;
  u32 column_dir; // obsolete and unused but it still lies in WAD files
  u16 patch_count;
  patch_map *patch_maps;
  Uint32* pixels;
};

typedef struct TextureMap texture_map;

struct TextureHeader {
  u32 num_textures;
  u32 *texture_map_offsets;
  texture_map *texture_maps;
};

typedef struct TextureHeader texture_header;

void texture_maps_free(texture_map *texture_maps, int len_texture_maps);

texture_map *get_texture_maps(FILE *f, lump *directory, header *header,
                              patch *patches, SDL_Renderer *renderer,
                              int *len_texture_maps);

texture_map* get_texture_from_name(texture_map* texture_maps, int len_texture_maps, char* name);
#endif
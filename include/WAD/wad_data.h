#ifndef WAD_DATA_H
#define WAD_DATA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "blockmap.h"
#include "color.h"
#include "flat.h"
#include "header.h"
#include "linedef.h"
#include "node.h"
#include "patch.h"
#include "sector.h"
#include "segment.h"
#include "sidedef.h"
#include "sound.h"
#include "subsector.h"
#include "texture.h"
#include "thing.h"
#include "vertex.h"

struct WADData {
  header header;
  lump *directory;
  int map_index;
  vertex *vertexes;
  linedef **linedefs;
  node *nodes;
  subsector *subsectors;
  segment *segments;
  thing *things;
  blockmap *blockmap;
  sector *sectors;
  sidedef *sidedefs;
  sound *sounds;
  int len_sounds;
  color *color_palette;
  patch *sprites;
  patch *texture_patches;
  texture_map *texture_maps;
  flat *flats;
  int len_flats;
  int len_texture_maps;
  int len_texture_patches;
  int len_sprites;
  int len_vertexes;
  int len_linedefs;
  int len_nodes;
  int len_subsectors;
  int len_segments;
  int len_things;
  int len_sectors;
  int len_sidedefs;
};

typedef struct WADData wad_data;

wad_data *init_wad_data(SDL_Renderer *renderer, const char *path);

void load_map(wad_data *wd, const char *path, char *map_name);

void free_map(wad_data *wd);

void wad_data_free(wad_data *wd);

/// Check if a map is currently loaded in the wad_data struct.
bool wad_is_map_loaded(wad_data *wd);

/// Server function to load a WAD file and a particular map.
///
/// When using this function, the server will not load any textures, sounds, or
/// sprites. It will only load the map data. Therefore you MUST only use
/// 'server_free_wad' to free the wad_data struct.
///
/// You can use `load_map` to load another map into the wad_data struct. This is
/// fine.
///
/// **SAFETY:** Use only 'server_free_wad' to free the wad_data struct. Using
/// 'init_wad_data' will lead to Undefined Behavior.
wad_data *server_load_wad(const char *path, char *map_name);

/// Server function to free a wad_data struct.
///
/// This function will free all the memory allocated by the 'server_load_wad'
/// function. It will not free any textures, sounds, or sprites (as they SHOULD
/// NOT be loaded). But it will free the map data, as 'server_load_wad' ensure
/// that a map is always loaded.
///
/// **SAFETY:** Do not mix this function with 'wad_data_free', especially if you
/// loaded a map using 'init_wad_data'.
void server_free_wad(wad_data *wd);
#endif
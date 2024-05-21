#include "../include/wad_data.h"
#include <stdio.h>

void load_textures(wad_data *wd, const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  const int PLAYPAL =
      get_lump_index(wd->directory, "PLAYPAL", wd->header.lump_count);
  wd->color_palette =
      get_color_palette_from_lump(file, wd->directory, PLAYPAL, 3, 0);
  wd->sprites = get_sprites(wd->directory, &wd->header, file, wd->color_palette,
                            &wd->len_sprites);
  wd->texture_patches =
      get_texture_patches(wd->directory, &wd->header, file, wd->color_palette,
                          &wd->len_texture_patches);
  wd->texture_maps =
      get_texture_maps(file, wd->directory, &wd->header, wd->texture_patches,
                       &wd->len_texture_maps);
  wd->flats = get_flats(file, wd->directory, &wd->header, wd->color_palette,
                        &wd->len_flats);
  fclose(file);
}

void load_sounds(wad_data *wd, const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  wd->sounds =
      get_sounds(file, wd->directory, wd->header.lump_count, &wd->len_sounds);
  fclose(file);
}

void load_map(wad_data *wd, const char *path, char *map_name) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  wd->map_index =
      get_lump_index(wd->directory, map_name, wd->header.lump_count);
  wd->len_vertexes = wd->directory[wd->map_index + VERTEXES].lump_size /
                     4; // 4 = number of bytes per vertex
  wd->len_linedefs = wd->directory[wd->map_index + LINEDEFS].lump_size /
                     14; // 14 = number of bytes per linedef
  wd->len_nodes = wd->directory[wd->map_index + NODES].lump_size /
                  28; // 28 = number of bytes per node
  wd->len_subsectors = wd->directory[wd->map_index + SSECTORS].lump_size /
                       4; // 4 = number of bytes per subsector
  wd->len_segments = wd->directory[wd->map_index + SEGS].lump_size /
                     12; // 12 = number of bytes per seg
  wd->len_things = wd->directory[wd->map_index + THINGS].lump_size /
                   10; // 10 = number of bytes per thing
  wd->len_sectors = wd->directory[wd->map_index + SECTORS].lump_size /
                    26; // 26 = number of bytes per sector
  wd->len_sidedefs = wd->directory[wd->map_index + SIDEDEFS].lump_size /
                     30; // 30 = number of bytes per sidedef
  wd->sectors =
      get_sectors_from_lump(file, wd->directory, wd->map_index + SECTORS, 26, 0,
                            wd->len_sectors, wd->flats, wd->len_flats);
  wd->sidedefs = get_sidedefs_from_lump(
      file, wd->directory, wd->map_index + SIDEDEFS, 30, 0, wd->len_sidedefs,
      wd->sectors, wd->texture_maps, wd->len_texture_maps);
  wd->vertexes = get_vertexes_from_lump(
      file, wd->directory, wd->map_index + VERTEXES, 4, 0, wd->len_vertexes);
  wd->linedefs =
      get_linedefs_from_lump(file, wd->directory, wd->map_index + LINEDEFS, 14,
                             0, wd->len_linedefs, wd->vertexes, wd->sidedefs);
  wd->nodes = get_nodes_from_lump(file, wd->directory, wd->map_index + NODES,
                                  28, 0, wd->len_nodes);
  wd->segments = get_segments_from_lump(
      file, wd->directory, wd->map_index + SEGS, 12, 0, wd->len_segments,
      wd->vertexes, wd->linedefs, wd->sectors);
  wd->subsectors =
      get_subsectors_from_lump(file, wd->directory, wd->map_index + SSECTORS, 4,
                               0, wd->len_subsectors, wd->segments);
  wd->things = get_things_from_lump(file, wd->directory, wd->map_index + THINGS,
                                    10, 0, wd->len_things);
  wd->blockmap = read_blockmap_from_lump(
      file, wd->directory, wd->map_index + BLOCKMAP, wd->linedefs);
  set_sectors_centers(wd->linedefs, wd->len_linedefs, wd->sectors,
                      wd->len_sectors);
}

// loads only textures and flats to allow for hot reloading of maps in the same
// WAD file
wad_data *init_wad_data(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  wad_data *wd = malloc(sizeof(wad_data));
  wd->header = read_header(file);
  wd->directory = read_directory(file, wd->header);
  wd->vertexes = NULL;
  load_textures(wd, path);
  load_sounds(wd, path);
  fclose(file);
  return wd;
}

void free_map(wad_data *wd) {
  free(wd->vertexes);
  linedefs_free(wd->linedefs, wd->len_linedefs);
  free(wd->nodes);
  free(wd->segments);
  free(wd->things);
  free(wd->sectors);
  free(wd->sidedefs);
  blockmap_free(wd->blockmap);
  subsectors_free(wd->subsectors, wd->len_subsectors);
  wd->vertexes = NULL;
}

void wad_data_free(wad_data *wd) {
  free_map(wd);
  free(wd->color_palette);
  sprites_free(wd->sprites, wd->len_sprites);
  textures_patches_free(wd->texture_patches, wd->len_texture_patches);
  texture_maps_free(wd->texture_maps, wd->len_texture_maps);
  flats_free(wd->flats, wd->len_flats);
  sounds_free(wd->sounds, wd->len_sounds);
  for (int i = 0; i < wd->header.lump_count; i++) {
    free(wd->directory[i].lump_name);
  }
  free(wd->directory);
  free(wd);
}

bool wad_is_map_loaded(wad_data *wd) { return wd->vertexes != NULL; }

wad_data *server_load_wad(const char *path, char *map_name) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  wad_data *wd = malloc(sizeof(wad_data));
  wd->header = read_header(file);
  wd->directory = read_directory(file, wd->header);
  load_map(wd, path, map_name);
  fclose(file);
  return wd;
}

void server_free_wad(wad_data *wd) {
  free_map(wd);
  for (int i = 0; i < wd->header.lump_count; i++) {
    free(wd->directory[i].lump_name);
  }
  free(wd->directory);
  free(wd);
}
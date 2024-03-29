#include "../include/texture.h"

texture_map read_texture_map(FILE *f, int offset) {
  texture_map tm;
  tm.name = read_texture_name(f, offset, 8);
  tm.masked = (bool)read_i32(f, offset + 8);
  tm.width = read_i16(f, offset + 12);
  tm.height = read_i16(f, offset + 14);
  tm.column_dir = read_i32(f, offset + 16);
  tm.patch_count = read_u16(f, offset + 20);
  tm.patch_maps = malloc(sizeof(patch_map) * tm.patch_count);
  for (int i = 0; i < tm.patch_count; i++) {
    tm.patch_maps[i] = read_patch_map(f, offset + 22 + i * 10);
  }
  return tm;
}

texture_header read_texture_header(FILE *f, int offset) {
  texture_header th;
  th.num_textures = read_i32(f, offset);
  th.texture_map_offsets = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    th.texture_map_offsets[i] = read_i32(f, offset + 4 + i * 4);
  }
  th.texture_maps = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    th.texture_maps[i] =
        read_texture_map(f, offset + th.texture_map_offsets[i]);
  }
  return th;
}

void texture_map_free(texture_map tm) {
  free(tm.name);
  free(tm.patch_maps);
}

void texture_maps_free(texture_map *texture_maps, int len_texture_maps) {
  for (int i = 0; i < len_texture_maps; i++) {
    texture_map_free(texture_maps[i]);
  }
  free(texture_maps);
}

texture_map *get_texture_maps(FILE *f, lump *directory, header *header,
                              int *len_texture_maps) {
  const int TEXTURE1_lump_index =
      get_lump_index(directory, "TEXTURE1", header->lump_count);
  lump TEXTURE1_lump = directory[TEXTURE1_lump_index];
  int offset = TEXTURE1_lump.lump_offset;
  texture_header th = read_texture_header(f, offset);
  *len_texture_maps = th.num_textures;
  texture_map *texture_maps = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    texture_maps[i] = th.texture_maps[i];
  }
  free(th.texture_map_offsets);
  free(th.texture_maps);
  return texture_maps;
}
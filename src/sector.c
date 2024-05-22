#include "../include/sector.h"
#include <stdio.h>

sector read_sector(FILE *f, int offset, flat *flats, int len_flats) {
  sector s;
  s.floor_height = read_i16(f, offset);
  s.ceiling_height = read_i16(f, offset + 2);
  // if (s.floor_height == s.ceiling_height) {
  //   DOORS_COUNT++;
  // }
  char *floor_texture = read_string(f, offset + 4, 8);
  char *ceiling_texture = read_string(f, offset + 12, 8);
  char *uppercased_floor_texture = malloc(9 * sizeof(char));
  char *uppercased_ceiling_texture = malloc(9 * sizeof(char));
  strtoupper(uppercased_floor_texture, floor_texture);
  strtoupper(uppercased_ceiling_texture, ceiling_texture);
  free(floor_texture);
  free(ceiling_texture);
  s.hash_ceiling = ElfHash(uppercased_ceiling_texture);
  s.hash_floor = ElfHash(uppercased_floor_texture);
  s.floor_texture =
      get_flat_from_name(flats, len_flats, uppercased_floor_texture);
  s.ceiling_texture =
      get_flat_from_name(flats, len_flats, uppercased_ceiling_texture);
  s.light_level = read_i16(f, offset + 20);
  s.type = read_i16(f, offset + 22);
  s.tag_number = read_i16(f, offset + 24);
  free(uppercased_floor_texture);
  free(uppercased_ceiling_texture);
  s.center_pos = (vec2){.x = 0,.y = 0};
  return s;
}

sector *get_sectors_from_lump(FILE *f, lump *directory, int lump_index,
                              int num_bytes, int header_length, int len_sectors,
                              flat *flats, int len_flats) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  sector *sectors = malloc(sizeof(sector) * len_sectors);
  for (int i = 0; i < len_sectors; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sectors[i] = read_sector(f, offset, flats, len_flats);
  }
  return sectors;
}

void sectors_free(sector *sectors) { free(sectors); }
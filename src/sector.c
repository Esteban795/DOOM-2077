#include "../include/sector.h"

sector read_sector(FILE *f, int offset) {
  sector s;
  s.floor_height = read_i16(f, offset);
  s.ceiling_height = read_i16(f, offset + 2);
  s.floor_texture = read_string(f, offset + 4, 8);
  s.ceiling_texture = read_string(f, offset + 12, 8);
  s.light_level = read_i16(f, offset + 20);
  s.type = read_i16(f, offset + 22);
  s.tag_number = read_i16(f, offset + 24);
  s.hash_ceiling = ElfHash(s.ceiling_texture);
  s.hash_floor = ElfHash(s.floor_texture);
  return s;
}

sector *get_sectors_from_lump(FILE *f, lump *directory, int lump_index,
                              int num_bytes, int header_length,
                              int len_sectors) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  sector *sectors = malloc(sizeof(sector) * len_sectors);
  for (int i = 0; i < len_sectors; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sectors[i] = read_sector(f, offset);
  }
  return sectors;
}

void sectors_free(sector *sectors, int len_sectors) {
  for (int i = 0; i < len_sectors; i++) {
    free(sectors[i].floor_texture);
    free(sectors[i].ceiling_texture);
  }
  free(sectors);
}
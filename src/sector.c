#include "../include/sector.h"
#include <stdio.h>

int DOORS_COUNT = 0;

sector read_sector(FILE *f, int offset,flat* flats,int len_flats) {
  sector s;
  s.floor_height = read_i16(f, offset);
  s.ceiling_height = read_i16(f, offset + 2);
  if (s.floor_height == s.ceiling_height) {
    DOORS_COUNT++;
  }
  char* floor_texture = read_string(f, offset + 4, 8);
  char* ceiling_texture = read_string(f, offset + 12, 8);
  s.hash_ceiling = ElfHash(ceiling_texture);
  s.hash_floor = ElfHash(floor_texture);
  s.floor_texture = get_flat_from_name(flats,len_flats, floor_texture);
  s.ceiling_texture = get_flat_from_name(flats,len_flats, ceiling_texture);
  s.light_level = read_i16(f, offset + 20);
  s.type = read_i16(f, offset + 22);
  s.tag_number = read_i16(f, offset + 24);
  free(floor_texture); //unused and still accessible through s.floor_texture.name
  free(ceiling_texture);
  return s;
}

sector *get_sectors_from_lump(FILE *f, lump *directory, int lump_index,
                              int num_bytes, int header_length, int len_sectors,
                              flat *flats, int len_flats){
  int offset = 0;
  lump lump_info = directory[lump_index];
  sector *sectors = malloc(sizeof(sector) * len_sectors);
  for (int i = 0; i < len_sectors; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sectors[i] = read_sector(f, offset,flats,len_flats);
  }
  return sectors;
}

void sectors_free(sector *sectors) {
  free(sectors);
}
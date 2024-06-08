#include "../../include/WAD/color.h"
#include <stdio.h>

color read_color(FILE* f,int offset) {
  color c;
  c.r = (int)read_1_byte(f, offset);
  c.g = (int)read_1_byte(f, offset + 1);
  c.b = (int)read_1_byte(f, offset + 2);
  c.a = 255;
  return c;
}

color* get_color_palette_from_lump(FILE* file, lump* directory,int lump_index,int num_bytes,int header_length) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  int count = lump_info.lump_size / num_bytes;
  color* palette = malloc(count * sizeof(color));
  for (int i = 0; i < count; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    palette[i] = read_color(file, offset);
  }
  return palette;
}
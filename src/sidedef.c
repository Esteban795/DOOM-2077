#include "../include/sidedef.h"

sidedef read_sidedef(FILE *f, int offset) {
  sidedef s;
  s.x_offset = read_i16(f, offset);
  s.y_offset = read_i16(f, offset + 2);
  read_texture_name(f, offset + 4, s.upper_texture);
  read_texture_name(f, offset + 12, s.lower_texture);
  read_texture_name(f, offset + 20, s.middle_texture);
  s.sector = read_i16(f, offset + 28);
  return s;
}

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  sidedef *sidedefs = malloc(sizeof(sidedef) * len_sidedefs);
  for (int i = 0; i < len_sidedefs; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sidedefs[i] = read_sidedef(f, offset);
  }
  return sidedefs;
}

void sidedef_free(sidedef *s) { free(s); }
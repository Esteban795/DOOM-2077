#include "../include/sidedef.h"

sidedef read_sidedef(FILE *f, int offset,sector* sectors) {
  sidedef s;
  s.x_offset = read_i16(f, offset);
  s.y_offset = read_i16(f, offset + 2);
  s.upper_texture = read_string(f, offset + 4, 8);
  s.lower_texture = read_string(f, offset + 12, 8);
  s.middle_texture = read_string(f, offset + 20, 8);
  s.sector_id = read_i16(f, offset + 28);
  s.sector = &sectors[s.sector_id];
  return s;
}

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs,sector* sectors) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  sidedef *sidedefs = malloc(sizeof(sidedef) * len_sidedefs);
  for (int i = 0; i < len_sidedefs; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sidedefs[i] = read_sidedef(f, offset,sectors);
  }
  return sidedefs;
}

void sidedefs_free(sidedef *sidedefs, int len_sidedefs) {
  for (int i = 0; i < len_sidedefs; i++) {
    free(sidedefs[i].upper_texture);
    free(sidedefs[i].lower_texture);
    free(sidedefs[i].middle_texture);
  }
  free(sidedefs);
}
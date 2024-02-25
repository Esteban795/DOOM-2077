#include "../include/subsector.h"

subsector read_subsector(FILE *f, int offset) {
  subsector s;
  s.num_segs = read_i16(f, offset);
  s.first_seg_id = read_i16(f, offset + 2);
  return s;
}

subsector *get_subsectors_from_lump(FILE *f, lump *directory, int lump_index,
                                    int num_bytes, int header_length,
                                    int len_subsectors) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  subsector *subsectors = malloc(sizeof(subsector) * len_subsectors);
  for (int i = 0; i < len_subsectors; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    subsectors[i] = read_subsector(f, offset);
  }
  return subsectors;
}
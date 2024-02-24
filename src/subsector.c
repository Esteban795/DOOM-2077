#include "../include/subsector.h"

subsector read_subsector(FILE *f, int offset, segment *segments) {
  subsector s;
  s.num_segs = read_i16(f, offset);
  s.segs = malloc(sizeof(segment) * s.num_segs);
  i16 first_id = read_i16(f, offset + 2);
  for (i16 i = 0; i < s.num_segs; i++) {
    s.segs[i] = segments[first_id + i];
  }
  return s;
}

subsector *get_subsectors_from_lump(FILE *f, lump *directory, int lump_index,
                                    int num_bytes, int header_length,
                                    int len_subsectors, segment *segments) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  subsector *subsectors = malloc(sizeof(subsector) * len_subsectors);
  for (int i = 0; i < len_subsectors; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    subsectors[i] = read_subsector(f, offset, segments);
  }
  return subsectors;
}

void subsectors_free(subsector *s, int len) {
  for (int i = 0; i < len; i++) {
    free(s[i].segs);
  }
  free(s);
}
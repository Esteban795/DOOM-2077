#include "../include/linedef.h"

linedef read_linedef(FILE *f, int offset) {
  linedef line = {.start_vertex_id = read_u16(f, offset),
                  .end_vertex_id = read_u16(f, offset + 2),
                  .flag = read_u16(f, offset + 4),
                  .line_type = read_u16(f, offset + 6),
                  .sector_tag = read_u16(f, offset + 8),
                  .front_sidedef_id = read_u16(f, offset + 10),
                  .back_sidedef_id = read_u16(f, offset + 12)};
  return line;
}

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs) {
  lump lump_info = directory[lump_index];
  linedef *linedefs = malloc(sizeof(linedef) * len_linedefs);
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset);
  }
  return linedefs;
}

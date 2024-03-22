#include "../include/linedef.h"

vertex* get_vertex_from_linedef(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

sidedef* get_sidedef_from_linedef(i16 sidedef_id, sidedef *sidedefs) {
  return &sidedefs[sidedef_id];
}

linedef read_linedef(FILE *f, int offset, vertex *vertexes,sidedef *sidedefs){
  linedef line;
  line.start_vertex = get_vertex_from_linedef(read_i16(f, offset), vertexes);
  line.end_vertex = get_vertex_from_linedef(read_i16(f, offset + 2), vertexes);
  line.flag = read_i16(f, offset + 4);
  line.line_type = read_i16(f, offset + 6);
  line.sector_tag = read_i16(f, offset + 8);
  i16 front_sidedef_id = read_i16(f, offset + 10);
  i16 back_sidedef_id = read_i16(f, offset + 12);
  line.has_back_sidedef = back_sidedef_id != -1;
  line.front_sidedef = get_sidedef_from_linedef(front_sidedef_id, sidedefs);
  if (line.has_back_sidedef) line.back_sidedef = get_sidedef_from_linedef(back_sidedef_id, sidedefs);
  return line;
}

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes,sidedef *sidedefs) {
  lump lump_info = directory[lump_index];
  linedef *linedefs = malloc(sizeof(linedef) * len_linedefs);
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset, vertexes,sidedefs);
  }
  return linedefs;
}

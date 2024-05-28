#include "../include/segment.h"


// because floats weren't really good back in the days, they used integers to represent angles with a specific formula called
// Binary Angle Measurement (BAM) which is a fixed-point representation of angles 
double bams_to_degrees(i16 bams) {
  int bitshift = 1 << 16;
  double res = (bams * bitshift) * 8.38190317e-8;
  return res < 0 ? 360 + res : res;
}


sector *get_sector_from_id(i16 sector_id, sector *sectors) {
  return &sectors[sector_id];
}

vertex *get_vertex_from_id(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

segment read_segment(FILE *f, int offset, vertex *vertexes, linedef **linedefs,
                     sector *sectors) {
  segment s;
  s.start_vertex = get_vertex_from_id(read_i16(f, offset), vertexes);
  s.end_vertex = get_vertex_from_id(read_i16(f, offset + 2), vertexes);
  s.angle = bams_to_degrees(read_i16(f, offset + 4));
  s.linedef = linedefs[read_i16(f, offset + 6)];
  s.direction = read_i16(f, offset + 8);
  s.offset = read_i16(f, offset + 10);
  s.angle = s.angle < 0 ? 360.0 + s.angle : s.angle;
  if (s.direction) {
    sidedef *front_sidedef = s.linedef->back_sidedef;
    sidedef *back_sidedef = s.linedef->front_sidedef;
    s.front_sector = get_sector_from_id(front_sidedef->sector_id, sectors);
    s.back_sector = s.linedef->flag & TWO_SIDED
                        ? get_sector_from_id(back_sidedef->sector_id, sectors)
                        : NULL;
  } else {
    sidedef *front_sidedef = s.linedef->front_sidedef;
    sidedef *back_sidedef = s.linedef->back_sidedef;
    s.front_sector = get_sector_from_id(front_sidedef->sector_id, sectors);
    s.back_sector = s.linedef->flag & TWO_SIDED
                        ? get_sector_from_id(back_sidedef->sector_id, sectors)
                        : NULL;
  }
  return s;
}

segment *get_segments_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_segments, vertex *vertexes,
                                linedef **linedefs, sector *sectors) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  segment *segments = malloc(sizeof(segment) * len_segments);
  for (int i = 0; i < len_segments; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    segments[i] = read_segment(f, offset, vertexes, linedefs, sectors);
  }
  return segments;
}

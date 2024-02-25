#ifndef SEGMENT_H
#define SEGMENT_H

#include "byte_reader.h"
#include "lump.h"
// https://doomwiki.org/wiki/Seg

struct Segment {
  i16 start_vertex_id;
  i16 end_vertex_id;
  double angle;
  i16 linedef_id;
  i16 direction;
  i16 offset;
};

typedef struct Segment segment;

segment read_segment(FILE *f, int offset);

segment *get_segments_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_segments);

#endif
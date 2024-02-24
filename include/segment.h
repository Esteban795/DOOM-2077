#ifndef SEGMENT_H
#define SEGMENT_H

#include "byte_reader.h"
#include "linedef.h"
#include "lump.h"
#include "vertex.h"
// https://doomwiki.org/wiki/Seg

struct Segment {
  vertex start_vertex;
  vertex end_vertex;
  double angle;
  linedef linedef;
  i16 direction;
  i16 offset;
};

typedef struct Segment segment;

segment read_segment(FILE *f, int offset, vertex *vertexes, linedef *linedefs);

segment *get_segments_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_segments, vertex *vertexes,
                                linedef *linedefs);

#endif
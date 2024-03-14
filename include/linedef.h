#ifndef LINEDEF_H
#define LINEDEF_H

#include "byte_reader.h"
#include "lump.h"
#include "vertex.h"
#include "sidedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


enum LinedefFlags {
  BLOCKING = 1,
  BLOCK_MONSTERS = 2,
  TWO_SIDED = 4,
  UPPER_UNPEGGED = 8,
  LOWER_UNPEGGED = 16,
  SECRET = 32,
  BLOCK_SOUND = 64,
  DONT_DRAW = 128,
  MAPPED = 256,
};

struct Linedef {
  vertex* start_vertex;
  vertex* end_vertex;
  u16 flag;
  u16 line_type;
  u16 sector_tag;
  sidedef* front_sidedef;
  sidedef* back_sidedef;
  bool has_back_sidedef;
};

typedef struct Linedef linedef;

linedef read_linedef(FILE *f, int offset, vertex *vertexes,sidedef *sidedefs);

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes,sidedef *sidedefs);
#endif

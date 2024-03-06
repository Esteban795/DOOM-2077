#ifndef LINEDEF_H
#define LINEDEF_H

#include "byte_reader.h"
#include "lump.h"
#include "vertex.h"
#include <stdio.h>
#include <stdlib.h>

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
  i16 start_vertex_id;
  i16 end_vertex_id;
  u16 flag;
  u16 line_type;
  u16 sector_tag;
  u16 front_sidedef_id;
  u16 back_sidedef_id;
};

typedef struct Linedef linedef;

linedef read_linedef(FILE *f, int offset);

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs);
#endif

#ifndef LINEDEF_H
#define LINEDEF_H

#include "byte_reader.h"
#include "lump.h"
#include "vertex.h"
#include <stdio.h>
#include <stdlib.h>

struct Linedef {
  vertex start_vertex;
  vertex end_vertex;
  u16 flag;
  u16 line_type;
  u16 sector_tag;
  u16 front_sidedef_id;
  u16 back_sidedef_id;
};

typedef struct Linedef linedef;

linedef read_linedef(FILE *f, int offset, vertex *vertexes);

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes);
#endif
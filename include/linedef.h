#ifndef LINEDEF_H
#define LINEDEF_H

#include "byte_reader.h"
#include "door.h"
#include "lump.h"
#include "sidedef.h"
#include "vertex.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// https://doomwiki.org/wiki/Linedef_type#Door_linedef_types

// OWC = open wait close
// OSO = open stay open
// CSC = close stay closed
// CWO = close wait open
// S = slow
// F = fast
// number = seconds it waits
enum LinedefDoorTypes {
  OWC_S_4 = 1,
  OWC_F_4 = 117,
  OSO_S = 31,
  OSO_F = 118,
  CSC_S = 42,
  CSC_F = 116,
  CWO_S = 196,
  DRK = 28,
  ACCEPTED = 46
};

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
  vertex *start_vertex;
  vertex *end_vertex;
  u16 flag;
  u16 line_type;
  u16 sector_tag;
  sidedef *front_sidedef;
  sidedef *back_sidedef;
  bool has_back_sidedef;
  door *door;
};

typedef struct Linedef linedef;

struct TupleIndex {
  int index1;
  int index2;
};

typedef struct TupleIndex tuple_index;

linedef* read_linedef(FILE *f, int offset, vertex *vertexes, sidedef *sidedefs);

linedef **get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes,
                                sidedef *sidedefs, int len_sectors);

door **get_doors(linedef **linedefs, int len_linedefs, int *doors_count,
                 int len_sectors);
                 
void linedefs_free(linedef **linedefs, int len_linedefs);
#endif

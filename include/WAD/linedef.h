#ifndef LINEDEF_H
#define LINEDEF_H

#include "../core/door.h"
#include "../core/lift.h"
#include "../util/byte_reader.h"
#include "lump.h"
#include "sidedef.h"

#include "vertex.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// https://doomwiki.org/wiki/Linedef_type#Door_linedef_types

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
  /// used to link a linedef to an action
  u16 sector_tag;
  sidedef *front_sidedef;
  sidedef *back_sidedef;
  bool has_back_sidedef;
  /// used to check if the linedef has been used in the game, for one time stuff
  bool used;
  bool is_repeatable;
  bool is_shootable;
  bool is_pushable;
  bool is_collidable;

  door *door;
  bool has_doors;
  lift *lifts;
  bool has_lifts;
};

typedef struct Linedef linedef;

linedef **get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                 int num_bytes, int header_length,
                                 int len_linedefs, vertex *vertexes,
                                 sidedef *sidedefs);

lift **get_lifts(linedef **linedefs, int len_linedefs, int *lifts_count,
                 sector *sectors, int len_sectors);

door **get_doors(linedef **linedefs, int len_linedefs, int *doors_count,
                 sector *sectors, int len_sectors);

void linedefs_free(linedef **linedefs, int len_linedefs);

void set_sectors_centers(linedef **linedefs, int len_linedefs, sector *sectors,
                         int len_sectors);
#endif

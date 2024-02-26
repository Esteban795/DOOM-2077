#ifndef SIDEDEF_H
#define SIDEDEF_H

#include "byte_reader.h"
#include "lump.h"
#include "sector.h"

struct Sidedef {
  i16 x_offset;
  i16 y_offset;
  char* upper_texture;
  char* lower_texture;
  char* middle_texture;
  sector sector;
};

typedef struct Sidedef sidedef;

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs,sector* sectors);
#endif
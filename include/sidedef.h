#ifndef SIDEDEF_H
#define SIDEDEF_H

#include "byte_reader.h"
#include "lump.h"
struct Sidedef {
  i16 x_offset;
  i16 y_offset;
  int8_t upper_texture[8];
  int8_t lower_texture[8];
  int8_t middle_texture[8];
  i16 sector;
};

typedef struct Sidedef sidedef;

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs);
#endif
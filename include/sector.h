#ifndef SECTOR_H
#define SECTOR_H

#include "byte_reader.h"
#include "lump.h"
#include <stdint.h>

struct Sector {
  i16 floor_height;
  i16 ceiling_height;
  int8_t floor_texture[8];
  int8_t ceiling_texture[8];
  i16 light_level;
  i16 type;
  i16 tag;
  i16 tag_number;
};

typedef struct Sector sector;

sector *get_sectors_from_lump(FILE *f, lump *directory, int lump_index,
                              int num_bytes, int header_length,
                              int len_sectors);
#endif
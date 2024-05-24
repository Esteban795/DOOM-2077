#ifndef SECTOR_H
#define SECTOR_H

#include "byte_reader.h"
#include "flat.h"
#include "lump.h"
#include "util.h"
#include <stdint.h>
#include "vec2.h"

struct Sector {
  i16 floor_height;
  i16 ceiling_height;
  flat *floor_texture;
  flat *ceiling_texture;
  i16 light_level;
  i16 type;
  i16 tag;
  i16 tag_number;
  unsigned long hash_floor;
  unsigned long hash_ceiling;
  vec2 center_pos;
};

typedef struct Sector sector;

sector *get_sectors_from_lump(FILE *f, lump *directory, int lump_index,
                              int num_bytes, int header_length, int len_sectors,
                              flat *flats, int len_flats);

void sectors_free(sector *sectors);
#endif

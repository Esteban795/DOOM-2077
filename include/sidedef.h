#ifndef SIDEDEF_H
#define SIDEDEF_H

#include "byte_reader.h"
#include "lump.h"
#include "sector.h"
#include "texture.h"
#include "util.h"
#include <assert.h>

struct Sidedef {
  i16 x_offset;
  i16 y_offset;
  texture_map *upper_texture;
  texture_map *lower_texture;
  texture_map *middle_texture;
  sector *sector;
  i16 sector_id;
  unsigned long hash_upper;
  unsigned long hash_lower;
  unsigned long hash_middle;
};

typedef struct Sidedef sidedef;

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs, sector *sectors,
                                texture_map *textures, int len_textures);

void sidedefs_free(sidedef *sidedefs);
#endif

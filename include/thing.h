#ifndef THING_H
#define THING_H

#include "byte_reader.h"
#include "lump.h"
// https://doomwiki.org/wiki/Thing

struct Thing {
  i16 x;
  i16 y;
  i16 angle;
  i16 type;
  i16 flags;
};

typedef struct Thing thing;

thing *get_things_from_lump(FILE *f, lump *directory, int lump_index,
                            int num_bytes, int header_length, int len_things);
#endif
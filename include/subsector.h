#ifndef SUBSECTOR_H
#define SUBSECTOR_H

#include "byte_reader.h"
#include "lump.h"

// https://doomwiki.org/wiki/Subsector

struct Subsector {
  i16 num_segs;
  i16 first_seg_id;
};

typedef struct Subsector subsector;

subsector *get_subsectors_from_lump(FILE *f, lump *directory, int lump_index,
                                    int num_bytes, int header_length,
                                    int len_subsectors);
#endif
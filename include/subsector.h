#ifndef SUBSECTOR_H
#define SUBSECTOR_H

#include "byte_reader.h"
#include "lump.h"
#include "segment.h"

// https://doomwiki.org/wiki/Subsector

struct Subsector {
  i16 num_segs;
  segment *segs;
};

typedef struct Subsector subsector;

subsector *get_subsectors_from_lump(FILE *f, lump *directory, int lump_index,
                                    int num_bytes, int header_length,
                                    int len_subsectors, segment *segments);

void subsectors_free(subsector *s, int len);
#endif
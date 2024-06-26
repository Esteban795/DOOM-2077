#ifndef HEADER_H
#define HEADER_H

#include "../util/byte_reader.h"
#include <stdio.h>

/// Ref: https://doom.fandom.com/wiki/WAD

struct Header {
  char *wad_type;
  i32 lump_count;
  i32 init_offset;
};

typedef struct Header header;

header read_header(FILE *f);
#endif
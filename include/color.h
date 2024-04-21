#ifndef COLOR_H
#define COLOR_H

#include "util.h"
#include "lump.h"

#define COLOR_PALETTE_SIZE 256

struct Color {
  int r;
  int g;
  int b;
  int a;
};

typedef struct Color color;

color get_color(color* cp,unsigned long text_hash, i16 light_level);

color* get_color_palette_from_lump(FILE* file, lump* directory,int lump_index,int num_bytes,int header_length);
#endif
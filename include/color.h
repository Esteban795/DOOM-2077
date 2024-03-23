#ifndef COLOR_H
#define COLOR_H

#include "util.h"

struct Color {
  int r;
  int g;
  int b;
  int a;
};

typedef struct Color color;

color get_color(unsigned long text_hash, i16 light_level);
#endif
#ifndef FLAT_H
#define FLAT_H

#include "byte_reader.h"
#include "patch.h"

/// Ref: https://doomwiki.org/wiki/Flat

struct Flat {
  char *name;
  int width;
  int height;
  Uint32 *pixels;
  SDL_PixelFormat *format;
};

typedef struct Flat flat;

flat *get_flats(FILE *f, lump *directory, header *header, color *palette,
                int *len_flats);

void flats_free(flat *flats, int len_flats);

flat *get_flat_from_name(flat *flats, int len_flats, char *name);
#endif
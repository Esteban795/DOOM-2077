#ifndef FLAT_H
#define FLAT_H

#include "byte_reader.h"
#include "patch.h"

struct Flat {
  char *name;
  int width;
  int height;
  Uint32 *pixels;
  SDL_Texture *flat_img;
};

typedef struct Flat flat;

flat *get_flats(FILE *f, SDL_Renderer *renderer, lump *directory,
                header *header, color *palette, int *len_flats);

void flats_free(flat *flats, int len_flats);

void display_flats(SDL_Renderer *renderer, flat *flats, int len_flats);
#endif
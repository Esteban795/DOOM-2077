#include "../include/flat.h"

void flat_free(flat fl) { // fl.name freed when freeing directory
  free(fl.pixels);
}

void flats_free(flat *flats, int len_flats) {
  for (int i = 0; i < len_flats; i++) {
    flat_free(flats[i]);
  }
  free(flats);
}

flat read_flat(FILE *f, color *palette, char *name, int offset) {
  flat fl;
  fl.width = 64;
  fl.height = 64;
  fl.name = name;
  fl.format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  fl.pixels = malloc(sizeof(Uint32) * 64 * 64);
  SDL_PixelFormat *fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      int color_id = read_u8(f, offset + i * 64 + j);
      color c = palette[color_id];
      Uint32 code_c = SDL_MapRGBA(fmt, c.r, c.g, c.b, 255);
      fl.pixels[i * 64 + j] = code_c;
    }
  }
  return fl;
}

flat *get_flats(FILE *f, lump *directory, header *header, color *palette,
                int *len_flats) {
  const int START_MARKER =
      get_lump_index(directory, "F1_START", header->lump_count);
  const int END_MARKER =
      get_lump_index(directory, "F1_END", header->lump_count);
  *len_flats = END_MARKER - START_MARKER - 1;
  flat *flats = malloc(sizeof(flat) * *len_flats);
  for (int i = 0; i < *len_flats; i++) {
    flats[i] = read_flat(f, palette, directory[START_MARKER + i + 1].lump_name,
                         directory[START_MARKER + i + 1].lump_offset);
  }
  return flats;
}

flat *get_flat_from_name(flat *flats, int len_flats, char *name) {
  for (int i = 0; i < len_flats; i++) {
    if (strcmp(flats[i].name, name) == 0) {
      return &flats[i];
    }
  }
  return NULL;
}
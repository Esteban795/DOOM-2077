#include "../include/flat.h"

void flat_free(flat fl) { // fl.name freed when freeing directory
  free(fl.pixels);
  SDL_DestroyTexture(fl.flat_img);
}

void flats_free(flat *flats, int len_flats) {
  for (int i = 0; i < len_flats; i++) {
    flat_free(flats[i]);
  }
  free(flats);
}

void display_flats(SDL_Renderer *renderer, flat *flats, int len_flats) {
  for (int i = 0; i < len_flats; i++) {
    SDL_RenderCopy(renderer, flats[i].flat_img, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(6);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }
}

flat read_flat(FILE *f, SDL_Renderer *renderer, color *palette, char *name,
               int offset) {
  flat fl;
  fl.width = 64;
  fl.height = 64;
  fl.name = name;
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
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STATIC, 64, 64);
  if (texture == NULL) {
    SDL_Log("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_UpdateTexture(texture, NULL, fl.pixels, 64 * 4);
  fl.flat_img = texture;
  return fl;
}

flat *get_flats(FILE *f, SDL_Renderer *renderer, lump *directory,
                header *header, color *palette, int *len_flats) {
  const int START_MARKER =
      get_lump_index(directory, "F1_START", header->lump_count);
  const int END_MARKER =
      get_lump_index(directory, "F1_END", header->lump_count);
  *len_flats = END_MARKER - START_MARKER - 1;
  flat *flats = malloc(sizeof(flat) * *len_flats);
  for (int i = 0; i < *len_flats; i++) {
    flats[i] = read_flat(f, renderer, palette,
                         directory[START_MARKER + i + 1].lump_name,
                         directory[START_MARKER + i + 1].lump_offset);
  }
  return flats;
}

flat* get_flat_from_name(flat* flats,int len_flats, char* name) {
  for (int i = 0; i < len_flats; i++) {
    if (strcmp(flats[i].name, name) == 0) {
      return &flats[i];
    }
  }
  return NULL;
}
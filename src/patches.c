#include "../include/patches.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdint.h>


patch_header read_patch_header(FILE *f, int offset) {
  patch_header ph;
  ph.width = read_i16(f, offset);
  ph.height = read_i16(f, offset + 2);
  ph.x_offset = read_i16(f, offset + 4);
  ph.y_offset = read_i16(f, offset + 6);
  ph.column_offsets = malloc(sizeof(uint32_t) * ph.width);
  return ph;
}

patch_column read_patch_column(FILE *f, int offset) {
  patch_column pc;
  pc.top_delta = read_u8(f, offset);
  pc.length = read_u8(f, offset + 1);
  pc.padding_pre = read_u8(f, offset + 2);
  pc.data = malloc(sizeof(u8) * pc.length);
  for (int i = 0; i < pc.length; i++) {
    pc.data[i] = read_u8(f, offset + 3 + i);
  }
  pc.padding_post = read_u8(f, offset + 3 + pc.length);
  return pc;
}

patch create_patch(FILE *f, int offset, SDL_Renderer* renderer,header *header, lump *directory,
                   char *patchname,color *palette) {
  patch p;
  p.palette = palette;
  p.header = read_patch_header(f, offset);
  p.columns = malloc(sizeof(patch_column) * p.header.width);
  int patch_index = get_lump_index(directory, patchname, header->lump_count);
  int patch_offset = directory[patch_index].lump_offset;
  int offs = 0;
  for (int i = 0; i < p.header.width; i++) {
    offs = patch_offset + p.header.column_offsets[i];
    while (true) {
      p.columns[i] = read_patch_column(f, offset + 8 + i * 8);
      if (p.columns[i].top_delta == 0xFF) {
        break;
      }
    }
  }
  int format;
  int pitch = 0;
  SDL_PixelFormat fmt;
  fmt.format = SDL_PIXELFORMAT_RGBA8888;
  Uint32* pixels = malloc(sizeof(Uint32) * p.header.width * p.header.height);
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, p.header.width, p.header.height);
  if (SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch) != 0) {
    printf("Error locking texture: %s\n", SDL_GetError());
  }
  SDL_SetRenderTarget(renderer, texture);
  for (int i = 0; i < p.header.width; i++) {
    if (p.columns[i].top_delta == 0xFF) continue; //no pixel in this column
    patch_column pc = p.columns[i];
    for (int j = 0; j < p.columns[i].length; j++) {
      int color_idx =  pc.data[j];
      color c = p.palette[color_idx];
      Uint32 color = SDL_MapRGB(&fmt, c.r, c.g, c.b);
      Uint32 pixel_pos = i * (pitch * sizeof(unsigned int)) + i;
      pixels[pixel_pos] = color;
    }
  }
  SDL_UnlockTexture(texture);
  p.patch_img = texture;
  return p;
}

void free_patch(patch *p) {
  free(p->header.column_offsets);
  for (int i = 0; i < p->header.width; i++) {
    free(p->columns[i].data);
  }
  free(p->columns);
  SDL_DestroyTexture(p->patch_img);
  free(p);
}

void free_patches(patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    free_patch(&patches[i]);
  }
}

void display_patches(SDL_Renderer* renderer, patch* patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    SDL_RenderCopy(renderer, patches[i].patch_img, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
  }
}

patch* get_patches(lump* directory, header* header, FILE* f, color* palette,int* patch_count) {
  int start_patches = get_lump_index(directory, PATCHES_START, header->lump_count);
  int end_patches = get_lump_index(directory, PATCHES_END, header->lump_count);
  *patch_count = end_patches - start_patches;
  patch* patches = malloc(sizeof(patch) * *patch_count);
  for (int i = 0; i < *patch_count; i++) {
    patches[i] = create_patch(f, directory[start_patches + i].lump_offset, NULL, header, directory, directory[start_patches + i].lump_name, palette);
  }
  return patches;
}
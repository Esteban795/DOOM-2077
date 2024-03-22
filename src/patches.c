#include "../include/patches.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

patch_header read_patch_header(FILE *f, int offset) {
  patch_header ph;
  ph.width = read_i16(f, offset);
  ph.height = read_i16(f, offset + 2);
  ph.x_offset = read_i16(f, offset + 4);
  ph.y_offset = read_i16(f, offset + 6);
  ph.column_offsets = malloc(sizeof(u32) * ph.width);
  if (ph.column_offsets == NULL) {
    printf("Error allocating memory for column offsets\n");
    exit(1);
  }
  for (int i = 0; i < ph.width; i++) {
    ph.column_offsets[i] = read_u32(f, offset + 8 + i * 4);
  }
  return ph;
}

patch_column read_patch_column(FILE *f, int column_offset) {
  patch_column pc;
  pc.top_delta = read_u8(f, column_offset);
  if (pc.top_delta == NO_PIXELS) {
    return pc;
  }
  pc.length = read_u8(f, column_offset + 1);
  pc.padding_pre = read_u8(f, column_offset + 2);
  pc.data = malloc(sizeof(u8) * pc.length);
  if (pc.data == NULL) {
    printf("Error allocating memory for patch column data\n");
    exit(1);
  }
  for (int i = 0; i < pc.length; i++) {
    pc.data[i] = read_u8(f, column_offset + 3 + i);
  }
  pc.padding_post = read_u8(f, column_offset + 3 + pc.length);
  return pc;
}

patch create_patch(FILE *f, int patch_offset, SDL_Renderer *renderer,
                   header *header, lump *directory, char *patchname,
                   color *palette) {
  patch p;
  p.palette = palette;
  p.header = read_patch_header(f, patch_offset);
  p.columns = malloc(sizeof(patch_column) * p.header.width);
  p.patchname = patchname;
  int column_offset = 0;
  for (int i = 0; i < p.header.width; i++) {
    column_offset = patch_offset + p.header.column_offsets[i];
    p.columns[i] = read_patch_column(f, column_offset);
    if (p.columns[i].top_delta == NO_PIXELS) {
      continue;
    }
  }
  // int pitch = 0;
  // SDL_PixelFormat fmt;
  // fmt.format = SDL_PIXELFORMAT_RGBA8888;
  // Uint32* pixels = malloc(sizeof(Uint32) * p.header.width * p.header.height);
  // SDL_Texture* texture = SDL_CreateTexture(renderer,
  // SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, p.header.width,
  // p.header.height); if (SDL_LockTexture(texture, NULL, (void**)&pixels,
  // &pitch) != 0) {
  //   printf("Error locking texture: %s\n", SDL_GetError());
  //   exit(1);
  // }
  // SDL_SetRenderTarget(renderer, texture);
  // for (int i = 0; i < p.header.width; i++) {
  //   if (p.columns[i].top_delta == 0xFF) continue; //no pixel in this column
  //   patch_column pc = p.columns[i];
  //   for (int j = 0; j < p.columns[i].length; j++) {
  //     int color_idx =  pc.data[j];
  //     color c = p.palette[color_idx];
  //     Uint32 color = SDL_MapRGB(&fmt, c.r, c.g, c.b);
  //     Uint32 pixel_pos = i * (pitch * sizeof(unsigned int)) + i;
  //     pixels[pixel_pos] = color;
  //   }
  // }
  // SDL_UnlockTexture(texture);
  // p.patch_img = texture;
  return p;
}

void free_patch(patch p) {
  free(p.header.column_offsets);
  for (int i = 0; i < p.header.width; i++) {
    if (p.columns[i].top_delta == NO_PIXELS)
      continue;
    free(p.columns[i].data);
  }
  free(p.columns);
  // SDL_DestroyTexture(p.patch_img);
}

void patches_free(patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    free_patch(patches[i]);
  }
  free(patches);
}

void display_patches(SDL_Renderer *renderer, patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    printf("Displaying patch %d\n", i);
    SDL_RenderCopy(renderer, patches[i].patch_img, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
  }
}

patch *get_patches(SDL_Renderer *renderer, lump *directory, header *header,
                   FILE *f, color *palette, int *patch_count) {
  int start_patches =
      get_lump_index(directory, PATCHES_START, header->lump_count);
  int end_patches = get_lump_index(directory, PATCHES_END, header->lump_count);
  *patch_count = end_patches - start_patches - 1;
  patch *patches = malloc(sizeof(patch) * *patch_count);
  for (int i = 0; i < *patch_count;
       i++) { // skip start_patches because it's only the marker
    patches[i] = create_patch(f, directory[start_patches + i + 1].lump_offset,
                              renderer, header, directory,
                              directory[start_patches + i].lump_name, palette);
  }
  return patches;
}
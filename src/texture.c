#include "../include/texture.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>

SDL_Texture *get_texture_from_patchmaps(texture_map tm, patch *patches,
                                        SDL_Renderer *renderer) {
  Uint32 *pixels = malloc(sizeof(Uint32) * tm.width * tm.height);
  printf("Pixels length %d\n", tm.width * tm.height);
  for (int k = 0; k < tm.patch_count; k++) {
    patch_map pm = tm.patch_maps[k];
    patch p = patches[pm.patch_index];
    printf("Patch index: %d\n", pm.patch_index);
    printf("Patch width: %d, height: %d\n", p.header.width, p.header.height);
    for (int i = 0; i < tm.height; i++){
      for (int j = 0; j < tm.width;j++){
        int x = j + pm.left_offset;
        int y = i * (tm.width + pm.top_offset);
        printf("x: %d, y: %d\n", x, y);
        pixels[x + y] = p.pixels[i * p.header.width + j];
      }
    }
    printf("\n\n\n\n\n");
  }
  SDL_Surface *surface =
      SDL_CreateRGBSurfaceFrom(pixels, tm.width, tm.height, 32, tm.width * 4,
                               0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

texture_map read_texture_map(FILE *f, int offset, patch *patches,
                             SDL_Renderer *renderer) {
  texture_map tm;
  tm.name = read_texture_name(f, offset, 8);
  tm.masked = (bool)read_i32(f, offset + 8);
  tm.width = read_i16(f, offset + 12);
  tm.height = read_i16(f, offset + 14);
  tm.column_dir = read_i32(f, offset + 16);
  tm.patch_count = read_u16(f, offset + 20);
  tm.patch_maps = malloc(sizeof(patch_map) * tm.patch_count);
  for (int i = 0; i < tm.patch_count; i++) {
    tm.patch_maps[i] = read_patch_map(f, offset + 22 + i * 10);
  }
  tm.texture = get_texture_from_patchmaps(tm, patches, renderer);
  return tm;
}

texture_header read_texture_header(FILE *f, int offset, patch *patches,
                                   SDL_Renderer *renderer) {
  texture_header th;
  th.num_textures = read_i32(f, offset);
  th.texture_map_offsets = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    th.texture_map_offsets[i] = read_i32(f, offset + 4 + i * 4);
  }
  th.texture_maps = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    th.texture_maps[i] = read_texture_map(f, offset + th.texture_map_offsets[i],
                                          patches, renderer);
  }
  return th;
}

void texture_map_free(texture_map tm) {
  free(tm.name);
  free(tm.patch_maps);
}

void texture_maps_free(texture_map *texture_maps, int len_texture_maps) {
  for (int i = 0; i < len_texture_maps; i++) {
    texture_map_free(texture_maps[i]);
  }
  free(texture_maps);
}

void display_texture_maps(SDL_Renderer *renderer, texture_map *texture_maps,
                          int len_texture_maps) {
  for (int i = 0; i < len_texture_maps; i++) {
    if (texture_maps[i].texture == NULL) {
      printf("No texture associated with patch %d\n", i);
      continue;
    }
    SDL_RenderCopy(renderer, texture_maps[i].texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }
}

texture_map *get_texture_maps(FILE *f, lump *directory, header *header,
                              patch *patches, SDL_Renderer *renderer,
                              int *len_texture_maps) {
  const int TEXTURE1_lump_index =
      get_lump_index(directory, "TEXTURE1", header->lump_count);
  lump TEXTURE1_lump = directory[TEXTURE1_lump_index];
  int offset = TEXTURE1_lump.lump_offset;
  texture_header th = read_texture_header(f, offset, patches, renderer);
  *len_texture_maps = th.num_textures;
  texture_map *texture_maps = malloc(sizeof(texture_map) * th.num_textures);
  for (u32 i = 0; i < th.num_textures; i++) {
    texture_maps[i] = th.texture_maps[i];
  }
  free(th.texture_map_offsets);
  free(th.texture_maps);
  return texture_maps;
}
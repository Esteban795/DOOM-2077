#include "../include/patches.h"

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

patch_column read_patch_column(FILE *f, int column_offset, int *new_offset) {
  patch_column pc;
  pc.top_delta = read_u8(f, column_offset);
  if (pc.top_delta == NO_PIXELS) {
    *new_offset = column_offset + 1;
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
  *new_offset = column_offset + 4 + pc.length;
  return pc;
}

Uint32 *transform_to_row_based(Uint32 *pixels, int width, int height) {
  Uint32 *row_based = malloc(sizeof(Uint32) * width * height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      row_based[i * width + j] = pixels[j * height + i];
    }
  }
  return row_based;
}

int find_nb_of_columns(FILE* f,patch_header ph, int offset){
  int actual_number_of_columns = 0;
  int cln_offset = 0;
  for (int i = 0; i < ph.width; i++) {
    cln_offset = offset + ph.column_offsets[i];
    while (true) {
      patch_column pc = read_patch_column(f, cln_offset, &cln_offset);
      actual_number_of_columns++;
      if (pc.top_delta == NO_PIXELS) {
        break;
      }
      free(pc.data);
    }
  }
  return actual_number_of_columns;
}

patch_column* read_patch_columns(FILE* f, patch_header ph, int offset, int actual_number_of_columns){
  patch_column* columns = malloc(sizeof(patch_column) * actual_number_of_columns);
  int column_offset = 0;
  int arr_offset = 0;
  for (int i = 0; i < ph.width; i++) {
    column_offset = offset + ph.column_offsets[i];
    while (true) {
      patch_column pc = read_patch_column(f, column_offset, &column_offset);
      columns[arr_offset] = pc;
      arr_offset++;
      if (pc.top_delta == NO_PIXELS) {
        break;
      }
    }
  }
  return columns;
}

SDL_Texture* get_texture_from_patch(SDL_Renderer* renderer,patch p){
  int pitch = p.header.width * 4;
  int ix = 0;
  int color_idx;
  SDL_PixelFormat *fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 *pixels = malloc(sizeof(Uint32) * p.header.width * p.header.height);
  memset(pixels,TRANSPARENT_COLOR,p.header.width * p.header.height * sizeof(Uint32));
  color c;
  Uint32 code_c;
  for (int j = 0; j < p.nb_columns; j++) {
    patch_column column = p.columns[j];
    if (column.top_delta == NO_PIXELS) {
      ix++;
      continue;
    }
    for (int iy = 0; iy < column.length; iy++) {
      color_idx = column.data[iy];
      c = p.palette[color_idx];
      code_c = SDL_MapRGBA(fmt, c.r, c.g, c.b, 255);
      pixels[ix * p.header.height + iy + column.top_delta] = code_c;
    }
  }
  Uint32 *row_based =
      transform_to_row_based(pixels, p.header.width, p.header.height);
  free(pixels);
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STATIC,
                                           p.header.width, p.header.height);
  if (texture == NULL) {
    SDL_Log("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_UpdateTexture(texture, NULL, row_based, pitch);
  free(row_based);
  return texture;
}

patch create_patch(FILE *f, int patch_offset, SDL_Renderer *renderer,
                   char *patchname, color *palette) {
  patch p;
  p.patchname = patchname;
  p.palette = palette;
  p.header = read_patch_header(f, patch_offset);
  int actual_number_of_columns = find_nb_of_columns(f, p.header, patch_offset);
  p.nb_columns = actual_number_of_columns;
  p.columns = read_patch_columns(f, p.header, patch_offset, actual_number_of_columns);
  p.patch_img = get_texture_from_patch(renderer,p);
  return p;
}

void free_patch(patch p) {
  free(p.header.column_offsets);
  for (int i = 0; i < p.nb_columns; i++) {
    if (p.columns[i].top_delta == NO_PIXELS) continue;  // nothing was actually allocated
    free(p.columns[i].data);
  }
  free(p.columns);
  SDL_DestroyTexture(p.patch_img);
}

void patches_free(patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    free_patch(patches[i]);
  }
  free(patches);
}

void display_patches(SDL_Renderer *renderer, patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    if (patches[i].patch_img == NULL) {
      printf("No texture associated with patch %d\n", i);
      continue;
    }
    SDL_RenderCopy(renderer, patches[i].patch_img, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(6);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
  }
}

void print_patch_column(patch_column pc) {
  printf("Top delta: %d\n", pc.top_delta);
  printf("Length: %d\n", pc.length);
  printf("Padding pre: %d\n", pc.padding_pre);
  printf("Data: [");
  for (int i = 0; i < pc.length; i++) {
    if (i != 0)
      printf(", ");
    printf("%d", pc.data[i]);
  }
  printf("]\n");
  printf("Padding post: %d\n", pc.padding_post);
}

void print_patch(patch p) {
  printf("Patch %s\n", p.patchname);
  printf("Width: %d\n", p.header.width);
  printf("Height: %d\n", p.header.height);
  printf("X Offset: %d\n", p.header.x_offset);
  printf("Y Offset: %d\n", p.header.y_offset);
  for (int i = 0; i < p.header.width; i++) {
    printf("[");
    for (int j = 0; j < p.columns[i].length; j++) {
      if (j != 0)
        printf(", ");
      printf("%d", p.columns[i].data[j]);
    }
    printf("]\n");
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
    patches[i] =
        create_patch(f, directory[start_patches + i + 1].lump_offset, renderer,
                     directory[start_patches + i + 1].lump_name, palette);
  }
  return patches;
}
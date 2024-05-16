#include "../include/patch.h"

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

patch_map read_patch_map(FILE *f, int offset) {
  patch_map pm;
  pm.left_offset = read_i16(f, offset);
  pm.top_offset = read_i16(f, offset + 2);
  pm.patch_index = read_u16(f, offset + 4);
  pm.step_dir = 0;  // both are unused so we don't care
  pm.color_map = 0; //
  return pm;
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
  free(pixels);
  return row_based;
}

int find_nb_of_columns(FILE *f, patch_header ph, int offset) {
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

patch_column *read_patch_columns(FILE *f, patch_header ph, int offset,
                                 int actual_number_of_columns) {
  patch_column *columns =
      malloc(sizeof(patch_column) * actual_number_of_columns);
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

Uint32 *get_pixels_from_patch(SDL_Renderer *renderer, patch p) {
  int ix = 0;
  int color_idx;
  SDL_PixelFormat *fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 *pixels = malloc(sizeof(Uint32) * p.header.width * p.header.height);
  memset(pixels, TRANSPARENT_COLOR,
         p.header.width * p.header.height * sizeof(Uint32));
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
  return row_based;
}

SDL_Texture *get_texture_from_pixels(SDL_Renderer *renderer, Uint32 *pixels,
                                     int width, int height) {
  SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_STATIC, width, height);
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  SDL_UpdateTexture(tex, NULL, pixels, width * sizeof(Uint32));
  return tex;
}

patch create_patch(FILE *f, SDL_Renderer *renderer, int patch_offset,
                   char *patchname, color *palette) {
  patch p;
  p.patchname = patchname;
  p.palette = palette;
  p.header = read_patch_header(f, patch_offset);
  int actual_number_of_columns = find_nb_of_columns(f, p.header, patch_offset);
  p.nb_columns = actual_number_of_columns;
  p.columns =
      read_patch_columns(f, p.header, patch_offset, actual_number_of_columns);
  Uint32* pixels = get_pixels_from_patch(renderer, p);
  p.pixels = malloc(sizeof(Uint32) * p.header.width * p.header.height);
  memcpy(p.pixels, pixels, sizeof(Uint32) * p.header.width * p.header.height);
  p.tex = get_texture_from_pixels(renderer, p.pixels, p.header.width, p.header.height);
  free(pixels);
  return p;
}

void sprite_free(patch p) {
  free(p.header.column_offsets);
  for (int i = 0; i < p.nb_columns; i++) {
    if (p.columns[i].top_delta == NO_PIXELS)
      continue; // nothing was actually allocated
    free(p.columns[i].data);
  }
  free(p.pixels);
  free(p.columns);
}

void sprites_free(patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    sprite_free(patches[i]);
  }
  free(patches);
}

void texture_patch_free(patch p) {
  free(p.header.column_offsets);
  for (int i = 0; i < p.nb_columns; i++) {
    if (p.columns[i].top_delta == NO_PIXELS)
      continue; // nothing was actually allocated
    free(p.columns[i].data);
  }
  free(p.pixels);
  free(p.columns);
  free(p.patchname);
}

void textures_patches_free(patch *patches, int patch_count) {
  for (int i = 0; i < patch_count; i++) {
    texture_patch_free(patches[i]);
  }
  free(patches);
}

patch *get_sprites(SDL_Renderer *renderer, lump *directory, header *header,
                   FILE *f, color *palette, int *patch_count) {
  int start_patches =
      get_lump_index(directory, PATCHES_START, header->lump_count);
  int end_patches = get_lump_index(directory, PATCHES_END, header->lump_count);
  *patch_count = end_patches - start_patches - 1;
  patch *patches = malloc(sizeof(patch) * *patch_count);
  for (int i = 0; i < *patch_count;
       i++) { // skip start_patches because it's only the marker
    patches[i] =
        create_patch(f, renderer, directory[start_patches + i + 1].lump_offset,
                     directory[start_patches + i + 1].lump_name, palette);
  }
  return patches;
}

patch *get_texture_patches(SDL_Renderer *rendererer, lump *directory,
                           header *header, FILE *f, color *palette,
                           int *len_textures_patches) {
  int PNAMES_lump_index =
      get_lump_index(directory, "PNAMES", header->lump_count);
  lump PNAMES_lump = directory[PNAMES_lump_index];
  int offset = PNAMES_lump.lump_offset;
  i32 num_patches = read_i32(f, offset);
  offset += 4;
  *len_textures_patches = (int)num_patches;
  patch *texture_patches = malloc(sizeof(patch) * num_patches);
  for (int i = 0; i < *len_textures_patches; i++) {
    char *patch_name = read_texture_name(f, offset + i * 8, 8);
    char *upper_patch_name = malloc(sizeof(char) * 9);
    strtoupper(upper_patch_name, patch_name);
    free(patch_name);
    i16 patch_index =
        get_lump_index(directory, upper_patch_name, header->lump_count);
    if (patch_index == -1) {
      free(upper_patch_name);
      continue;
    }
    texture_patches[i] =
        create_patch(f, rendererer, directory[patch_index].lump_offset,
                     upper_patch_name, palette);
  }
  return texture_patches;
}

patch *get_patch_from_name(patch *patches, int len_patches, char *name) {
  for (int i = 0; i < len_patches; i++) {
    if (!strcasecmp(patches[i].patchname, name)) {
      return &patches[i];
    }
  }
  return NULL;
}

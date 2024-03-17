#include "../include/patches.h"


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

patch create_patch(FILE *f, int offset, header *header, lump *directory,
                   char *patchname) {
  patch p;
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
  return p;
}
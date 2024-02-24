#include "../include/lump.h"

lump *read_directory(FILE *f, header h) {
  lump *directory = malloc(sizeof(lump) * h.lump_count);
  int offset = 0;
  for (int i = 0; i < h.lump_count; i++) {
    offset = h.init_offset + i * 16;
    lump l = {.lump_offset = read_i32(f, offset),
              .lump_size = read_i32(f, offset + 4),
              .lump_name = read_string(f, offset + 8, 8)

    };
    directory[i] = l;
  }
  return directory;
}

u16 get_lump_index(lump *directory, char *lump_name, int lump_count) {
  for (int i = 0; i < lump_count; i++) {
    if (strcmp(directory[i].lump_name, lump_name) == 0) {
      return i;
    }
  }
  return -1;
}
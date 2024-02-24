#include "../include/thing.h"

thing read_thing(FILE *f, int offset) {
  thing t;
  t.x = read_i16(f, offset);
  t.y = read_i16(f, offset + 2);
  t.angle = read_i16(f, offset + 4);
  t.type = read_i16(f, offset + 6);
  t.flags = read_i16(f, offset + 8);
  return t;
}

thing *get_things_from_lump(FILE *f, lump *directory, int lump_index,
                            int num_bytes, int header_length, int len_things) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  thing *things = malloc(sizeof(thing) * len_things);
  for (int i = 0; i < len_things; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    things[i] = read_thing(f, offset);
  }
  return things;
}
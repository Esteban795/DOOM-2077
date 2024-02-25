#include "../include/header.h"

header read_header(FILE *f) {
  header head = {.wad_type = read_string(f, 0, 4),
                 .lump_count = read_i32(f, 4),
                 .init_offset = read_i32(f, 8)};
  return head;
}
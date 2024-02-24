#include "../include/vertex.h"

vertex read_vertex(FILE *f, int offset) {
  vertex pos = {.x = read_i16(f, offset), .y = read_i16(f, offset + 2)};
  return pos;
}

vertex *get_vertexes_from_lump(FILE *f, lump *directory, int lump_index,
                               int num_bytes, int header_length,
                               int len_vertexes) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  vertex *vertexes = malloc(sizeof(vertex) * len_vertexes);
  for (int i = 0; i < len_vertexes; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    vertexes[i] = read_vertex(f, offset);
  }
  return vertexes;
}
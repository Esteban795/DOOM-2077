#include "../include/node.h"

node read_node(FILE *f, int offset) {
  node n;
  n.x_partition = read_i16(f, offset);
  n.y_partition = read_i16(f, offset + 2);
  n.dx_partition = read_i16(f, offset + 4);
  n.dy_partition = read_i16(f, offset + 6);
  n.front_bbox.top = read_i16(f, offset + 8);
  n.front_bbox.bottom = read_i16(f, offset + 10);
  n.front_bbox.left = read_i16(f, offset + 12);
  n.front_bbox.right = read_i16(f, offset + 14);
  n.back_bbox.top = read_i16(f, offset + 16);
  n.back_bbox.bottom = read_i16(f, offset + 18);
  n.back_bbox.left = read_i16(f, offset + 20);
  n.back_bbox.right = read_i16(f, offset + 22);
  n.front_child_id = read_i16(f, offset + 24);
  n.back_child_id = read_i16(f, offset + 26);
  return n;
}

node *get_nodes_from_lump(FILE *f, lump *directory, int lump_index,
                          int num_bytes, int header_length, int len_nodes) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  node *nodes = malloc(sizeof(node) * len_nodes);
  for (int i = 0; i < len_nodes; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    nodes[i] = read_node(f, offset);
  }
  return nodes;
}

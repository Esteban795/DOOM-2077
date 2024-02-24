#ifndef NODE_H
#define NODE_H

#include "byte_reader.h"
#include "lump.h"
// https://doomwiki.org/wiki/Node

struct BBox {
  i16 top;
  i16 bottom;
  i16 left;
  i16 right;
};

typedef struct BBox bbox;

struct Node {
  i16 x_partition;
  i16 y_partition;
  i16 dx_partition;
  i16 dy_partition;
  bbox back_bbox;
  bbox front_bbox;
  i16 front_child_id;
  i16 back_child_id;
};

typedef struct Node node;

node read_node(FILE *wad_file, int offset);

node *get_nodes_from_lump(FILE *f, lump *directory, int lump_index,
                          int num_bytes, int header_length, int len_nodes);
#endif
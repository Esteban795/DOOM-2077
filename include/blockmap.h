#ifndef BLOCKMAP_H
#define BLOCKMAP_H

#include "byte_reader.h"
#include "linedef.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_START ((int16_t)0x0000)
#define BLOCK_END ((int16_t)0xFFFF)

// https://doomwiki.org/wiki/Blockmap

// A block is a 128x128 unit square that contains a list of linedefs that are incident to the block
struct Block {
  linedef **linedefs;
  size_t nlinedefs;
};

typedef struct Block block;


struct BlockmapHeader {
  i16 x;
  i16 y;
  i16 ncols;
  i16 nrows;
};

typedef struct BlockmapHeader blockmap_header;

struct Blockmap {
  blockmap_header *header;
  block *blocks;
  size_t nblocks;
};

typedef struct Blockmap blockmap;

blockmap *read_blockmap_from_lump(FILE *f, lump *directory, int lump_index,
                                  linedef **linedefs);

void blockmap_free(blockmap *bm);

// Given `x` and `y` coords, return the index of the block that contains the point
int blockmap_get_block_index(blockmap *bm, int x, int y);
#endif
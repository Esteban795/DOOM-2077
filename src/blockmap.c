#include "../include/blockmap.h"

static blockmap_header *read_blockmap_header(FILE *f, int offset) {
  blockmap_header *bh = malloc(sizeof(blockmap_header));
  bh->x = read_i16(f, offset);
  bh->y = read_i16(f, offset + 2);
  bh->ncols = read_u16(f, offset + 4);
  bh->nrows = read_u16(f, offset + 6);
  return bh;
}

static size_t find_number_of_linedefs(FILE *f, int lump_offset,
                                      int block_offset) {
  size_t nlinedefs = 0;
  i16 temp;
  int init_offset = lump_offset + block_offset;
  if ((temp = read_i16(f, init_offset)) != BLOCK_START)
    return 0; // make sure we are at the start of the block
  int offset = init_offset + 2;
  while ((temp = read_u16(f, offset)) != BLOCK_END) {
    offset += 2;
    nlinedefs += 1;
  }
  return nlinedefs;
}

static block read_block(FILE *f, int lump_offset, int block_offset,
                        linedef *linedefs) {
  block b;
  b.nlinedefs = find_number_of_linedefs(f, lump_offset, block_offset);
  int offset = lump_offset + block_offset + 2; // Skip BLOCK_START
  b.linedefs = malloc(sizeof(linedef) * b.nlinedefs);
  for (size_t i = 1; i < b.nlinedefs; i++) {
    linedef l = linedefs[read_i16(f, offset)];
    b.linedefs[i] = l;
  }
  return b;
}

blockmap *read_blockmap(FILE *f, int lump_offset, linedef *linedefs) {
  const int HEADER_LENGTH = 8;
  blockmap *bm = malloc(sizeof(blockmap));
  bm->header = read_blockmap_header(f, lump_offset);
  bm->nblocks = bm->header->ncols * bm->header->nrows;
  bm->blocks = malloc(sizeof(block) * bm->nblocks);
  i16 *block_offsets = malloc(sizeof(i16) * bm->nblocks);
  for (size_t i = 0; i < bm->nblocks; i++) {
    block_offsets[i] = 2 * read_i16(f, lump_offset + HEADER_LENGTH + i * 2);
  }
  for (size_t i = 0; i < bm->nblocks; i++) {
    bm->blocks[i] = read_block(f, lump_offset, block_offsets[i], linedefs);
  }
  return bm;
}

blockmap *read_blockmap_from_lump(FILE *f, lump *directory, int lump_index,
                                  linedef *linedefs) {
  lump lump_info = directory[lump_index];
  return read_blockmap(f, lump_info.lump_offset, linedefs);
}

void blockmap_free(blockmap *bm) {
  free(bm->header);
  for (size_t i = 0; i < bm->nblocks; i++) {
    free(bm->blocks[i].linedefs);
  }
  free(bm->blocks);
  free(bm);
}
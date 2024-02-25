#ifndef WAD_DATA_H
#define WAD_DATA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "blockmap.h"
#include "header.h"
#include "linedef.h"
#include "node.h"
#include "sector.h"
#include "segment.h"
#include "sidedef.h"
#include "subsector.h"
#include "thing.h"
#include "vertex.h"

#define DT 2

struct WADData {
  header header;
  lump *directory;
  int map_index;
  vertex *vertexes;
  linedef *linedefs;
  node *nodes;
  subsector *subsectors;
  segment *segments;
  thing *things;
  blockmap *blockmap;
  sector *sectors;
  sidedef *sidedefs;
  int len_vertexes;
  int len_linedefs;
  int len_nodes;
  int len_subsectors;
  int len_segments;
  int len_things;
  int len_sectors;
  int len_sidedefs;
};

typedef struct WADData wad_data;

wad_data *init_wad_data(const char *path);

void wad_data_free(wad_data *wd);
#endif
#include "../include/wad_data.h"
#include <stdio.h>

wad_data *init_wad_data(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  wad_data *wd = malloc(sizeof(wad_data));
  wd->header = read_header(file);
  wd->directory = read_directory(file, wd->header);
  wd->map_index = get_lump_index(wd->directory, "E1M1", wd->header.lump_count);
  wd->len_vertexes = wd->directory[wd->map_index + VERTEXES].lump_size /
                     4; // 4 = number of bytes per vertex
  wd->len_linedefs = wd->directory[wd->map_index + LINEDEFS].lump_size /
                     14; // 14 = number of bytes per linedef
  wd->len_nodes = wd->directory[wd->map_index + NODES].lump_size /
                  28; // 28 = number of bytes per node
  wd->len_subsectors = wd->directory[wd->map_index + SSECTORS].lump_size /
                       4; // 4 = number of bytes per subsector
  wd->len_segments = wd->directory[wd->map_index + SEGS].lump_size /
                     12; // 12 = number of bytes per seg
  wd->len_things = wd->directory[wd->map_index + THINGS].lump_size /
                   10; // 10 = number of bytes per thing
  wd->len_sectors = wd->directory[wd->map_index + SECTORS].lump_size /
                    26; // 26 = number of bytes per sector
  wd->len_sidedefs = wd->directory[wd->map_index + SIDEDEFS].lump_size /
                     30; // 30 = number of bytes per sidedef
  wd->vertexes = get_vertexes_from_lump(
      file, wd->directory, wd->map_index + VERTEXES, 4, 0, wd->len_vertexes);
  wd->linedefs =
      get_linedefs_from_lump(file, wd->directory, wd->map_index + LINEDEFS, 14,
                             0, wd->len_linedefs, wd->vertexes);
  wd->nodes = get_nodes_from_lump(file, wd->directory, wd->map_index + NODES,
                                  28, 0, wd->len_nodes);
  wd->segments =
      get_segments_from_lump(file, wd->directory, wd->map_index + SEGS, 12, 0,
                             wd->len_segments, wd->vertexes, wd->linedefs);
  wd->subsectors =
      get_subsectors_from_lump(file, wd->directory, wd->map_index + SSECTORS, 4,
                               0, wd->len_subsectors, wd->segments);
  wd->things = get_things_from_lump(file, wd->directory, wd->map_index + THINGS,
                                    10, 0, wd->len_things);
  wd->blockmap = read_blockmap_from_lump(
      file, wd->directory, wd->map_index + BLOCKMAP, wd->linedefs);
  wd->sectors = get_sectors_from_lump(
      file, wd->directory, wd->map_index + SECTORS, 26, 0, wd->len_sectors);
  wd->sidedefs = get_sidedefs_from_lump(
      file, wd->directory, wd->map_index + SIDEDEFS, 30, 0, wd->len_sidedefs);
  return wd;
}

void wad_data_free(wad_data *wd) {
  free(wd->vertexes);
  free(wd->linedefs);
  free(wd->nodes);
  free(wd->segments);
  free(wd->things);
  free(wd->header.wad_type);
  free(wd->sectors);
  free(wd->sidedefs);
  blockmap_free(wd->blockmap);
  subsectors_free(wd->subsectors, wd->len_subsectors);
  for (int i = 0; i < wd->header.lump_count; i++) {
    free(wd->directory[i].lump_name);
  }
  free(wd->directory);
  free(wd);
}